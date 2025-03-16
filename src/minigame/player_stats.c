/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_stats.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: totommi <totommi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 12:56:41 by totommi           #+#    #+#             */
/*   Updated: 2025/03/16 12:59:10 by totommi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "socket_is_closed.h"

/* full print of the specified player stats */
void	player_specs(t_player player)
{
	ft_printf(CYAN);
	ft_printf("PLAYER_IP    : %s\n", player.ip);
	ft_printf("PLAYER_NAME  : %s\n", player.name);
	ft_printf("PLAYER_SOCKET: %d\n", player.socket);
	ft_printf("PLAYER_NUM   : %d\n", player.num);
	ft_printf("PLAYER_POS   : %d_%d_%d\n", (int)player.pos.x, (int)player.pos.y, (int)player.pos.z);
	ft_printf("PLAYER_TARGET: %d_%d_%d\n", (int)player.target.x, (int)player.target.y, (int)player.target.z);
	ft_printf(RESET);

}

/* quick show of player stats:
	name:ip at s_y_z */
void	quick_stats(t_player *allPlayers)
{
	int	i;

	i = 0;
	while(allPlayers[i].ip[0] != '\0')
	{
		printf("%-15s:%s at %d_%d_%d\n",
			allPlayers[i].name,
			allPlayers[i].ip,
			(int)allPlayers[i].pos.x,
			(int)allPlayers[i].pos.y,
			(int)allPlayers[i].pos.z);
		i++;
	}
}

/* checks weather the player pos is different from 0_0_0 */
int	player_alive(t_player player)
{
	if (player.pos.x > 0 || player.pos.y > 0 || player.pos.z > 0)
		return (1);
	return (0);
}
