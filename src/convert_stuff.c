/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   convert_stuff.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: totommi <totommi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 14:30:56 by topiana-          #+#    #+#             */
/*   Updated: 2025/03/13 17:18:33 by totommi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "socket_is_closed.h"

unsigned int	ip_to_uns(const char *ip);

int		ft_atohexi(const char *nptr);
char	*ft_itohex_buff(char *buff, unsigned int nb);

/* Takes a char pointer and an unsigned int as parameters.
Concatenates (strlcat(3)) the buffer with the unsigned int
converted to hexadecimal.
RETURNS: the buffer, NULL if buff is NULL. */
char	*ft_itohex_buff(char *buff, unsigned int nb)
{
	char		base[] = "0123456789abcdef";

	if (buff == NULL)
		return (NULL); 
	if (nb >= 16)
		ft_itohex_buff(buff, nb / 16);
	ft_strlcat(buff, &base[(nb % 16)], strlen(buff) + 2);
	return (buff);
}

/* converts a string into an int.
Skips all the blank spaces, then if the number starts with '0x'
then is treated as an hexadecimal number, if it isn't is treated as
a decimal number (ft_atoi(3)).
RETURNS: the number (int) obtained. */
int		ft_atohexi(const char *nptr)
{
	int		nb;
	char	base[] = "0123456789abcdef";

	if (nptr == NULL)
		return (0);
	while (ft_strchr(" \f\n\r\t\v", (char)*nptr) && *nptr)
		nptr++;
	if (ft_strncmp("0x", nptr, 2) != 0)
		return (ft_atoi(nptr));
	nptr += 2;
	nb = 0;
	while ((*nptr >= '0' && *nptr <= '9')
		|| (*nptr >= 'a' && *nptr <= 'f'))
	{
		nb = nb * 16 + (ft_strichr(base, *nptr) - 1);
		nptr++;
	}
	return (nb);
}

/* takes an IP (000.000.000.000) as parameter.
Converts it's value into the corresponding unsigned int
(needed by htonl(3)).
RETURNS: the value (unsigned int) obtained, 0 if 0.0.0.0 or the pointer is NULL.
! ! ! FIX THE 0 RETURN ! ! ! */
unsigned int	ip_to_uns(const char *ip)
{
	char			**split;
	unsigned int		uns;
	char			hex[10];
	int				nb;
	int				i;

	split = ft_split(ip, '.');
	if (split == NULL)
		return (0);
	uns = 0;
	i = 0;
	ft_memset(hex, 0, 10);
	ft_strlcat(hex, "0x", 3);
	while (i < 4)
	{
		nb = ft_atoi(split[i]);
		if (nb <= 16)
			ft_strlcat(hex, "0", strlen(hex) + 2);
		ft_itohex_buff(hex, nb); 
		i++;
	}
	uns = ft_atohexi(hex);
	ft_freentf("2", split);
	return (uns);
}
