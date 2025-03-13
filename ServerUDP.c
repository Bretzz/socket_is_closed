/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerUDP.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: totommi <totommi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 22:59:15 by topiana-          #+#    #+#             */
/*   Updated: 2025/03/13 01:07:42 by totommi          ###   ########.fr       */
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
		ft_freentf("2", split);
		perror( "bad coords" );
		return (0);
	}
	player->pos.x = ft_atoi(split[0]);
	player->pos.y = ft_atoi(split[1]);
	player->pos.z = ft_atoi(split[2]);
	ft_freentf("2", split);
	return (1);
}

/* static int	register_player(const char *ip, const char *coords, t_player *player)
{
	char	**split;

	printf("registering new player...\n");
	usleep(1000000);
	if (ip == NULL)
	{
		perror( "bad ip" );
		return (0);
	}
	memmove(player->ip, ip, 15);
	if ( (player->socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
		perror("socket failed");
		return (0);
	}
	printf("sendfd=%i\n", player->socket);

	struct sockaddr_in playeraddr;
	memset( &playeraddr, 0, sizeof(playeraddr) );
	
    playeraddr.sin_family = AF_INET;
    playeraddr.sin_port = htons( 50037 );              
    playeraddr.sin_addr.s_addr = inet_addr(player->ip);

	//send test
	if (sendto( player->socket, "Hi there!", 9, 0, (struct sockaddr *)&playeraddr, sizeof(struct sockaddr_in)) < 0 )
		perror( "sendto failed" );

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
	printf("new player registered:\n");
	player_specs(*player);
	return (1);
} */

static int	register_player(const char *ip, int playersocket, const char *coords, t_player *player)
{
	char	**split;

	printf("registering new player...\n");
	usleep(1000000);
	if (ip == NULL)
	{
		perror( "bad ip" );
		return (0);
	}
	memmove(player->ip, ip, 15);
	player->socket = playersocket;
	printf("sendfd=%i\n", player->socket);

	//send test
	if (send( player->socket, "Hi there!", 9 , 0))
		perror( "sendto failed" );

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
	printf("new player registered:\n");
	player_specs(*player);
	return (1);
}

/* single second player handling. 
NOTE: player[0] is updated by minigame*/
/* static  *//* int	handle_players(const char *buffer, t_recenv *recenv)
{
	char			**split;

	split = ft_split(buffer, ':');
	if (split == NULL || recenv == NULL)
		return (0);
	if (recenv->player[1].ip[0] == '\0')
	{
		if (!register_player(split[0], split[1], &recenv->player[1]))
			return (0);
	}
	else if (is_ip(recenv->player[1].ip))
	{
		if (!update_player(split[0], split[1], &recenv->player[1]))
			return (0);
	}
	printf("%-15s : %f\n", recenv->player[0].ip, recenv->player[0].pos.x);
	printf("%-15s : %f\n", recenv->player[1].ip, recenv->player[1].pos.x);
	ft_freentf("2", split);
	return (1);
} */

/* single second player handling. 
NOTE: player[0] is updated by minigame*/
static int	handle_players(const char *buffer, t_recenv *recenv)
{
	char			**split;

	split = ft_split(buffer, ':');
	if (split == NULL || recenv == NULL)
		return (0);
	if (is_ip(recenv->player[1].ip))
	{
		if (!update_player(split[0], split[1], &recenv->player[1]))
			return (0);
	}
	printf("%-15s : %f\n", recenv->player[0].ip, recenv->player[0].pos.x);
	printf("%-15s : %f\n", recenv->player[1].ip, recenv->player[1].pos.x);
	ft_freentf("2", split);
	return (1);
}

void	*toutorial_reciever2(void *arg)
{
	int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    socklen_t addrlen = sizeof(address);

	(void)arg;
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET,
                   SO_REUSEADDR | SO_REUSEPORT, &opt,
                   sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr*)&address,
             sizeof(address))
        < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((new_socket
         = accept(server_fd, (struct sockaddr*)&address,
                  &addrlen))
        < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

	struct sockaddr_in addrin;
	socklen_t addr_len = sizeof(struct sockaddr_in);

	char buffer[MAXLINE] = { 0 };
    while ( 1 )
	{
		ft_printf("talk to me baby...\n");
        int length = recvfrom( new_socket, buffer, sizeof(buffer) - 1, 0, (struct sockaddr *)&addrin, &addr_len );
        if ( length < 0 ) {
			perror( "recvfrom failed" );
            break;
        }
		printf( "%d bytes: '%s' from %s\n", length, buffer, inet_ntoa(addrin.sin_addr));
	}
	return (NULL);
}

void	*toutorial_reciever(void *arg)
{
	struct sockaddr_storage their_addr;
    socklen_t addr_size;
    struct addrinfo hints, *res;
    int sockfd, new_fd;
	
	(void)arg;
    // !! don't forget your error checking for these calls !!

    // first, load up address structs with getaddrinfo():

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;  // use IPv4 or IPv6, whichever
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;     // fill in my IP for me

    getaddrinfo(NULL, "8080", &hints, &res);

    // make a socket, bind it, and listen on it:

    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (bind(sockfd, res->ai_addr, res->ai_addrlen))
		perror( "bind fail" );
    if (listen(sockfd, 10))
		perror( "listen fail" );
	printf("listening...\n");

    // now accept an incoming connection:

    addr_size = sizeof their_addr;
    new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &addr_size);
	printf("connection acceppted!\n");

	struct sockaddr_in addrin;
	socklen_t addr_len = sizeof(struct sockaddr_in);

	char buffer[MAXLINE];
    while ( 1 )
	{
		ft_printf("talk to me baby...\n");
        int length = recvfrom( new_fd, buffer, sizeof(buffer) - 1, 0, (struct sockaddr *)&addrin, &addr_len );
        if ( length < 0 ) {
			perror( "recvfrom failed" );
            break;
        }
		printf( "%d bytes: '%s' from %s\n", length, buffer, inet_ntoa(addrin.sin_addr));
	}
	return (NULL);
}

/* static  */void	*server_reciever(void *arg)
{
	t_recenv	*recenv;

	recenv = (t_recenv *)arg;
	
	/* struct addrinfo hints, *recvinfo;
	
	memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;  // use IPv4 or IPv6, whichever
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = 0; //AI_PASSIVE     // fill in my IP for me

    getaddrinfo(NULL, "42042", &hints, &recvinfo); */
	
	int recvfd, playerfd[2];
    if ( (recvfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
	{
        perror( "reciever socket failed" );
        return (NULL);
    }
	printf("recvfd=%i\n", recvfd);

    struct sockaddr_in recvaddr;
    memset( &recvaddr, 0, sizeof(recvaddr) );
    recvaddr.sin_family = AF_INET;
    recvaddr.sin_port = htons( 42042 );
	recvaddr.sin_addr.s_addr = htonl( INADDR_ANY ); //host setup
    //inet_pton(AF_INET, "127.0.0.1", &recvaddr);
	
	//binding host reciever socket to ANY address
	if ( bind(recvfd, (struct sockaddr *)&recvaddr, sizeof(struct sockaddr_in)) < 0 )
	{
		perror( "reciever bind failed" );
		return (NULL);
	}

	
	//listening for 1 connection
	if ( listen(recvfd, 1) < 0 )
	{
		perror( "listen failed" );
		return (NULL);
	}
	ft_printf("I'm listening...\n");
	
	struct sockaddr_in addrin;
	socklen_t addr_len = sizeof(struct sockaddr_in);

	//accepting 1 connection
	playerfd[0] = accept(recvfd, (struct sockaddr *)&addrin, &addr_len);
	if ( playerfd[0] < 0 )
	{
		perror( "accept failed" );
		return (NULL);
	}
	ft_printf("I heard you!!!\n");
	register_player(inet_ntoa(addrin.sin_addr), playerfd[0], "0-0-0", &recenv->player[1]);

	char buffer[MAXLINE];
    while ( 1 )
	{
		ft_printf("talk to me baby...\n");
        int length = recvfrom( playerfd[0], buffer, sizeof(buffer) - 1, 0, (struct sockaddr *)&addrin, &addr_len );
        if ( length < 0 ) {
			perror( "recvfrom failed" );
            break;
        }
		printf( "%d bytes: '%s' from %s\n", length, buffer, inet_ntoa(addrin.sin_addr));
		handle_players(buffer, recenv);
	}
	close(recvfd);
	return (NULL);
}

static void	host_player_init(t_player *player, char **env)
{
	memmove(&player->ip, get_serv_ip(env), 15);
	memmove(&player->name, "pippo host", 10);
	player->num = 0;
}

int server_routine( int argc, char *argv[], char *env[])
{
	(void)argc; (void)argv;

	ft_printf("LOCAL_IP=%s, SERVER_IP=%s\n", get_locl_ip(env), get_serv_ip(env));

	t_player player[2];
	memset(&player, 0, 2 * sizeof(t_player));
	
	//player init
	host_player_init(&player[0], env);

	player_specs(player[0]);

	pthread_t	tid;
	t_recenv	recenv;

	recenv.env = env;
	recenv.player = player;
	recenv.max_players = 2;

    //reciever
	if (pthread_create(&tid, NULL, &toutorial_reciever2, &recenv) < 0)
		perror( "reciever launch failed" );

    minigame(0, &player[0]);

	pthread_join(tid, NULL);
    //close( sendfd );
	return (0);
}
