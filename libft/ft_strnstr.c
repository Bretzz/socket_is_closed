/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 09:23:23 by topiana-          #+#    #+#             */
/*   Updated: 2024/11/23 17:43:50 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	unsigned int	i;
	unsigned int	j;

	if (*little == '\0')
		return ((char *)big);
	i = 0;
	j = 0;
	while (big[i] != '\0' && i < len)
	{
		if (little[j] == '\0')
			break ;
		j = 0;
		while (little[j] && big[i + j] == little[j])
			j++;
		i++;
	}
	if (little[j] == '\0' && (i + j - 1) <= len)
		return ((char *)(big + i - 1));
	return (NULL);
}
/*
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
int	main(int argc, char *argv[])
{
	if (argc == 4)
		printf("%s\n", ft_strnstr(argv[1], argv[2], atoi(argv[3])));
	printf("%s\n", ft_strnstr("aaabcabcd", "aabc", -1));
	printf("%s\n", ft_strnstr("aaabcabcd", "abcd", 9));
	printf("%s\n", ft_strnstr("lorem ipsum dolor sit amet", "dolor", 15));
	return (0);
}*/
