/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientUDP.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: totommi <totommi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 22:59:15 by topiana-          #+#    #+#             */
/*   Updated: 2025/03/15 17:39:37 by totommi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "socket_is_closed.h"

//updating player[0]'s data
static int	update_player(const char *ip, const char *coords, const char *target, t_player *player)
{
	char	**split;

	ft_printf(PURPLE"updating player %s...\n"RESET, ip);
	if (ip == NULL)
	{
		ft_printf(RED"bad ip: null string\n"RESET);
		return (0);
	}
	split = ft_split(coords, '_');
	if (split == NULL)
	{
		ft_printf(RED"bad coords: null string\n"RESET);
		ft_freentf("2", split);
		return (0);
	}
	player->pos.x = ft_atoi(split[0]);
	player->pos.y = ft_atoi(split[1]);
	player->pos.z = ft_atoi(split[2]);
	ft_freentf("2", split);
	split = ft_split(target, '_');
	if (split == NULL)
	{
		ft_printf(RED"bad target: null string\n"RESET);
		ft_freentf("2", split);
		return (1);
	}
	player->target.x = ft_atoi(split[0]);
	player->target.y = ft_atoi(split[1]);
	player->target.z = ft_atoi(split[2]);
	ft_freentf("2", split);
	return (1);
}

static int	kill_player(t_player *player)
{
	ft_printf(KILL"killed player %s%s\n", player->ip, RESET);
	ft_memset(player, 0, sizeof(t_player));
	return (1);
}

static int	one_player_data(const char *buffer, t_recenv *recenv)
{
	char	**split;
	size_t	len;
	int		i;
	
	split = ft_split(buffer, ':');
	if (split == NULL || recenv == NULL)
		return (0);
	if (!ft_strncmp("host", split[0], 4)) //update host
	{
		if (!ft_strncmp("died", split[1], 4))  //kill host
		{
			kill_player(&recenv->player[0]);
			return (ft_freentf("2", split), -1);
			//host redirect procedure
		}
		else if (!update_player(split[0], split[1], split[2], &recenv->player[0]))
			return (ft_freentf("2", split), 0);
		return (1);
	}
	len = ft_strlen(split[0]);
	if (!is_ip(split[0]) || !ft_strncmp(recenv->player[1].ip, split[0], len))
		return (0);
	i = 2; //0 = host, 1 = self, >1 = extra players
	while (i < MAXPLAYERS)
	{
		if (!ft_strncmp(split[0], recenv->player[i].ip, len)) //update other players
		{
			if (!ft_strncmp("died", split[1], 4))  //kill player
				kill_player(&recenv->player[i]);
			else if (!update_player(split[0], split[1], split[2], &recenv->player[i]))
				return (ft_freentf("2", split), 0);
			return (ft_freentf("2", split), 1);
		}
		if (recenv->player[i].ip[0] == '\0')
			break ;
		i++;
	}
	//adding new player
	ft_memmove(&recenv->player[i].ip, split[0], len);
	ft_memmove(&recenv->player[i].name, "new_player", 10);
	recenv->player[i].num = i;
	ft_printf(GREEN"A new player appeared!\n"RESET);
	if (!update_player(split[0], split[1], split[2], &recenv->player[i]))
		return (ft_freentf("2", split), 0);
	ft_freentf("2", split);
	return (1);
}

/* moves the server's player 'player[0]'. 
NOTE: the 'player[1]' is moved by the minigame. */
static int	handle_players(const char *buffer, t_recenv *recenv)
{
	char	**split;
	int		i;

	split = ft_split(buffer, '\n'); //not used, but eventually can hanndle more than one player data
	if (split == NULL || recenv == NULL)
		return (0);
	i = 0;
	while (split[i] != NULL)
	{
		if (one_player_data(split[i++], recenv) < 0)
			return (ft_freentf("2", split), -1);
	}
	ft_printf(STATS);
	printf("%-15s at %d_%d_%d\n", recenv->player[0].ip, (int)recenv->player[0].pos.x, (int)recenv->player[0].pos.y, (int)recenv->player[0].pos.z);
	printf("%-15s at %d_%d_%d\n", recenv->player[1].ip, (int)recenv->player[1].pos.x, (int)recenv->player[1].pos.y, (int)recenv->player[1].pos.z);
	ft_printf(RESET);
	ft_freentf("2", split);
	return (1);
}

static void	*client_reciever(void *arg)
{
	t_recenv	*recenv;
	int			len;

	recenv = (t_recenv *)arg;
	
	char buffer[MAXLINE];
	while ( 1 )
	{
		ft_printf("talk to me...\n");
		len = recv( recenv->player[0].socket, buffer, MAXLINE - 1, 0 );
		if ( len < 0 ) {
			perror(ERROR"recv failed"RESET);
			break;
		}
		ft_printf(YELLOW"%d bytes: '%s' from Server\n"RESET, len, buffer);
		if (handle_players(buffer, recenv) < 0)
		{
			ft_printf(HOST"A NEW HOST WILL RISE%s\n", RESET);
			return (close(recenv->player[0].socket), NULL);
		}
	}
	close(recenv->player[0].socket);
	return (NULL);
}

static int client_player_init(t_player *player, char **env)
{
	int		servfd;
	char	buffer[30];
	
	if ((servfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror(ERROR"socket failure"RESET);
		return (0);
	}
	
	struct sockaddr_in serveraddr;

	ft_memset(&serveraddr, 0, sizeof(struct sockaddr_in));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons ( MYPORT );
	serveraddr.sin_addr.s_addr = inet_addr(get_serv_ip(env)); //192.168.1.5 //INADDR_ANY

	if (connect(servfd, (struct sockaddr *)&serveraddr, sizeof(struct sockaddr)))
	{
		perror(ERROR"connectin failed"RESET);
		return (0);
	}
	ft_printf(GREEN"connection accepted!!!\n"RESET);

	ft_memset(buffer, 0, 30);
	ft_strlcpy(buffer, get_locl_ip(env), 16);
	ft_strlcat(buffer, ":", ft_strlen(buffer) + 2);
	ft_strlcat(buffer, get_my_name(env), ft_strlen(buffer) + 11);
	
	//send test
	if (send(servfd, buffer, 30, 0) < 0)
		perror(ERROR"send failure"RESET);

	//host init
	ft_memmove(&player[0].sockaddr, &serveraddr, sizeof(struct sockaddr_in));
	ft_memmove(&player[0].ip, "host", 4);
	ft_memmove(&player[0].name, "host", 4);
	player[0].socket = servfd;
	//player init
	ft_memmove(&player[1].ip, get_locl_ip(env), 15);
	ft_memmove(&player[1].name, get_my_name(env), 10);
	player[1].num = 1;
	return (1);
}

int client_routine( int argc, char *argv[], char *env[])
{
	(void)argc; (void)argv;

	ft_printf("LOCAL_IP=%s, SERVER_IP=%s\n", get_locl_ip(env), get_serv_ip(env));

	//player[0] = server, player[1] = client
	t_player player[MAXPLAYERS];
	ft_memset(&player, 0, MAXPLAYERS * sizeof(t_player));

	//initialize the data to connect to the server
	if (!client_player_init(&player[0], env))
		return (0);

	ft_printf("===  HOST  ===\n");
	player_specs(player[0]);
	ft_printf("=== PLAYER ===\n");
	player_specs(player[1]);
	ft_printf("= == == == = =\n");

	t_recenv	recenv;
	
	recenv.env = env;
	recenv.player = &player[0];
	
	//reciever
	pthread_t	tid;
	if (pthread_create(&tid, NULL, &client_reciever, &recenv) < 0)
		perror(ERROR"reciever launch failed"RESET);

	minigame(1, &player[0]);

	pthread_join(tid, NULL);
	return (0);
}
