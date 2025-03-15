/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerUDP.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: totommi <totommi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 22:59:15 by topiana-          #+#    #+#             */
/*   Updated: 2025/03/15 01:26:55 by totommi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "socket_is_closed.h"

//just looking, no mutex
static char	*get_lobby_stats(char *buffer, t_player *player)
{
	int	i;
	char	minibuffer[45];

	if (buffer == NULL || player == NULL)
		return (NULL);
	ft_memset(buffer, 0, (MAXPLAYERS + 1) * 45 * sizeof(char));
	i = 0;
	while (i < MAXPLAYERS)
	{
		ft_memset(minibuffer, 0, sizeof(minibuffer));
		if (player[i].ip[0] != '\0')
			get_pos(minibuffer,
				player[i].pos,
				player[i].target,
				player[i].ip);
		ft_strlcat(buffer, minibuffer, ft_strlen(buffer) + sizeof(minibuffer) + 1);
		if (i < MAXPLAYERS - 1 && player[i + 1].ip[0] != '\0')
			ft_strlcat(buffer, "\n", ft_strlen(buffer) + 2);
		i++;
	}
	return (buffer);
}

//mutex needed
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
	pthread_mutex_lock(player->mutex);
	player->pos.x = ft_atoi(split[0]);
	player->pos.y = ft_atoi(split[1]);
	player->pos.z = ft_atoi(split[2]);
	pthread_mutex_unlock(player->mutex);
	ft_freentf("2", split);
	split = ft_split(target, '_');
	if (split == NULL)
	{
		ft_printf(RED"bad target: null string\n"RESET);
		ft_freentf("2", split);
		return (1);
	}
	pthread_mutex_lock(player->mutex);
	player->target.x = ft_atoi(split[0]);
	player->target.y = ft_atoi(split[1]);
	player->target.z = ft_atoi(split[2]);
	pthread_mutex_unlock(player->mutex);
	ft_freentf("2", split);
	return (1);
}

//mutex needed (already locked)
static int	register_player(int index, const char *ip, int playersocket, const char *coords, t_player *player)
{
	char	**split;

	ft_printf(PURPLE"registering new player...\n"RESET);
	if (ip == NULL)
	{
		ft_printf(RED"bad ip: null string\n"RESET);
		return(0);
	}
	ft_memmove(player[index].ip, ip, 15);
	player[index].socket = playersocket;
	player[index].num = index;	
	split = ft_split(coords, '_');
	if (split == NULL)
	{
		ft_printf(RED"bad coords: null string\n"RESET);
		ft_freentf("2", split);
		return (1);
	}
	player[index].pos.x = ft_atoi(split[0]);
	player[index].pos.y = ft_atoi(split[1]);
	player[index].pos.z = ft_atoi(split[2]);
	ft_freentf("2", split);
	ft_printf(GREEN"NEW PLAYER REGISTERED:\n"RESET);
	player_specs(player[index]);
	return (1);
}

//mutex needed
static int	kill_player(t_player *player, char *dead)
{
	pthread_mutex_t	*mutex;

	mutex = player->mutex;
	pthread_mutex_lock(mutex);
	close(player->socket);
	ft_memset(player, 0, sizeof(t_player));
	*dead = 1;
	pthread_mutex_unlock(mutex);
	return (1);
}

/* single second player handling. 
NOTE: player[0] is updated by minigame*/
static int	handle_players(char *dead, const char *buffer, t_recenv *recenv)
{
	char			**split;
	char			lobby[45 * (MAXPLAYERS + 1)];
	int				i[2];
	
	split = ft_split(buffer, ':');
	if (split == NULL || recenv == NULL || !is_ip(split[0]))
		return (ft_freentf("2", split), 0);
	i[0] = 1;
	while (i[0] < MAXPLAYERS)
	{
		if (!ft_strncmp(split[0], recenv->player[i[0]].ip, 15))
		{
			if (!ft_strncmp("died", split[1], 4))  //kill player
				kill_player(&recenv->player[i[0]], dead);
			else
				update_player(split[0], split[1], split[2], &recenv->player[i[0]]);
			break ;
		}
		i[0]++;
	}
	get_lobby_stats(lobby, recenv->player);
	i[1] = 1;
	while (i[1] < MAXPLAYERS)
	{
		if (recenv->player[i[1]].ip[0] != '\0' && i[1] != i[0]
			&& (send(recenv->player[i[1]].socket, lobby, sizeof(lobby), 0) < 0))
			perror(ERROR"send failure"RESET);
		i[1]++;
	}
	ft_printf(STATS);
	printf("%-15s at %d_%d_%d\n", recenv->player[0].ip, (int)recenv->player[0].pos.x, (int)recenv->player[0].pos.y, (int)recenv->player[0].pos.z);
	printf("%-15s at %d_%d_%d\n", recenv->player[1].ip, (int)recenv->player[1].pos.x, (int)recenv->player[1].pos.y, (int)recenv->player[1].pos.z);
	ft_printf(RESET);
	return (ft_freentf("2", split), 1);
}

/* accept one connnection and starts to listen to it.
updates recenv->index so that the next thread can go onto
the next slot. */
void	*server_reciever(void *arg)
{
	t_recenv	*recenv;
	int			connfd;
	int			my_index;
	char		dead;

	recenv = (t_recenv *)arg;
	my_index = recenv->index;

	struct sockaddr_in clientaddr;
	socklen_t addrlen;

	ft_memset(&clientaddr, 0, sizeof(struct sockaddr_in));
	if ((connfd = accept(recenv->socket, (struct sockaddr *)&clientaddr, &addrlen)) < 0)
		perror(ERROR"accept failed"RESET);
	
	pthread_mutex_lock(&recenv->player_mutex);
	ft_printf(GREEN"connection accepted!\n"RESET);

	int	len;
	char buffer[MAXLINE] = { 0 };
	if ((len = recv(connfd, buffer, MAXLINE - 1, 0)) < 0) //expecting a message with ip:name
		perror(ERROR"recv failure"RESET);
	else
		ft_printf("%d bytes: %s\n", len, buffer);

	recenv->player[my_index].mutex = &recenv->player_mutex;
	register_player(my_index, buffer, connfd, "0_0_0", recenv->player); //register player one
	
	//send test
	if (send(recenv->player[my_index].socket, "HTTP/1.0 200 OK\r\n\r\nHello sweety", 35, 0) < 0)
		perror(ERROR"send failure"RESET);
	
	recenv->index++;
	pthread_mutex_unlock(&recenv->player_mutex);
	
	//how do I listen to multiple players?
	//WAIT
	pthread_t	tid;

	//reciever
	if (pthread_create(&tid, NULL, &server_reciever, recenv) < 0)
		perror(ERROR"reciever launch failed"RESET);
	pthread_detach(tid);
	
	struct sockaddr_in addrin;
	int		length;

	dead = 0;
	while (!dead)
	{
		ft_memset(buffer, 0, MAXLINE);
		ft_printf("talk to me baby...\n");
		length = recvfrom( recenv->player[my_index].socket, buffer, MAXLINE - 1, 0, (struct sockaddr *)&addrin, &addrlen );
		if ( length < 0 ) {
			perror(ERROR"recvfrom failed"RESET);
			break;
		}
		ft_printf(YELLOW"%d bytes: '%s' from %s\n"RESET, length, buffer, inet_ntoa(addrin.sin_addr));
		handle_players(&dead, buffer, recenv);
	}
	ft_printf(KILL"player %i is dead %s\n", my_index, RESET); //because we are zeroing it...
	return (NULL);
}

/* initialize the listening fd */
static int	host_player_init(t_player *player, char **env)
{
	
	int listfd;
	if ((listfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		return (perror(ERROR"socket failure"RESET), -1);

	struct sockaddr_in serveraddr;

	ft_memset(&serveraddr, 0, sizeof(struct sockaddr_in));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons ( MYPORT );
	serveraddr.sin_addr.s_addr = htonl( INADDR_ANY );

	if (bind(listfd, (struct sockaddr *)&serveraddr, sizeof(struct sockaddr_in)))
		return (perror(ERROR"bind failure"RESET), -1);

	if (listen(listfd, 10))
		return(perror(ERROR"listen failure"RESET), -1);
	
	ft_printf(LISTEN"I'm listening...%s\n", RESET);

	//player[0] 'host' data init
	ft_memmove(&player[0].ip, get_serv_ip(env), 15);
	ft_memmove(&player[0].name, "pippo host", 10);
	player[0].num = 0;
	return(listfd);
}

int server_routine( int argc, char *argv[], char *env[])
{
	(void)argc; (void)argv;

	int listfd;
	ft_printf("LOCAL_IP=%s, SERVER_IP=%s\n", get_locl_ip(env), get_serv_ip(env));

	t_player player[MAXPLAYERS];
	ft_memset(&player, 0, MAXPLAYERS * sizeof(t_player));
	
	//player init
	if ((listfd = host_player_init(&player[0], env)) < 0)
		return (1);

	player_specs(player[0]);

	pthread_t	tid;
	t_recenv	recenv;

	recenv.env = env;
	recenv.player = player;
	recenv.socket = listfd;
	recenv.index = 1; //next player index

	if (pthread_mutex_init(&recenv.player_mutex, NULL) != 0)
		perror(ERROR"mutex init failed"RESET);

	//reciever
	if (pthread_create(&tid, NULL, &server_reciever, &recenv) < 0)
		perror(ERROR"reciever launch failed"RESET);
	pthread_detach(tid);	//since if we died as server we don't need to wait the other listener

	minigame(0, &player[0]);

	//pthread_join(tid, NULL);
	//close( sendfd );
	return (0);
}
