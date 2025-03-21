/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_manage.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 22:01:30 by totommi           #+#    #+#             */
/*   Updated: 2025/03/17 15:24:54 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "socket_is_closed.h"

/* moves the player at the index 'src' to the index 'dest'.
After that bzeros the player 'src'.
RETURNS: a ptr to the new player 'src' index (dest), NULL if player is null. */
t_player	*move_player(int src, int dest, t_player *player)
{
	if (player == NULL)
		return (NULL);
	ft_memmove(&player[dest], &player[src], sizeof(t_player));
	ft_memset(&player[src], 0, sizeof(t_player));
	player[dest].num = dest;
	return (&player[dest]);
}

/* RETURNS: the index of the first empty player slot. */
int	next_free_slot(t_player *player)
{
	int	i;

	if (player == NULL)
		return (-1);
	i = 0;
	while (i < MAXPLAYERS && player[i].ip[0] != '\0')
		i++;
	return (i);
}

/* compacts to the top all the players left.
ex:
[0]: host
[1]:
[2]: player2
[3]: 
[4]: player4
->
[0]: host
[1]: player2
[2]: player4
[3]:
[4]: */
void push_up(t_player *player)
{
	int	i[2];
	int	slot;
	
	i[0] = 0;
	while (i[0] < MAXPLAYERS)
	{
		slot = next_free_slot(player);
		i[1] = slot + 1;
		while (i[1] < MAXPLAYERS)
		{
			if (player[i[1]].ip[0] != '\0')
			{
				move_player(i[1], slot, player);
				break ;
			}
			i[1]++;
		}
		i[0]++;
	}
}

/* moves player[1] to player[0], then pushes up all the other players,
filling the gaps.
NOTE: the player poointer points to a stack allocated mem, no alloc or freezes. */
void	server_player_pack(t_player *player)
{
	int	i[2];
	
	if (player == NULL)
		return ;
	i[0] = 1;
	while (i[0] < MAXPLAYERS && player[i[0]].ip[0] != '\0') //picks the oldest player alive
		i[0]++;
	move_player(i[0], 0, player);
	push_up(player);
}

/* moves the player with ip equal to 'servip' to player[0].
then pushes all the other player up.
NOTE: the client will remain at player[1]. */
void	client_player_pack(char *servip, t_player *player)
{
	int	i;

	i = 0;
	while (i < MAXPLAYERS && ft_strncmp(servip, player[i].ip, ft_strlen(player[i].ip)))
		i++;
	move_player(i, 0, player);
	push_up(player);
}
