/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   put_stuff.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 15:31:33 by topiana-          #+#    #+#             */
/*   Updated: 2025/03/17 15:37:47 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "socket_is_closed.h"

void	my_pixel_put(void *my_struct, int x, int y, float z, unsigned int color);
int		put_line(t_mlx *mlx, t_point p, t_point t, t_point my_pos, unsigned int color);

void	my_pixel_put(void *my_struct, int x, int y, float z, unsigned int color)
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
int	put_line(t_mlx *mlx, t_point p, t_point t, t_point my_pos, unsigned int color)
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
