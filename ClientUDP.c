/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientUDP.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 22:59:15 by topiana-          #+#    #+#             */
/*   Updated: 2025/03/12 22:31:09 by topiana-         ###   ########.fr       */
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

static void	client_player_init(t_player *player, char **env)
{
	/* int sendfd;
	if ( (sendfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
		perror("socket failed");
		return ;
	}
	printf("sendfd=%i\n", sendfd);
	
	//serveraddr setup (bind to socket to send message to the server)
	struct sockaddr_in serveraddr;
	memset( &serveraddr, 0, sizeof(serveraddr) );
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons( 50037 );              
	serveraddr.sin_addr.s_addr = inet_addr(get_serv_ip(env)); */
	
	int connfd;
    if ( (connfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 )
	{
        perror( "reciever socket failed" );
        return ;
    }
	printf("connfd=%i\n", connfd);

    struct sockaddr_in serveraddr;
    memset( &serveraddr, 0, sizeof(serveraddr) );
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons( 42042 );
	serveraddr.sin_addr.s_addr = inet_addr(get_serv_ip(env)); //client setup

	//binding reciever socket to server address
	/* if ( bind(recvfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0 )
	{
		perror( "reciever bind failed" );
		return ;
	} */

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
	client_player_init(&player[1], env);

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
