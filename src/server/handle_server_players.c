/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_server_players.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 13:11:47 by totommi           #+#    #+#             */
/*   Updated: 2025/03/17 15:24:43 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "socket_is_closed.h"

int	register_player(int index, const char *player_data, int playersocket, t_player *player);
int	handle_server_players(char *dead, int length, const char *buffer, t_recenv *recenv);

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
	//pthread_mutex_lock(player->mutex);
	player->pos.x = ft_atoi(split[0]);
	player->pos.y = ft_atoi(split[1]);
	player->pos.z = ft_atoi(split[2]);
	//pthread_mutex_unlock(player->mutex);
	ft_freentf("2", split);
	split = ft_split(target, '_');
	if (split == NULL)
	{
		ft_printf(RED"bad target: null string\n"RESET);
		ft_freentf("2", split);
		return (1);
	}
	//pthread_mutex_lock(player->mutex);
	player->target.x = ft_atoi(split[0]);
	player->target.y = ft_atoi(split[1]);
	player->target.z = ft_atoi(split[2]);
	//pthread_mutex_unlock(player->mutex);
	ft_freentf("2", split);
	return (1);
}

static int	kill_player(t_player *player, char *dead)
{
	//pthread_mutex_t	*mutex;

	//mutex = player->mutex;
	//pthread_mutex_lock(mutex);
	close(player->socket);
	ft_memset(player, 0, sizeof(t_player));
	*dead = 1;
	//pthread_mutex_unlock(mutex);
	return (1);
}

//mutex needed (already locked) //name:ip
int	register_player(int index, const char *player_data, int playersocket, t_player *player)
{
	char	**split;

	ft_printf(PURPLE"registering new player...\n"RESET);
	split = ft_split(player_data, ':');
	if (split == NULL)
	{
		ft_printf(RED"bad ip: null string\n"RESET);
		return(0);
	}
	ft_memmove(player[index].name, split[0], 10);
	ft_memmove(player[index].ip, split[1], 15);
	player[index].socket = playersocket;
	player[index].num = index;
	ft_freentf("2", split);
	player[index].pos.x = 10;
	player[index].pos.y = 10;
	player[index].pos.z = 10;
	ft_printf(GREEN"NEW PLAYER REGISTERED:\n"RESET);
	player_specs(player[index]);
	return (1);
}

/* single second player handling. 
NOTE: player[0] is updated by minigame*/
int	handle_server_players(char *dead, int length, const char *buffer, t_recenv *recenv)
{
	char			**split;
	//char			lobby[45 * (MAXPLAYERS + 1)];
	int				i[2];
	
	split = ft_split(buffer, ':');
	if (split == NULL || recenv == NULL || !is_ip(split[1]))
		return (ft_freentf("2", split), 0);
	i[0] = 1;
	while (i[0] < MAXPLAYERS)
	{
		if (!ft_strncmp(split[1], recenv->player[i[0]].ip, ft_strlen(recenv->player[i[0]].ip))
			&& !ft_strncmp(split[0], recenv->player[i[0]].name, ft_strlen(recenv->player[i[0]].name)))
		{
			if (!ft_strncmp("died", split[2], 4))  //kill player
				kill_player(&recenv->player[i[0]], dead);
			else
				update_player(split[1], split[2], split[3], &recenv->player[i[0]]);
			break ;
		}
		i[0]++;
	}
	//get_lobby_stats(lobby, recenv->player);
	i[1] = 1;
	while (i[1] < MAXPLAYERS)
	{
		if (recenv->player[i[1]].ip[0] != '\0' && i[1] != i[0]
			&& (send(recenv->player[i[1]].socket, buffer, length, 0) < 0))
			perror(ERROR"send failure"RESET);
		i[1]++;
	}
	ft_printf(STATS);
	quick_stats(recenv->player);
	/* printf("%-15s at %d_%d_%d\n", recenv->player[0].ip, (int)recenv->player[0].pos.x, (int)recenv->player[0].pos.y, (int)recenv->player[0].pos.z);
	printf("%-15s at %d_%d_%d\n", recenv->player[1].ip, (int)recenv->player[1].pos.x, (int)recenv->player[1].pos.y, (int)recenv->player[1].pos.z); */
	ft_printf(RESET);
	return (ft_freentf("2", split), 1);
}
