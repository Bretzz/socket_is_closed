/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 18:15:39 by topiana-          #+#    #+#             */
/*   Updated: 2024/11/21 23:36:51 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	unsigned int	i;

	c = (unsigned char)c;
	i = 0;
	while (s[i] != '\0' && s[i] != c)
		i++;
	if (s[i] == '\0' && c != '\0')
		return (NULL);
	return ((char *)(s + i));
}
/*
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int	main(int argc, char *argv[])
{
	printf("%s\n", strchr(argv[1], atoi(argv[2])));
	printf("%s\n", ft_strchr(argv[1], atoi(argv[2])));
	return (0);
}*/
