/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 10:32:59 by topiana-          #+#    #+#             */
/*   Updated: 2024/12/21 16:05:44 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*mem;

	if (!(nmemb && size) || nmemb > SIZE_MAX / size)
	{
		nmemb = 1;
		size = 1;
	}
	mem = malloc(nmemb * size);
	if (mem == NULL)
		return (mem);
	else
		ft_bzero(mem, nmemb * size);
	return (mem);
}

/* #include <stdio.h>
#include <string.h>
int	main(int argc, char *argv[])
{
	char	*s1;
	char	*s2;

	s1 = calloc(ft_atoi(argv[1]), ft_atoi(argv[2]));
	printf("%p\n", s1);
	s2 = ft_calloc(ft_atoi(argv[1]), ft_atoi(argv[2]));
	printf("%p\n", s2);
	printf("%d\n", memcmp(s1, s2, (ft_atoi(argv[1]) * ft_atoi(argv[2]))));
	return (0);
} */
