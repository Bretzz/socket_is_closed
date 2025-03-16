/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_client_players.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: totommi <totommi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 13:04:43 by totommi           #+#    #+#             */
/*   Updated: 2025/03/16 13:08:21 by totommi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "socket_is_closed.h"

int	handle_client_players(const char *buffer, t_recenv *recenv);

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
	//size_t	len[2];
	int		i;
	
	split = ft_split(buffer, ':');
	if (split == NULL || split[1] == NULL || recenv == NULL)
		return (0);
	if (!ft_strncmp("host", split[1], 4)) //update host
	{
		if (!ft_strncmp("died", split[2], 4))  //kill host
		{
			kill_player(&recenv->player[0]);
			return (ft_freentf("2", split), -1);
			//host redirect procedure (follow the -1 return)
		}
		else if (!update_player(split[1], split[2], split[3], &recenv->player[0]))
			return (ft_freentf("2", split), 0);
		return (1);
	}
	if (!is_ip(split[1])
		|| (!ft_strncmp(recenv->player[1].ip, split[1], ft_strlen(recenv->player[1].ip))
			&& !ft_strncmp(recenv->player[1].name, split[0], ft_strlen(recenv->player[1].name))))
		return (ft_freentf("2", split), 0);
	ft_printf("someone that isn't me neither the host is sending stuff...\n");
	i = 2; //0 = host, 1 = self, >1 = extra players
	while (i < MAXPLAYERS)
	{
		if (recenv->player[i].ip[0] == '\0')
			break ;
		if (!ft_strncmp(split[1], recenv->player[i].ip, ft_strlen(recenv->player[i].ip))
			&& !ft_strncmp(split[0], recenv->player[i].name, ft_strlen(recenv->player[i].name))) //update other players
		{
			if (!ft_strncmp("died", split[2], 4))  //kill player
				kill_player(&recenv->player[i]);
			else if (!update_player(split[0], split[2], split[3], &recenv->player[i]))
				return (ft_freentf("2", split), 0);
			return (ft_freentf("2", split), 1);
		}
		i++;
	}
	//adding new player
	ft_memmove(&recenv->player[i].name, split[0], ft_strlen(split[0]));
	ft_memmove(&recenv->player[i].ip, split[1], ft_strlen(split[1]));
	recenv->player[i].num = i;
	ft_printf(GREEN"A new player appeared!\n"RESET);
	if (!update_player(split[1], split[2], split[3], &recenv->player[i]))
		return (ft_freentf("2", split), 0);
	ft_freentf("2", split);
	return (1);
}

/* moves the server's player 'player[0]'. 
NOTE: the 'player[1]' is moved by the minigame. */
int	handle_client_players(const char *buffer, t_recenv *recenv)
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
	quick_stats(recenv->player);
	/* printf("%-15s at %d_%d_%d\n", recenv->player[0].ip, (int)recenv->player[0].pos.x, (int)recenv->player[0].pos.y, (int)recenv->player[0].pos.z);
	printf("%-15s at %d_%d_%d\n", recenv->player[1].ip, (int)recenv->player[1].pos.x, (int)recenv->player[1].pos.y, (int)recenv->player[1].pos.z); */
	ft_printf(RESET);
	ft_freentf("2", split);
	return (1);
}
