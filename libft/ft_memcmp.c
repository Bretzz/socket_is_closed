/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 18:48:33 by topiana-          #+#    #+#             */
/*   Updated: 2024/11/25 13:23:37 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	size_t			i;
	unsigned char	*s1_c;
	unsigned char	*s2_c;

	if (n == 0)
		return (0);
	s1_c = (unsigned char *)s1;
	s2_c = (unsigned char *)s2;
	i = 0;
	while (s1_c[i] == s2_c[i] && i < n - 1)
		i++;
	return (s1_c[i] - s2_c[i]);
}
/*
#include <string.h>
#include <stdlib.h>

int	main(int argc, char *argv[])
{
	printf("%d\n", memcmp(argv[1], argv[2], atoi(argv[3]))); 
	printf("%d\n", ft_memcmp(argv[1], argv[2], atoi(argv[3]))); 
	return (0);
}*/
