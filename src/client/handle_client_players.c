/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_client_players.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: totommi <totommi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 13:04:43 by totommi           #+#    #+#             */
/*   Updated: 2025/03/17 11:57:23 by totommi          ###   ########.fr       */
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
	ft_printf(KILL"killed %s%s\n", player->name, RESET);
	ft_memset(player, 0, sizeof(t_player));
	return (1);
}

/* NOTE: could be that the next message sent after 'host:died' isn't 'new-host:ip'.
NOOT TRUE IF WE GO WITH THE MULTI LINE TECH.
for now we assume the next message will be a 'new-host:ip' assignment.
NOTE: socket will be closed after the return by the reciever,
and the reciever thread will die cuz we went trough with the -1 return. */
static int	new_host_check(char *buffer, t_recenv *recenv)
{
	char	**split;

	split = ft_split(buffer, ':');
	if (split == NULL)
		return (0);
	if (!ft_strncmp("new-host", split[0], 8))
	{
		if (!ft_strncmp(recenv->player[1].ip, split[2], ft_strlen(split[2]))
			&& !ft_strncmp(recenv->player[1].name, split[1], ft_strlen(split[1])))
		{
			make_me_host(recenv->env);
			server_player_pack(recenv->player);
			*recenv->id = 0; //index update for minigame
			if (server_routine(recenv->player, 0, NULL, recenv->env) != 0)
				return (ft_freentf("2", split), -1);
			return (ft_freentf("2", split), 3);
		}
		//client redirect stuff
		make_him_host(split[2], recenv->env);
		usleep(1000); //wait for host init
		client_routine(recenv->player, recenv->id, NULL, recenv->env);
		return (ft_freentf("2", split), 2);
			
	}
	return (ft_freentf("2", split), 1);
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
		{
			if (new_host_check(split[i], recenv) < 0)
				exit(EXIT_FAILURE); //clean up the mess plss
			return (ft_freentf("2", split), -1);
		}
	}
	ft_printf(STATS);
	quick_stats(recenv->player);
	ft_printf(RESET);
	ft_freentf("2", split);
	return (1);
}

/* moves the player with ip equal to 'servip' to player[0].
then pushes all the other player up.
NOTE: the client will remain at player[1]. */
void	client_player_pack(char *servip, t_player *player)
{
	int	i;
	int	len;

	len = ft_strlen(servip);
	i = 0;
	while (i < MAXPLAYERS && ft_strncmp(servip, player[i].ip, ))
}
