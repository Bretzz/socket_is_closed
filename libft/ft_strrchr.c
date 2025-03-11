/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 16:17:12 by topiana-          #+#    #+#             */
/*   Updated: 2024/11/21 23:38:17 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	int	i;

	c = (unsigned char)c;
	i = ft_strlen(s);
	while (i >= 0)
	{
		if (*(s + i) == c)
			return ((char *)(s + i));
		i--;
	}
	return (NULL);
}
/*
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int	main(int argc, char *argv[])
{
	printf("%p\n", strrchr(argv[1], atoi(argv[2])));
	printf("%p\n", ft_strrchr(argv[1], atoi(argv[2])));
	return (0);
}*/
