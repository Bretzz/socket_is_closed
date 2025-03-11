/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 19:50:21 by topiana-          #+#    #+#             */
/*   Updated: 2024/11/21 14:01:04 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	int		i;
	char	*map;

	if (!s || !f)
		return (NULL);
	map = (char *)ft_calloc(ft_strlen(s) + 1, sizeof(char));
	if (map == NULL)
		return (NULL);
	i = 0;
	while (s[i] != '\0')
	{
		map[i] = (*f)(i, s[i]);
		i++;
	}
	return (map);
}
/*
#include <stdio.h>

char	to_A(unsigned int i, char c)
{
	return ('A');
}

int	main(int argc, char *argv[])
{
	printf("%s\n", ft_strmapi(argv[1], NULL));
	return (0);
}*/
