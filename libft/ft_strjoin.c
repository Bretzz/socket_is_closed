/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 13:22:54 by topiana-          #+#    #+#             */
/*   Updated: 2024/11/22 14:56:41 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	size;
	char	*twelve;

	size = ft_strlen(s1) + ft_strlen(s2) + 1;
	twelve = (char *)malloc(size);
	if (twelve == NULL)
		return (NULL);
	ft_strlcpy(twelve, s1, ft_strlen(s1) + 1);
	ft_strlcat(twelve, s2, size);
	return (twelve);
}

/*
#include <stdio.h>

int	main(int argc, char *argv[])
{
	printf("%s\n", ft_strjoin(argv[1], argv[2]));
	return (0);
}*/
