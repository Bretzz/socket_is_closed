/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minigame.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 23:13:08 by topiana-          #+#    #+#             */
/*   Updated: 2025/03/12 17:59:22 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "socket_is_closed.h"

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
	mlx->win = mlx_new_window(mlx->mlx, MLX_WIN_X, MLX_WIN_Y, mlx->player->ip);
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
	{
		if (mlx->player[i].ip[0] != '\0')
			my_pixel_put(mlx, mlx->player[i].pos.x, mlx->player[i].pos.y, mlx->player[i].pos.z, 0xFFFFFF);
		i++;
	}

	mlx_put_image_to_window(mlx->mlx, mlx->win, mlx->img.img, 0, 0);
	//ft_printf("ccc\n");
	mlx_destroy_image(mlx->mlx, mlx->img.img);
	return (1);
}

static int	send_pos(t_player player)
{
	char	pos[30];
	char	*coords[3];
	
	if (player.ip[0] == '\0' || !ft_strncmp("host", player.ip, 4))
		return (0);
	coords[0] = ft_itoa((int)player.pos.x);
	coords[1] = ft_itoa((int)player.pos.y);
	coords[2] = ft_itoa((int)player.pos.z);
	memset(pos, 0, 30);
	ft_strlcpy(pos, player.ip, 16);
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

static int	handle_heypress(int keysym, void *arg)
{
	t_mlx	*mlx;
	int		i;

	mlx = (t_mlx *)arg;
	if (keysym == XK_Escape || keysym == 53)
		clean_exit(mlx);
	else if (keysym == XK_Down || keysym == 125)
		mlx->player[0].pos.y += 10;
	else if (keysym == XK_Up || keysym == 126)
		mlx->player[0].pos.y -= 10;
	else if (keysym == XK_Left || keysym == 123)
		mlx->player[0].pos.x -= 10;
	else if (keysym == XK_Right || keysym == 124)
		mlx->player[0].pos.x += 10;
	else
		printf("Key Pressed: %i\n", keysym);
	put_board(mlx);
	i = 0;
	while (i < 2)
		send_pos(mlx->player[i++]);
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

void	*minigame(void	*arg)
{
	t_mlx	mlx;

	memset(&mlx, 0, sizeof(t_mlx));
	mlx.player = (t_player *)arg;
	if (juice_the_pc(&mlx))
		return (NULL);

	put_board(&mlx);
	
	mlx_hook(mlx.win, KeyPress, KeyPressMask, &handle_heypress, &mlx);
	mlx_hook(mlx.win, DestroyNotify, StructureNotifyMask, &clean_exit, &mlx);
	mlx_loop_hook(mlx.mlx, &update_frame, &mlx);
	mlx_loop(mlx.mlx);
	return (NULL);
}

/* int	main()
{
	t_player player;

	memset(&player, 0, sizeof(t_player));
	if (!minigame(&player))
		return (0);
	return (1);
} */
