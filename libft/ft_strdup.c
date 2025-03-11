/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 12:09:47 by topiana-          #+#    #+#             */
/*   Updated: 2025/01/19 11:30:10 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s)
{
	char			*sdup;
	size_t			size;

	if (s == NULL)
		return (NULL);
	size = ft_strlen(s) + 1;
	sdup = (char *)ft_calloc(size, sizeof(char));
	if (sdup == NULL)
		return (NULL);
	ft_strlcpy(sdup, s, size);
	return (sdup);
}
/*
#include <string.h>
#include <stdio.h>
int	main(int argc, char *argv[])
{
	//printf("%s\n", strdup(argv[1]));
	printf("%s\n", ft_strdup(argv[1]));
	return (0);
}*/
