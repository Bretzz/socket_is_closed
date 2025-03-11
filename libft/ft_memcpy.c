/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 17:12:25 by topiana-          #+#    #+#             */
/*   Updated: 2024/11/25 13:45:12 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char	*dest_c;
	unsigned char	*src_c;
	size_t			i;

	dest_c = (unsigned char *)dest;
	src_c = (unsigned char *)src;
	i = 0;
	while (i < n)
	{
		dest_c[i] = src_c[i];
		i++;
	}
	return (dest);
}
/*
#include <stdio.h>
#include <string.h>
int	main(int argc, char *argv[])
{
	char	s1[] = "1234567890";
	char	s2[] = "1234567890";
	char	dest[10];

	if (argc == 1)
	{
		//printf("%s\n", memcpy(NULL, NULL, 10));
		printf("%s\n", ft_memcpy(NULL, NULL, 10));
		//printf("%s\n", dest);
	}
	//printf("%s\n", memcpy(s1 + 1, s1, 3));
	//printf("%s\n", ft_memcpy(s2 + 1, s2, 3));
	return (0);
}*/
