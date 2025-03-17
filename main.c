/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: totommi <totommi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 19:00:20 by topiana-          #+#    #+#             */
/*   Updated: 2025/03/17 10:37:59 by totommi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "socket_is_closed.h"

int	main(int argc, char *argv[], char *env[])
{
	t_player	player[MAXPLAYERS];
	int			index;
	
	(void)argc;
	ft_memset(&player, 0, MAXPLAYERS * sizeof(t_player));
	if (ft_strncmp("host", get_serv_ip(env), 4) == 0)
	{
		if (server_routine(player, &index, argv, env) != 0)
			return (1);
		usleep(1000); //segfaults without it, prolly it needs the init to be executed
		index = 0;
		minigame(&index, &player[0]);
	}
	else
	{
		if (client_routine(player, &index, argv, env) != 0)
			return (1);
		usleep(1000);
		index = 1;
		minigame(&index, &player[0]);
	}
	return (0);
}
