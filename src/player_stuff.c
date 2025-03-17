/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_stuff.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: totommi <totommi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 22:01:30 by totommi           #+#    #+#             */
/*   Updated: 2025/03/16 22:41:45 by totommi          ###   ########.fr       */
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
	memmove(&player[dest], &player[src], sizeof(t_player));
	memset(&player[src], 0, sizeof(t_player));
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
