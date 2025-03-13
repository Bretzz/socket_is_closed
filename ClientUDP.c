/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientUDP.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 22:59:15 by topiana-          #+#    #+#             */
/*   Updated: 2025/03/13 12:26:20 by topiana-         ###   ########.fr       */
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
	printf("%-15s at %f\n", recenv->player[0].ip, recenv->player[0].pos.x);
	printf("%-15s at %f\n", recenv->player[1].ip, recenv->player[1].pos.x);
	ft_freentf("2", split);
	return (1);
}

/* static  */void	*client_reciever(void *arg)
{
	t_recenv	*recenv;

	recenv = (t_recenv *)arg;
	
	char buffer[MAXLINE];
	while ( 1 )
	{
		ft_printf("talk to me...\n");
		int length = recv( recenv->player[0].socket, buffer, MAXLINE - 1, 0 );
		if ( length < 0 ) {
			perror( "recv failed" );
			break;
		}
		printf( "%d bytes: '%s' from Server\n", length, buffer);
		handle_players(buffer, recenv);
	}
	close(recenv->player[0].socket);
	return (NULL);
}

static int client_player_init(t_player *player, char **env)
{
	int servfd;
	
	if ((servfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("socket failure");
		return (0);
	}
	
	struct sockaddr_in serveraddr;

	memset(&serveraddr, 0, sizeof(struct sockaddr_in));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons ( MYPORT );
	serveraddr.sin_addr.s_addr = inet_addr(get_serv_ip(env)); //192.168.1.5 //INADDR_ANY

	if (connect(servfd, (struct sockaddr *)&serveraddr, sizeof(struct sockaddr)))
	{
		perror("connectin failed");
		return (0);
	}
	printf("connection accepted!!!\n");

	if (send(servfd, get_locl_ip(env), 15, 0) < 0)
		perror("send failure");

	int len;
	char buffer[MAXLINE] = { 0 };
	if ((len = recv(servfd, buffer, MAXLINE - 1, 0)) < 0)
		perror("recv failure");
	else
		printf("%d bytes: '%s' from Server\n", len, buffer);

	//player init
	memmove(&player[1].ip, get_locl_ip(env), 15);
	memmove(&player[1].name, "pippo", 5);
	//host init
	memmove(&player->sockaddr, &serveraddr, sizeof(struct sockaddr_in));
	memmove(&player[0].ip, "host", 4);
	memmove(&player[0].name, "host", 4);
	player->socket = servfd;
	player->num = 1;
	return (1);
}

int client_routine( int argc, char *argv[], char *env[])
{
	(void)argc; (void)argv;

	ft_printf("LOCAL_IP=%s, SERVER_IP=%s\n", get_locl_ip(env), get_serv_ip(env));

	//player[0] = server, player[1] = client
	t_player player[2];
	memset(&player, 0, 2 * sizeof(t_player));

	//initialize the data to connect to the server
	if (!client_player_init(&player[0], env))
		return (0);

	printf("===  HOST  ===\n");
	player_specs(player[0]);
	printf("=== PLAYER ===\n");
	player_specs(player[1]);
	printf("= == == == = =\n");

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
