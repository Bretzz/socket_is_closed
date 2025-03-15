/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket_is_closed.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: totommi <totommi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 22:51:31 by topiana-          #+#    #+#             */
/*   Updated: 2025/03/15 00:59:21 by totommi          ###   ########.fr       */
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
//# include <sys/types.h>
# include <netdb.h>

//mlx libraries
# include "mlx.h"
# include <X11/X.h>
# include <X11/keysym.h>
# include <math.h>

//libft
# include "libft.h"

# include <pthread.h>

# define MYPORT 42042
# define MAXLINE 1024 
# define MAXPLAYERS 3

# define MLX_WIN_X 500
# define MLX_WIN_Y 500

//colors
# define BOLD "\033[1m"
# define BLACK "\033[0;30m"
# define RED "\033[0;31m"
# define GREEN "\033[0;32m"
# define YELLOW "\033[0;33m"
# define BLUE "\033[0;34m"
# define PURPLE "\033[0;35m"
# define CYAN "\033[0;36m"
# define WHITE "\033[0;37m"
# define RESET "\033[0m"

# define ERROR "\033[0;41m"
# define STATS "\033[0;34m"
# define KILL "\033[0;101m"
# define LISTEN "\033[0;101m"
# define HOST "\033[0;43m"

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
	t_point				target;
	int					num;
	pthread_mutex_t		*mutex;
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
	t_player		*player;
	int				index;
}				t_mlx;		

typedef	struct s_recenv
{
	int				socket;
	char			**env;
	unsigned int	index;
	t_player		*player;
	pthread_mutex_t	player_mutex;
}				t_recenv;

//client

int client_routine( int argc, char *argv[], char *env[]);

//server

int server_routine( int argc, char *argv[], char *env[]);

//minigame

int				minigame(int my_pos, t_player *player);

int				player_alive(t_player player);
void			player_specs(t_player player);
char 			*get_pos(char *buffer, t_point my_pos, t_point my_target, const char *my_ip);

int 			server_duty(void);

//awesome functions

char	*get_locl_ip(char **env);
char	*get_serv_ip(char **env);

int		is_ip(const char *s);

//stolen cool stuff

int				ft_freentf(const char *s, ...);
void			*ft_realloc(void *ptr, size_t old_size, size_t new_size);

//convert stuff

unsigned int	ip_to_uns(const char *ip);

int				ft_atohexi(const char *nptr);
char			*ft_itohex_buff(char *buff, unsigned int nb);

#endif