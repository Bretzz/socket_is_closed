/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   west_putstr_fd.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 20:25:27 by topiana-          #+#    #+#             */
/*   Updated: 2025/03/11 15:52:23 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "libft.h"

int	west_putstr_fd(int fd, char *s)
{
	int	i;

	if (!s)
		return (west_putstr_fd(fd, "(null)"));
	i = 0;
	while (s[i] != '\0')
	{
		write(fd, &s[i], 1);
		i++;
	}
	return (i);
}
