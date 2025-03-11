/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 16:53:24 by topiana-          #+#    #+#             */
/*   Updated: 2024/11/21 21:47:08 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *s, int c, size_t n)
{
	size_t			i;
	unsigned char	*s_c;

	s_c = (unsigned char *)s;
	i = 0;
	while (i < n)
	{
		s_c[i] = c;
		i++;
	}
	return (s);
}
/*
#include <stdio.h>
#include <string.h>
int	main(int argc, char *argv[])
{
	char	str[3] = "aaa";
	
	if (argc == 1)
	{
		ft_memset(str, 48, -1);
		//memset(str, 48, -1);
		printf("%s\n", str);
	}
	if (argc == 2)
	{
		memset(argv[1], 48, 3);
		printf("%s\n", argv[1]);
		ft_memset(argv[1], 48, 3);
		printf("%s\n", argv[1]);
	}
	return (0);
}*/
