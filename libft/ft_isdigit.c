/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isdigit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 16:37:26 by topiana-          #+#    #+#             */
/*   Updated: 2024/11/21 21:46:43 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_isdigit(int c)
{
	if (c < '0' || c > '9')
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
		printf("%i\n", ft_isdigit(*argv[1]));	
		printf("%i\n", isdigit(*argv[1]));
	}
	printf("%i\n", ft_isdigit(NULL));
	printf("%i\n", isdigit(NULL));
	return (0);
}*/
