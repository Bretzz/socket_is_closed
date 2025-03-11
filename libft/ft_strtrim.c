/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 14:46:57 by topiana-          #+#    #+#             */
/*   Updated: 2024/11/23 18:16:02 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	unsigned int	i;
	unsigned int	len;

	if (s1 == NULL)
		return (NULL);
	i = 0;
	while (ft_strchr(set, s1[i]))
		i++;
	len = ft_strlen(s1);
	if (i >= len)
		return ((char *)ft_calloc(1, sizeof(char)));
	while (ft_strchr(set, s1[len]))
		len--;
	len = len - i + 1;
	return (ft_substr(s1, i, len));
}
/*
#include <stdio.h>
#include <string.h>
int	main(int argc, char *argv[])
{
 	char *s1 = "  \t \t \n   \n\n\n\t";
 	char *s2 = "";
 	char *ret = ft_strtrim(s1, " \n\t");
 
 	if (!strcmp(ret, s2))
		printf("OK\n");
	else
		printf("KO\n");
	return (0);
}*/
