/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerUDP.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 22:59:15 by topiana-          #+#    #+#             */
/*   Updated: 2025/03/13 12:20:07 by topiana-         ###   ########.fr       */
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

static int	register_player(const char *ip, int playersocket, const char *coords, t_player *player)
{
	char	**split;

	printf("registering new player...\n");
	//usleep(1000000);
	if (ip == NULL)
	{
		perror( "bad ip" );
		return (0);
	}
	memmove(player->ip, ip, 15);
	player->socket = playersocket;
	printf("sendfd=%i\n", player->socket);
	
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

void	*server_reciever(void *arg)
{
	t_recenv *recenv;

	recenv = (t_recenv *)arg;
	
	int recvfd, connfd;
	if ((recvfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		perror("socket failure");

	struct sockaddr_in serveraddr;

	memset(&serveraddr, 0, sizeof(struct sockaddr_in));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons ( MYPORT );
	serveraddr.sin_addr.s_addr = htonl( INADDR_ANY );

	if (bind(recvfd, (struct sockaddr *)&serveraddr, sizeof(struct sockaddr_in)))
		perror("bind failure");

	if (listen(recvfd, 10))
		perror("listen failure");

	struct sockaddr_in clientaddr;
	socklen_t addrlen;

	memset(&clientaddr, 0, sizeof(struct sockaddr_in));
	if ((connfd = accept(recvfd, (struct sockaddr *)&clientaddr, &addrlen)) < 0)
		perror("accept failed");
	printf("connection accepted!\n");

	int	len;
	char buffer[MAXLINE] = { 0 };
	if ((len = recv(connfd, buffer, MAXLINE - 1, 0)) < 0)
		perror("recv failure");
	else
		printf("%d bytes: %s\n", len, buffer);

	register_player(buffer, connfd, "0-0-0", &recenv->player[1]);
	
	//write(connfd, "HTTP/1.0 200 OK\r\n\r\nHello sweety", 35);
	if (send(connfd, "HTTP/1.0 200 OK\r\n\r\nHello sweety", 35, 0) < 0)
		perror("send failure");

	struct sockaddr_in addrin;
	
	while ( 1 )
	{
		memset(buffer, 0, MAXLINE);
		ft_printf("talk to me baby...\n");
		int length = recvfrom( connfd, buffer, MAXLINE - 1, 0, (struct sockaddr *)&addrin, &addrlen );
		if ( length < 0 ) {
			perror( "recvfrom failed" );
			break;
		}
		printf( "%d bytes: '%s' from %s\n", length, buffer, inet_ntoa(addrin.sin_addr));
		handle_players(buffer, recenv);
	}
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
	if (pthread_create(&tid, NULL, &server_reciever, &recenv) < 0)
		perror( "reciever launch failed" );

	minigame(0, &player[0]);

	pthread_join(tid, NULL);
	//close( sendfd );
	return (0);
}
