/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minigame.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 23:13:08 by topiana-          #+#    #+#             */
/*   Updated: 2025/03/13 12:15:24 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "socket_is_closed.h"

void	player_specs(t_player player)
{
	printf("player ip    : %s\n", player.ip);
	printf("player name  : %s\n", player.name);
	printf("player socket: %d\n", player.socket);
	printf("player num   : %d\n", player.num);
	printf("player pos   : %d-%d-%d\n", (int)player.pos.x, (int)player.pos.y, (int)player.pos.z);
	printf("player target: %d-%d-%d\n", (int)player.target.x, (int)player.target.y, (int)player.target.z);

}

int	player_alive(t_player player)
{
	if (player.pos.x > 0 || player.pos.y > 0 || player.pos.z > 0)
		return (1);
	return (0);
}

static int clean_exit(void *arg)
{
	t_mlx	*mlx;

	mlx = (t_mlx *)arg;
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
	mlx->win = mlx_new_window(mlx->mlx, MLX_WIN_X, MLX_WIN_Y, mlx->player[mlx->index].ip);
	if (!mlx->win)
	{
		free(mlx->mlx);
		return (1);
	}
	return (0);
}

static void	my_pixel_put(void *my_struct, int x, int y, float z, unsigned int color)
{
	t_mlx	*mlx;
	char	*dst;
	
	(void)z;
	if (!my_struct)
		return ;
	mlx = (t_mlx *)my_struct;
	//printf("putting (%i, %i, %f), with color %x\n", x, y, z, color);
	// If the point is off-screen, do not draw it
	if (x < 0 || y < 0 || /* z > 0 || */ x >= mlx->win_x || y >= mlx->win_y)
		return ;
	dst = mlx->img.addr + (y * mlx->img.line_length + x * (mlx->img.bits_per_pixel / sizeof(int *)));
	*(unsigned int *)dst = color;
}


static int	put_line(t_mlx *mlx, t_point p, t_point t, unsigned int color)
{
	int dx =  fabsf (t.x - p.x), sx = p.x < t.x ? 1 : -1;
	int dy = -fabsf (t.y - p.y), sy = p.y < t.y ? 1 : -1; 
	int err = dx + dy, e2; /* error value e_xy */
 
	for (;;){  /* loop */
		my_pixel_put(mlx, p.x, p.y, 0, color);
		if (p.x == t.x && p.y == t.y) break;
		e2 = 2 * err;
		if (e2 >= dy) { err += dy; p.x += sx; } /* e_xy+e_x > 0 */
		if (e2 <= dx) { err += dx; p.y += sy; } /* e_xy+e_y < 0 */
	}
	return (1);
}

static int	handle_player(t_player *player, t_mlx *mlx)
{
	static int lineframes[2];
	
	if (player->ip[0] != '\0')
	{
		my_pixel_put(mlx, player->pos.x, player->pos.y, player->pos.z, 0xFFFFFF);
		if (player->target.x || player->target.y)
		{
			//player_specs(*player);
			if (lineframes[player->num] == 10)
			{
				memset(&player->target, 0, sizeof(t_point));
				lineframes[player->num] = 0;
				return (1);
			}
			put_line(mlx, player->pos, player->target, 0xFFFFFF);
			lineframes[player->num]++;
			/* player->target.x = 0;
			player->target.y = 0; */
			//memset(&player->target, 0, sizeof(t_point));
			//player_specs(*player);
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
	while (i < 2)
		handle_player(&mlx->player[i++], mlx);

	mlx_put_image_to_window(mlx->mlx, mlx->win, mlx->img.img, 0, 0);
	//ft_printf("ccc\n");
	mlx_destroy_image(mlx->mlx, mlx->img.img);
	//printf("the board is put\n");
	return (1);
}

static int	send_pos(t_point my_pos, const char *my_ip, t_player player)
{
	char	pos[30];
	char	*coords[3];
	
	if (player.ip[0] == '\0')
		return (0);
	coords[0] = ft_itoa((int)my_pos.x);
	coords[1] = ft_itoa((int)my_pos.y);
	coords[2] = ft_itoa((int)my_pos.z);
	memset(pos, 0, 30);
	ft_strlcpy(pos, my_ip, 16);
	ft_strlcat(pos, ":", strlen(pos) + 2);
	ft_strlcat(pos, coords[0], strlen(pos) + strlen(coords[0]) + 1);
	ft_strlcat(pos, "-", strlen(pos) + 2);
	ft_strlcat(pos, coords[1], strlen(pos) + strlen(coords[1]) + 1);
	ft_strlcat(pos, "-", strlen(pos) + 2);
	ft_strlcat(pos, coords[2], strlen(pos) + strlen(coords[2]) + 1);
	if (sendto( player.socket, pos, 30, 0, (struct sockaddr *)&player.sockaddr, sizeof(struct sockaddr_in)) < 0 )
		perror( "sendto failed" );
	free(coords[0]); free(coords[1]); free(coords[2]);
	return (0);
}

static int	handle_mouse(int keysym, int x, int y, t_mlx *mlx)
{
	if (keysym == 1) // keysym == 1
	{
		mlx->player[mlx->index].target.x = x;
		mlx->player[mlx->index].target.y = y;
		printf("RIGHT CLICK!!!\n");
	}
	else
		printf("Mouse thing N. %d\n", keysym);
	return (0);
}

static int	handle_heypress(int keysym, void *arg)
{
	t_mlx	*mlx;
	int		i;

	mlx = (t_mlx *)arg;
	if (keysym == XK_Escape || keysym == 53)
		clean_exit(mlx);
	else if (keysym == XK_Down || keysym == XK_s || keysym == 125)
		mlx->player[mlx->index].pos.y += 10;
	else if (keysym == XK_Up || keysym == XK_w || keysym == 126)
		mlx->player[mlx->index].pos.y -= 10;
	else if (keysym == XK_Left || keysym == XK_a || keysym == 123)
		mlx->player[mlx->index].pos.x -= 10;
	else if (keysym == XK_Right || keysym == XK_d || keysym == 124)
		mlx->player[mlx->index].pos.x += 10;
	else
		printf("Key Pressed: %i\n", keysym);
	put_board(mlx);
	//player_specs(mlx->player[0]); player_specs(mlx->player[1]);
	i = 0;
	while (i < 2)
	{
		if (i != mlx->index)
			send_pos(mlx->player[mlx->index].pos, mlx->player[mlx->index].ip, mlx->player[i]);
		i++;
	}
	return (0);
}

static int	update_frame(t_mlx *mlx)
{
	static int	frame;

	if (frame % 60 == 0)
	{
		put_board(mlx);
	}
	usleep(1000);
	return (0);
}

int	minigame(int my_pos, t_player *player)
{
	t_mlx	mlx;

	memset(&mlx, 0, sizeof(t_mlx));
	mlx.player = player;
	mlx.index = my_pos;
	if (juice_the_pc(&mlx))
		return (1);

	put_board(&mlx);
	
	mlx_hook(mlx.win, KeyPress, KeyPressMask, &handle_heypress, &mlx);
	mlx_hook(mlx.win, DestroyNotify, StructureNotifyMask, &clean_exit, &mlx);
	mlx_mouse_hook(mlx.win, &handle_mouse, &mlx);
	mlx_loop_hook(mlx.mlx, &update_frame, &mlx);
	mlx_loop(mlx.mlx);
	return (0);
}

/* int	main()
{
	t_player player;

	memset(&player, 0, sizeof(t_player));
	if (!minigame(&player))
		return (0);
	return (1);
} */
