/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 15:35:27 by topiana-          #+#    #+#             */
/*   Updated: 2025/03/17 15:36:08 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "socket_is_closed.h"

int	handle_heypress(int keysym, t_mlx *mlx);
int	handle_mouse(int keysym, int x, int y, t_mlx *mlx);

int	handle_mouse(int keysym, int x, int y, t_mlx *mlx)
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

/* static int	handle_UpDw_press(int keysym, void *arg)
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
} */

int	handle_heypress(int keysym, t_mlx *mlx)
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
