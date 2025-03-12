/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientUDP.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 22:59:15 by topiana-          #+#    #+#             */
/*   Updated: 2025/03/13 00:05:31 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "socket_is_closed.h"

static int	update_player(const char *ip, const char *coords, t_player *player)
{
	char	**split;

	printf("updating player %s\n", ip);
	if (ip == NULL)
	{
		perror( "bad ip" );
		return (0);
	}
	split = ft_split(coords, '-');
	if (split == NULL)
	{
		perror( "bad coords" );
		ft_freentf("2", split);
		return (0);
	}
	player->pos.x = ft_atoi(split[0]);
	player->pos.y = ft_atoi(split[1]);
	player->pos.z = ft_atoi(split[2]);
	ft_freentf("2", split);
	return (1);
}

/* moves the server's player 'player[0]'. 
NOTE: the 'player[1]' is moved by the minigame. */
static int	handle_players(const char *buffer, t_recenv *recenv)
{
	char	**split;

	split = ft_split(buffer, ':');
	if (split == NULL || recenv == NULL)
		return (0);
	if (!update_player(split[0], split[1], &recenv->player[0]))
		return (0);
	printf("%s at %f\n", recenv->player[0].ip, recenv->player[0].pos.x);
	printf("%s at %f\n", recenv->player[1].ip, recenv->player[1].pos.x);
	ft_freentf("2", split);
	return (1);
}

/* static  */void	*client_reciever(void *arg)
{
	t_recenv	*recenv;

	recenv = (t_recenv *)arg;
	/* int connfd;
    if ( (connfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 )
	{
        perror( "reciever socket failed" );
        return (NULL);
    }
	printf("connfd=%i\n", connfd);

    struct sockaddr_in servaddr;
    memset( &servaddr, 0, sizeof(servaddr) );
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons( 42042 );
	servaddr.sin_addr.s_addr = inet_addr(get_serv_ip(recenv->env)); //client setup

	//connecting to the server
	if ( connect(connfd, (struct sockaddr *)&servaddr, sizeof(struct sockaddr_in)) < 0 )
	{
		perror( "reciever bind failed" );
		return (NULL);
	}
	printf("connected to the server\n"); */
	
    /* struct sockaddr_in addrin;
    socklen_t addr_len = sizeof(struct sockaddr_in); */
	
    char buffer[MAXLINE];
    while ( 1 )
	{
		ft_printf("talk to me... on %d\n", recenv->player[1].socket);
        int length = recv( recenv->player[1].socket, buffer, sizeof(buffer) - 1, 0 );
        if ( length < 0 ) {
			perror( "recv failed" );
            break;
        }
		printf( "%d bytes: '%s' from Server\n", length, buffer);
		handle_players(buffer, recenv);
	}
	close(recenv->player[1].socket);
	return (NULL);
}

void toutorial_init2(t_player *player, char **env)
{
	int status, valread, client_fd;
    struct sockaddr_in serv_addr;
    char* hello = "Hello from client";
    char buffer[1024] = { 0 };
	
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary
    // form
    if (inet_pton(AF_INET, get_serv_ip(env), &serv_addr.sin_addr)
        <= 0) {
        printf(
            "\nInvalid address/ Address not supported \n");
        return ;
    }

    if ((status
         = connect(client_fd, (struct sockaddr*)&serv_addr,
                   sizeof(serv_addr)))
        < 0) {
        printf("\nConnection Failed \n");
        return ;
    }
    send(client_fd, hello, strlen(hello), 0);
    printf("Hello message sent\n");
    valread = read(client_fd, buffer,
                   1024 - 1); // subtract 1 for the null
                              // terminator at the end
    printf("%s\n", buffer);

    // closing the connected socket
    close(client_fd);
    player->num = 1;
	printf("init done!\n");
}

void toutorial_init(t_player *player, char **env)
{

	struct addrinfo hints, *res;
	int sockfd;

	// first, load up address structs with getaddrinfo():

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	printf("addrinfo?\n");
	getaddrinfo(get_serv_ip(env), "8080", &hints, &res);

	// make a socket:

	printf("socket?\n");
	sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

	// connect!

	printf("connected?\n");
	connect(sockfd, res->ai_addr, res->ai_addrlen);

	printf("sending?\n");
	if (send( sockfd, get_locl_ip(env), 15, 0 ) < 0 )
		perror( "sendto failed" ); 
		
	/* memmove(&player->sockaddr, &serv->ai_addr, sizeof(struct sockaddr_in));
	memmove(&player->ip, get_locl_ip(env), 15);
	memmove(&player->name, "pippo", 5);
	player->socket = connfd; */
	player->num = 1;
	printf("init done!\n");
}

/* static  */void	client_player_init(t_player *player, char **env)
{
	int connfd;
	if ( (connfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
		perror("socket failed");
		return ;
	}
	printf("connfd=%i\n", connfd);
	
	//serveraddr setup (bind to socket to send message to the server)
	struct sockaddr_in serveraddr;
	memset( &serveraddr, 0, sizeof(serveraddr) );
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons( 42042 );              
	serveraddr.sin_addr.s_addr = inet_addr(get_serv_ip(env));

	//connecting to the server
	if ( connect(connfd, (struct sockaddr *)&serveraddr, sizeof(struct sockaddr_in)) < 0 )
	{
		perror( "reciever bind failed" );
		return ;
	}
	printf("connected to the server\n");

	//send test
	if (sendto( connfd, get_locl_ip(env), 15, 0, (struct sockaddr *)&serveraddr, sizeof(struct sockaddr_in)) < 0 )
		perror( "sendto failed" );
		
	memmove(&player->sockaddr, &serveraddr, sizeof(struct sockaddr_in));
	memmove(&player->ip, get_locl_ip(env), 15);
	memmove(&player->name, "pippo", 5);
	player->socket = connfd;
	player->num = 1;
}

int client_routine( int argc, char *argv[], char *env[])
{
	(void)argc; (void)argv;

	ft_printf("LOCAL_IP=%s, SERVER_IP=%s\n", get_locl_ip(env), get_serv_ip(env));

	//player[0] = server, player[1] = client
	t_player player[2];
	memset(&player, 0, 2 * sizeof(t_player));

	//client_player_init(&player[1], env);
	toutorial_init(&player[1], env);

	player_specs(player[1]);

	t_recenv	recenv;
	
	recenv.env = env;
	recenv.player = &player[0];
	recenv.max_players = 2;
	
    //reciever
	pthread_t	tid;
	if (pthread_create(&tid, NULL, &client_reciever, &recenv) < 0)
		perror( "reciever launch failed" );

    minigame(1, &player[0]);

	pthread_join(tid, NULL);
	return (0);
}
