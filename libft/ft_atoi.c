/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 09:49:09 by topiana-          #+#    #+#             */
/*   Updated: 2024/11/23 18:30:13 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(const char *nptr)
{
	int				sign;
	int				nb;
	unsigned char	*nptr_c;

	nptr_c = (unsigned char *)nptr;
	while (ft_strchr(" \f\n\r\t\v", (char)*nptr_c) && *nptr_c)
		nptr_c++;
	sign = 1;
	if (*nptr_c == '-')
	{
		sign = -1;
		nptr_c++;
	}
	else if (*nptr_c == '+')
		nptr_c++;
	nb = 0;
	while (*nptr_c >= '0' && *nptr_c <= '9')
	{
		nb = nb * 10 + (*nptr_c - '0');
		nptr_c++;
	}
	return (nb * sign);
}
/*
#include <stdio.h>
#include <stdlib.h>
int	main(int argc, char *argv[])
{
	printf("%d\n", atoi("\t -123"));
	printf("%d\n", ft_atoi("\t -123"));
	printf("%d\n", atoi("\0 1"));
	printf("%d\n", ft_atoi("\0 \0 1"));
	//printf("%d\n", atoi(argv[1]));
	//printf("%d\n", ft_atoi(argv[1]));
	return (0);
}*/
