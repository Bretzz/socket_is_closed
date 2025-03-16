/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_inet.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: totommi <totommi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 12:51:37 by totommi           #+#    #+#             */
/*   Updated: 2025/03/16 13:18:49 by totommi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "socket_is_closed.h"

char	*get_pos(char *buffer, t_player me);
char	*get_death(char *buffer, t_player me);
char	*get_lobby_stats(char *buffer, t_player *player);
void	send_all(t_mlx *mlx, const char *msg, size_t msg_size);

/* buffer is a stack allocated mem,
fills the buffer with get_pos() calls on the various players. */
char	*get_lobby_stats(char *buffer, t_player *player)
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
			get_pos(minibuffer, player[i]);
		ft_strlcat(buffer, minibuffer, ft_strlen(buffer) + sizeof(minibuffer) + 1);
		if (i < MAXPLAYERS - 1 && player[i + 1].ip[0] != '\0')
			ft_strlcat(buffer, "\n", ft_strlen(buffer) + 2);
		i++;
	}
	return (buffer);
}

//buffer is a stack allocated mem, buffsize needed 35
char *get_death(char *buffer, t_player me)
{
	if (buffer == NULL)
		return (NULL);
	ft_memset(buffer, 0, sizeof(buffer));
	ft_strlcpy(buffer, me.name, ft_strlen(me.name) + 1);
	ft_strlcat(buffer, ":", strlen(buffer) + 2);
	ft_strlcat(buffer, me.ip, ft_strlen(buffer) + 16);
	ft_strlcat(buffer, ":", strlen(buffer) + 2);
	ft_strlcat(buffer, "died", strlen(buffer) + 5);
	return (buffer);
}

//buffer is a stack allocated mem, buffsize needed 55
char *get_pos(char *buffer, t_player me)
{
	char	*coords[6];
	
	if (buffer == NULL)
		return (NULL);
	ft_memset(buffer, 0, sizeof(buffer));
	coords[0] = ft_itoa((int)me.pos.x);
	coords[1] = ft_itoa((int)me.pos.y);
	coords[2] = ft_itoa((int)me.pos.z);
	coords[3] = ft_itoa((int)me.target.x);
	coords[4] = ft_itoa((int)me.target.y);
	coords[5] = ft_itoa((int)me.target.z);
	ft_strlcpy(buffer, me.name, ft_strlen(me.name) + 1);
	ft_strlcat(buffer, ":", ft_strlen(buffer) + 2);
	ft_strlcat(buffer, me.ip, ft_strlen(buffer) +  16);
	ft_strlcat(buffer, ":", ft_strlen(buffer) + 2);
	ft_strlcat(buffer, coords[0], ft_strlen(buffer) + ft_strlen(coords[0]) + 1);
	ft_strlcat(buffer, "_", ft_strlen(buffer) + 2);
	ft_strlcat(buffer, coords[1], ft_strlen(buffer) + ft_strlen(coords[1]) + 1);
	ft_strlcat(buffer, "_", ft_strlen(buffer) + 2);
	ft_strlcat(buffer, coords[2], ft_strlen(buffer) + ft_strlen(coords[2]) + 1);
	if (me.target.x || me.target.y || me.target.z)
	{
		ft_strlcat(buffer, ":", ft_strlen(buffer) + 2);
		ft_strlcat(buffer, coords[3], ft_strlen(buffer) + ft_strlen(coords[3]) + 1);
		ft_strlcat(buffer, "_", ft_strlen(buffer) + 2);
		ft_strlcat(buffer, coords[4], ft_strlen(buffer) + ft_strlen(coords[4]) + 1);
		ft_strlcat(buffer, "_", ft_strlen(buffer) + 2);
		ft_strlcat(buffer, coords[5], ft_strlen(buffer) + ft_strlen(coords[5]) + 1);
	}
	free(coords[0]); free(coords[1]); free(coords[2]); free(coords[3]); free(coords[4]); free(coords[5]);
	return (buffer);
}

void	send_all(t_mlx *mlx, const char *msg, size_t msg_size)
{
	int	i;

	ft_printf(GREEN"sending: %s\n"RESET, msg);
	if (mlx->index != 0) //sendto host (ClientUDP)
	{
		if (send(mlx->player[0].socket, msg, msg_size, 0) < 0 )
			perror(ERROR"sendto failed"RESET);
		return ;
	}
	i = 1;
	while (i < MAXPLAYERS) //sendto players (ServerUDP)
	{
		//ft_printf("sending to player: %s, socket: %i\n", mlx->player[i].ip, mlx->player[i].socket);
		if (mlx->player[i].ip[0] != '\0')
		{			
			if (send(mlx->player[i].socket, msg, msg_size, 0) < 0 )
				perror(ERROR"sendto failed"RESET);
		}
		i++;
	}
}
