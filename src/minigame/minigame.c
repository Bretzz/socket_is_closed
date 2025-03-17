/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minigame.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: totommi <totommi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 23:13:08 by topiana-          #+#    #+#             */
/*   Updated: 2025/03/17 01:22:03 by totommi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "socket_is_closed.h"

static int clean_exit(t_mlx *mlx)
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

static void	my_pixel_put(void *my_struct, int x, int y, float z, unsigned int color)
{
	t_mlx	*mlx;
	char	*dst;
	
	(void)z;
	if (!my_struct)
		return ;
	mlx = (t_mlx *)my_struct;
	//ft_printf("putting (%i, %i, %f), with color %x\n", x, y, z, color);
	// If the point is off-screen, do not draw it
	if (x < 0 || y < 0 || /* z > 0 || */ x >= mlx->win_x || y >= mlx->win_y)
		return ;
	dst = mlx->img.addr + (y * mlx->img.line_length + x * (mlx->img.bits_per_pixel / sizeof(int *)));
	*(unsigned int *)dst = color;
}

/* returns 0 if we got killed. */
/* static  */int	put_line(t_mlx *mlx, t_point p, t_point t, t_point my_pos, unsigned int color)
{
	int dx =  fabsf (t.x - p.x), sx = p.x < t.x ? 1 : -1;
	int dy = -fabsf (t.y - p.y), sy = p.y < t.y ? 1 : -1; 
	int err = dx + dy, e2; /* error value e_xy */
 
	for (;;){  /* loop */
		e2 = 2 * err;
		if (e2 >= dy) { err += dy; p.x += sx; } /* e_xy+e_x > 0 */
		if (e2 <= dx) { err += dx; p.y += sy; } /* e_xy+e_y < 0 */

		//my stuff (death check)
		my_pixel_put(mlx, p.x, p.y, 0, color);
		if (p.x == my_pos.x && p.y == my_pos.y) return (0);

		if (p.x == t.x && p.y == t.y) break;
	}
	return (1);
}

static int	handle_player(t_player *player, t_mlx *mlx)
{
	static int	lineframes[2];
	
	/* if (*mlx->index != 0)
		ft_printf("player '%s' putting...\n", player->ip); */
	if (player->ip[0] != '\0')
	{
		my_pixel_put(mlx, player->pos.x, player->pos.y, player->pos.z, 0xFFFFFF);
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
				0xFFFFFF))
				clean_exit(mlx);
			lineframes[player->num]++;
			//ft_memset(&player->target, 0, sizeof(t_point));
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
	while (i < MAXPLAYERS)
		handle_player(&mlx->player[i++], mlx);

	mlx_put_image_to_window(mlx->mlx, mlx->win, mlx->img.img, 0, 0);
	//ft_printf("ccc\n");
	mlx_destroy_image(mlx->mlx, mlx->img.img);
	//ft_printf("the board is put\n");
	return (1);
}

static int	handle_mouse(int keysym, int x, int y, t_mlx *mlx)
{
	char	buffer[45];

	if (keysym == 1) // keysym == 1
	{
		mlx->player[*mlx->index].target.x = x;
		mlx->player[*mlx->index].target.y = y;
		ft_printf("PIU-PIU!!!\n");
		//player_specs(mlx->player[*mlx->index]);
		get_pos(buffer, mlx->player[*mlx->index]);
		send_all(mlx, buffer, 45);
	}
	else
		ft_printf("Mouse thing N. %d\n", keysym);
	return (0);
}

static int	handle_UpDw_press(int keysym, void *arg)
{
	t_mlx	*mlx;

	mlx = (t_mlx *)arg;
	if (keysym == XK_Down || keysym == XK_s || keysym == 125 || keysym == 1)
		mlx->player[*mlx->index].pos.y += 10;
	else if (keysym == XK_Up || keysym == XK_w || keysym == 126 || keysym == 13)
		mlx->player[*mlx->index].pos.y -= 10;
	return (0);
}

static int	handle_LxRx_press(int keysym, void *arg)
{
	t_mlx	*mlx;

	mlx = (t_mlx *)arg;
	if (keysym == XK_Left || keysym == XK_a || keysym == 123 || keysym == 0)
		mlx->player[*mlx->index].pos.x -= 10;
	else if (keysym == XK_Right || keysym == XK_d || keysym == 124 || keysym == 2)
		mlx->player[*mlx->index].pos.x += 10;
	return (0);
}

static int	handle_heypress(int keysym, t_mlx *mlx)
{
	char	buffer[45];
	
	if (keysym == XK_Escape || keysym == 53)
		clean_exit(mlx);
	else if (keysym == XK_KP_Space || keysym == 49)
	{
		buffer[0] = 0;
		while (buffer[0] < MAXPLAYERS)
		{
			ft_printf("== = = == == =\n");
			player_specs(mlx->player[(int)buffer[0]]);
			buffer[0]++;
		}
	}
	else if (keysym == XK_Down || keysym == XK_s || keysym == 125 || keysym == 1)
		mlx->player[*mlx->index].pos.y += 10;
	else if (keysym == XK_Up || keysym == XK_w || keysym == 126 || keysym == 13)
		mlx->player[*mlx->index].pos.y -= 10;
	else if (keysym == XK_Left || keysym == XK_a || keysym == 123 || keysym == 0)
		mlx->player[*mlx->index].pos.x -= 10;
	else if (keysym == XK_Right || keysym == XK_d || keysym == 124 || keysym == 2)
		mlx->player[*mlx->index].pos.x += 10;
	else if (keysym == XK_Delete || keysym == 51)
	{
		clean_exit(mlx);
		//return (0);
	}
	else
		printf("Key Pressed: %i\n", keysym);
	//player_specs(mlx->player[0]); player_specs(mlx->player[1]);
	get_pos(buffer, mlx->player[*mlx->index]);
	send_all(mlx, buffer, 45);
	return (0);
}

static int	update_frame(t_mlx *mlx)
{
	static int	frame;

	if (frame % 60 == 0)
		put_board(mlx);
	usleep(1000);
	return (0);
}

/* static  */int	handle_movement(t_mlx *mlx)
{
	//pthread_t tid[2];
	
	(void)handle_heypress; (void)handle_UpDw_press; (void)handle_LxRx_press;
	//mlx_do_key_autorepeatoff(mlx->mlx);
	//pthread_create()
	//mlx_key_hook(mlx->win, &handle_UpDw_press, mlx);
	//mlx_key_hook(mlx->win, &handle_LxRx_press, mlx);
	mlx_hook(mlx->win, KeyPress, KeyPressMask, &handle_heypress, mlx);
	//mlx_hook(mlx.win, KeyPress, KeyPressMask, &handle_UpDw_press, &mlx); 
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
