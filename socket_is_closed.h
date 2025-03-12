/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket_is_closed.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: totommi <totommi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 22:51:31 by topiana-          #+#    #+#             */
/*   Updated: 2025/03/12 12:45:45 by totommi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_IS_CLOSED_H
# define SOCKET_IS_CLOSED_H

//std libraries
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h> 
# include <string.h>

//net libraries
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>

//mlx libraries
# include "mlx.h"
# include <X11/X.h>
# include <X11/keysym.h>

//libft
# include "libft.h"

# include <pthread.h>

# define PORT 8080
# define MAXLINE 1000 

# define MLX_WIN_X 500
# define MLX_WIN_Y 500

typedef struct s_point
{
	float	x;
	float	y;
	float	z;
}				t_point;

//player data struct
typedef struct s_player
{
	char				ip[15];
	char				name[10];
	struct sockaddr_in	sockaddr;
	int					socket;
	t_point				pos;
}				t_player;

//mlx img variables
typedef struct s_img
{
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}				t_img;

//mlx big struct
typedef struct s_mlx
{
	int				win_x;
	int				win_y;
	void			*mlx;
	void			*win;
	t_img			img;
	t_player		player;
}				t_mlx;		

void	*minigame(void	*arg);
int		ft_freentf(const char *s, ...);
void	*ft_realloc(void *ptr, size_t old_size, size_t new_size);

#endif