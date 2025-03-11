/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 16:48:41 by topiana-          #+#    #+#             */
/*   Updated: 2024/11/21 21:46:52 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i] != '\0')
		i++;
	return (i);
}
/*
#include <string.h>
#include <stdio.h>
int	main(int argc, char *argv[])
{
	if (argc == 2)
	{
		printf("%zu\n", ft_strlen(argv[1]));	
		printf("%zu\n", strlen(argv[1]));
	}
	//printf("%zu\n", ft_strlen(NULL));
	printf("%zu\n", strlen(NULL));
	return (0);
}*/
