/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minigame.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 23:13:08 by topiana-          #+#    #+#             */
/*   Updated: 2025/03/12 00:16:39 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "socket_is_closed.h"

static int clean_exit(void *arg)
{
	t_mlx	*mlx;

	mlx = (t_mlx *)arg;
	mlx_destroy_window(mlx->mlx, mlx->win);
	mlx_destroy_display(mlx->mlx);
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
	mlx->win = mlx_new_window(mlx->mlx, MLX_WIN_X, MLX_WIN_Y, "minigame");
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
	printf("putting (%i, %i, %f), with color %x\n", x, y, z, color);
	// If the point is off-screen, do not draw it
	if (x < 0 || y < 0 || /* z > 0 || */ x >= mlx->win_x || y >= mlx->win_y)
		return ;
	dst = mlx->img.addr + (y * mlx->img.line_length + x * (mlx->img.bits_per_pixel / sizeof(int *)));
	*(unsigned int *)dst = color;
}

static int	put_board(t_mlx *mlx)
{
	mlx->img.img = mlx_new_image(mlx->mlx, mlx->win_x, mlx->win_y);
	mlx->img.addr = mlx_get_data_addr(mlx->img.img, &mlx->img
			.bits_per_pixel, &mlx->img.line_length, &mlx->img.endian);
	if (!mlx->img.img || !mlx->img.addr)
		return (0);

	my_pixel_put(mlx, mlx->player.pos.x, mlx->player.pos.y, mlx->player.pos.z, 0xFFFFFF);

	mlx_put_image_to_window(mlx->mlx, mlx->win, mlx->img.img, 0, 0);
	//ft_printf("ccc\n");
	mlx_destroy_image(mlx->mlx, mlx->img.img);
	return (1);
}

static int	send_pos(t_player player)
{
	char	pos[15];
	char	*coords[3];
	
	coords[0] = ft_itoa((int)player.pos.x);
	coords[1] = ft_itoa((int)player.pos.x);
	coords[2] = ft_itoa((int)player.pos.x);
	memset(pos, 0, 15);
	ft_strlcat(pos, coords[0], strlen(coords[0] + 1));
	ft_strlcat(pos, ":", strlen(pos) + 2);
	ft_strlcat(pos, coords[0], strlen(pos) + strlen(coords[0]) + 1);
	ft_strlcat(pos, ":", strlen(pos) + 2);
	ft_strlcat(pos, coords[0], strlen(pos) + strlen(coords[0]) + 1);
	printf("socket=%i\n", player.socket);
	if (sendto( player.socket, pos, 15, 0, (struct sockaddr *)&player.sockaddr, sizeof(struct sockaddr_in)) < 0 )
		perror( "sendto failed" );
	else
		printf( "pos sent\n" );
	free(coords[0]); free(coords[1]); free(coords[2]);
	return (0);
}

static int	handle_heypress(int keysym, void *arg)
{
	t_mlx	*mlx;

	mlx = (t_mlx *)arg;
	if (keysym == XK_Escape)
		clean_exit(mlx);
	else if (keysym == XK_Down)
		mlx->player.pos.y += 10;
	else if (keysym == XK_Up)
		mlx->player.pos.y -= 10;
	else if (keysym == XK_Left)
		mlx->player.pos.x -= 10;
	else if (keysym == XK_Right)
		mlx->player.pos.x += 10;
	else
		printf("Key Pressed: %i\n", keysym);
	put_board(mlx);
	send_pos(mlx->player);
	return (0);
}

void	*minigame(void	*arg)
{
	t_mlx	mlx;

	memset(&mlx, 0, sizeof(t_mlx));
	mlx.player = *(t_player *)arg;
	if (juice_the_pc(&mlx))
		return (NULL);

	put_board(&mlx);
	
	mlx_hook(mlx.win, KeyPress, KeyPressMask, &handle_heypress, &mlx);
	mlx_hook(mlx.win, DestroyNotify, StructureNotifyMask, &clean_exit, &mlx);
	mlx_loop(mlx.mlx);
	return (NULL);
}

/* int	main()
{
	if (!minigame(NULL))
		return (0);
	return (1);
} */
