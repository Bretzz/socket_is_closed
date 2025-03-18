/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minigame.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 23:13:08 by topiana-          #+#    #+#             */
/*   Updated: 2025/03/17 15:52:51 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "socket_is_closed.h"

int 	clean_exit(t_mlx *mlx);
void	send_all(t_mlx *mlx, const char *msg, size_t msg_size);

int clean_exit(t_mlx *mlx)
{
	char	buffer[80];
	char	minibuffer[35];
	
	get_death(buffer, mlx->player[*mlx->index]);
	if (*mlx->index == 0)
	{
		get_host_update(minibuffer, mlx->player);
		ft_strlcat(buffer, "\n", ft_strlen(buffer) + 2);
		ft_strlcat(buffer, minibuffer, ft_strlen(buffer) + ft_strlen(minibuffer) + 1);
	}
	send_all(mlx, buffer, sizeof(buffer));
	mlx_destroy_window(mlx->mlx, mlx->win);
	//mlx_destroy_display(mlx->mlx);
	free(mlx->mlx);
	exit(EXIT_SUCCESS);
}
/*set up the pc-side of the t_data (mlx) struct
RETURNS: 0 all good, 1 error*/
static int	juice_the_pc(t_mlx *mlx)
{
	mlx->mlx = mlx_init();
	if (!mlx->mlx)
		return (1);
	mlx->win_x = MLX_WIN_X;
	mlx->win_y = MLX_WIN_Y;
	mlx->win = mlx_new_window(mlx->mlx, MLX_WIN_X, MLX_WIN_Y, mlx->player[*mlx->index].name);
	if (!mlx->win)
	{
		free(mlx->mlx);
		return (1);
	}
	return (0);
}

void	send_all(t_mlx *mlx, const char *msg, size_t msg_size)
{
	int	i;

	ft_printf(GREEN"sending: %s\n"RESET, msg);
	if (*mlx->index != 0) //sendto host (ClientUDP)
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

/* puts all the player info, position and target (with line for shot).
if we got hit by a line (even ours) we exit. */
static int	handle_player(t_player *player, t_mlx *mlx)
{
	static int	lineframes[2];
	unsigned int	color;

	if (!ft_strncmp("host", player->ip, 5))
		color = 0xFF0000;
	else
		color = 0xFFFFFF;
	
	if (player->ip[0] != '\0')
	{
		my_pixel_put(mlx, player->pos.x, player->pos.y, player->pos.z, color);
		if (player->target.x || player->target.y)
		{
			if (lineframes[player->num] == 10)
			{
				ft_memset(&player->target, 0, sizeof(t_point));
				lineframes[player->num] = 0;
				return (1);
			}
			if (!put_line(mlx,
				player->pos,
				player->target,
				mlx->player[*mlx->index].pos,
				color))
				clean_exit(mlx);
			lineframes[player->num]++;
		}
		return (1);
	}
	return (0);
}

static int	put_board(t_mlx *mlx)
{
	int	i;
	
	mlx->img.img = mlx_new_image(mlx->mlx, mlx->win_x, mlx->win_y);
	mlx->img.addr = mlx_get_data_addr(mlx->img.img, &mlx->img
			.bits_per_pixel, &mlx->img.line_length, &mlx->img.endian);
	if (!mlx->img.img || !mlx->img.addr)
		return (0);

	i = 0;
	while (i < MAXPLAYERS)
		handle_player(&mlx->player[i++], mlx);

	mlx_put_image_to_window(mlx->mlx, mlx->win, mlx->img.img, 0, 0);
	//ft_printf("ccc\n");
	mlx_destroy_image(mlx->mlx, mlx->img.img);
	//ft_printf("the board is put\n");
	return (1);
}

static int	update_frame(t_mlx *mlx)
{
	static int	frame;

	if (frame % 60 == 0)
		put_board(mlx);
	usleep(1000);
	return (0);
}

int	minigame(int *index, t_player *player)
{
	t_mlx	mlx;

	ft_memset(&mlx, 0, sizeof(t_mlx));
	mlx.player = player;
	mlx.index = index;
	if (juice_the_pc(&mlx))
		return (1);
	
	//ft_printf("pc juiced\n");
	put_board(&mlx);
	//ft_printf("board put\n");

	//game mechanics
	mlx_mouse_hook(mlx.win, &handle_mouse, &mlx);
	mlx_key_hook(mlx.win, &handle_heypress, &mlx);
	mlx_do_key_autorepeaton(mlx.mlx);
	
	//window management
	mlx_hook(mlx.win, DestroyNotify, StructureNotifyMask, &clean_exit, &mlx);
	mlx_loop_hook(mlx.mlx, &update_frame, &mlx);
	mlx_loop(mlx.mlx);
	return (0);
}

/* int	main()
{
	t_player player;

	ft_memset(&player, 0, sizeof(t_player));
	if (!minigame(&player))
		return (0);
	return (1);
} */
