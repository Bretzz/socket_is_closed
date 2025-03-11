/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 18:18:59 by topiana-          #+#    #+#             */
/*   Updated: 2024/11/23 18:24:45 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	if (n == 0)
		return (0);
	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i] && i < n - 1)
		i++;
	return ((unsigned char)(s1[i]) - (unsigned char)(s2[i]));
}

/* #include <string.h>
#include <stdlib.h>
#include <stdio.h>
int	main(int argc, char *argv[])
{
	printf("%d\n", strncmp("test\200", "test\0", 6));
	printf("%d\n", ft_strncmp("test\200", "test\0", 6));
	printf("%d\n", strncmp(argv[1], argv[2], atoi(argv[3])));
	printf("%d\n", ft_strncmp(argv[1], argv[2], atoi(argv[3])));
	return (0);
} */
