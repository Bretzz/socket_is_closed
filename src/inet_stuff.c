/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inet_stuff.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: totommi <totommi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 15:35:22 by topiana-          #+#    #+#             */
/*   Updated: 2025/03/17 10:35:47 by totommi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "socket_is_closed.h"

int	is_ip(const char *s)
{
	int	i[2];
	
	if (s == NULL)
		return (0);
	i[0] = 0;
	while (i[0] < 3)
	{
        i[1] = 0;
		while (*s >= '0' && *s <= '9' && i[1]++ < 3)
			s++;
		if (*(s++) != '.')
			return (0);
		i[0]++;
	}
    i[1] = 0;
	while (*s >= '0' && *s <= '9' && i[1]++ < 3)
		s++;
	if (*s == '.')
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
		if (!ft_strncmp("LOCAL_IP=", env[i], 9) && *(env[i] + 9) != '\0')
			return (env[i] + 9);
		i++;
	}
	return ("ip-not-found");
}

char	*get_serv_ip(char **env)
{
	int	i;

	if (env == NULL)
		return (NULL);
	i = 0;
	while (env[i] != NULL)
	{
		if (!ft_strncmp("SERVER_IP=", env[i], 10) && *(env[i] + 10) != '\0')
			return (env[i] + 10);
		i++;
	}
	return ("ip-not-found");
}

char	*get_my_name(char **env)
{
	int	i;

	if (env == NULL)
		return (NULL);
	i = 0;
	while (env[i] != NULL)
	{
		if (!ft_strncmp("NAME=", env[i], 5) && *(env[i] + 5) != '\0')
			return (env[i] + 5);
		i++;
	}
	return ("no-name");
}

int	make_me_host(char **env)
{
	int	i;

	if (env == NULL)
		return (0);
	i = 0;
	while (env[i] != NULL)
	{
		if (!strncmp("SERVER_IP=", env[i], 10) && *(env[i] + 10) != '\0')
		{
			ft_memset(env[i] + 10, 0, ft_strlen(env[i] + 10));
			ft_memmove(env[i] + 10, "host", 4);
			return (1);
		}
		i++;
	}
	return (0);
}

int	make_him_host(char *ip, char **env)
{
	int	i;

	if (ip == NULL || env == NULL)
		return (0);
	i = 0;
	while (env[i] != NULL)
	{
		if (!strncmp("SERVER_IP=", env[i], 10) && *(env[i] + 10) != '\0')
		{
			ft_memset(env[i] + 10, 0, ft_strlen(env[i] + 10));
			ft_memmove(env[i] + 10, ip, ft_strlen(ip));
			return (1);
		}
		i++;
	}
	return (0);
}
