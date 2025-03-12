/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 19:00:20 by topiana-          #+#    #+#             */
/*   Updated: 2025/03/12 19:55:26 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "socket_is_closed.h"

int	main(int argc, char *argv[], char *env[])
{
	if (ft_strncmp("host", get_serv_ip(env), 4) == 0)
		server_routine(argc, argv, env);
	else
		client_routine(argc, argv, env);
	return (0);
}
