/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_freentf.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 14:49:52 by topiana-          #+#    #+#             */
/*   Updated: 2025/03/17 15:05:59 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "socket_is_closed.h"

static void	free_mtx(void **mtx)
{
	int	i;
	
	if (mtx == NULL)
	return ;
	i = 0;
	while (mtx[i] != NULL)
	{
		free(mtx[i]);
		i++;
	}
	free(mtx);
}

static void	free_mtx_mtx(void ***mtx)
{
	int	i;

	if (mtx == NULL)
		return ;
	i = 0;
	while (mtx[i] != NULL)
	{
		free_mtx(mtx[i]);
		i++;
	}
	free(mtx);
}

int	ft_freentf(const char *s, ...)
{
	va_list	arg;
	if (!s)
		return (0);
	va_start(arg, s);
	while (*s != '\0')
	{
		if (*s == '1')
			free(va_arg(arg, void *));
		else if (*s == '2')
			free_mtx(va_arg(arg, void **));
		else if (*s == '3')
			free_mtx_mtx(va_arg(arg, void ***));
		else
		{
			ft_printfd(2, "Invalid pointer type\n");
			return (0);
		}
		s++;
	}
	va_end(arg);
	return (1);
}
