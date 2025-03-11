/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 17:18:11 by topiana-          #+#    #+#             */
/*   Updated: 2024/11/23 18:08:12 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char	*dest_c;
	unsigned char	*src_c;

	if (!n)
		return (dest);
	dest_c = (unsigned char *)dest;
	src_c = (unsigned char *)src;
	if (dest_c > src_c)
	{
		while (n--)
			dest_c[n] = src_c[n];
	}
	else if (dest_c < src_c)
	{
		while (n--)
			*dest_c++ = *src_c++;
	}
	return (dest);
}
/*
#include <stdio.h>
#include <string.h>
int	main(int argc, char *argv[])
{
	char	s1[] = "lorem ipsum dolor sit amet";
	char	s2[] = "lorem ipsum dolor sit amet";
	char	*dest;

	if (argc == 1)
	{
		printf("%s\n", memmove(NULL, NULL, 0));
		//printf("%s\n", ft_memmove(NULL, NULL, 10));
		//printf("%s\n", dest);
	}
	//printf("%s\n", memmove(s1, s1 + 1, 8));
	//printf("%s\n", ft_memmove(s2, s2 + 1, 8));
	//printf("%s\n", memmove(s1 + 1, "consectetur", 5));
	//printf("%s\n", ft_memmove(s2 + 1, "consectetur", 5));
	return (0);
}*/
