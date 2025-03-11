/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalpha.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 16:18:44 by topiana-          #+#    #+#             */
/*   Updated: 2024/11/21 21:46:31 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_isalpha(int c)
{
	if (c < 'A' || (c > 'Z' && c < 'a') || c > 'z')
		return (0);
	else
		return (1);
}
/*
#include "libft.h"
#include <ctype.h>
#include <stdio.h>
int	main(int argc, char *argv[])
{
	if (argc == 2)
	{
		printf("%i\n", ft_isalpha(*argv[1]));	
		printf("%i\n", isalpha(*argv[1]));
	}
	printf("%i\n", ft_isalpha(NULL));
	printf("%i\n", isalpha(NULL));
	return (0);
}*/
