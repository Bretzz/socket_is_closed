/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inet_stuff.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: totommi <totommi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 15:35:22 by topiana-          #+#    #+#             */
/*   Updated: 2025/03/13 17:10:00 by totommi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "socket_is_closed.h"

int	is_ip(const char *s)
{
	int	i;
	
	if (s == NULL)
		return (0);
	i = 0;
	while (i < 3)
	{
		while (*s >= '0' && *s <= '9')
			s++;
		if (*(s++) != '.')
			return (0);
		i++;
	}
	while (*s >= '0' && *s <= '9')
		s++;
	if (*s != '\0')
		return (0);
	return (1);
}

/* returns a pointer to the value of the IP var in the env */
char	*get_locl_ip(char **env)
{
	int	i;

	if (env == NULL)
		return (NULL);
	i = 0;
	while (env[i] != NULL)
	{
		if (!ft_strncmp("LOCAL_IP=", env[i], 9))
			return (env[i] + 9);
		i++;
	}
	return ("ip not found");
}

char	*get_serv_ip(char **env)
{
	int	i;

	if (env == NULL)
		return (NULL);
	i = 0;
	while (env[i] != NULL)
	{
		if (!ft_strncmp("SERVER_IP=", env[i], 10))
			return (env[i] + 10);
		i++;
	}
	return ("ip not found");
}
