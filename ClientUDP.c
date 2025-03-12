/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientUDP.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: totommi <totommi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 22:59:15 by topiana-          #+#    #+#             */
/*   Updated: 2025/03/12 12:58:50 by totommi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "socket_is_closed.h"

/*check for the value 'c' in the string 's',
if found returns the pos (index + 1) of the first instance of 'c'*/
/* static int	ft_strichr(const char *s, int c)
{
	unsigned int	i;

	c = (unsigned char)c;
	i = 0;
	while (s[i] != '\0' && s[i] != c)
		i++;
	if (s[i] == '\0' && c != '\0')
		return (0);
	return (i + 1);
} */

/* returns a pointer to the value of the IP var in the env */
static char	*get_my_ip(char **env)
{
	int	i;

	if (env == NULL)
		return (NULL);
	i = 0;
	while (env[i] != NULL)
	{
		if (!ft_strncmp("IP=", env[i], 3))
			return (env[i] + 3);
		i++;
	}
	return ("ip not found");
}

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

static uint32_t	ip_to_hostl(const char *ip)
{
	char			**split;
	uint32_t		uns;
	char			hex[10];
	int				nb;
	int				i;

	split = ft_split(ip, '.');
	if (split == NULL)
		return (0);
	uns = 0;
	i = 0;
	memset(hex, 0, 10);
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

void	*reciever(void *arg)
{
	const char	*ip;

	ip = (const char *)arg;
	int fd;
    if ( (fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 )
	{
        perror( "socket failed" );
        return (NULL);
    }

    struct sockaddr_in serveraddr;
    memset( &serveraddr, 0, sizeof(serveraddr) );
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons( 50037 );
    serveraddr.sin_addr.s_addr = htonl( ip_to_hostl(ip) );

    if ( bind(fd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0 )
	{
        perror( "bind failed" );
        return (NULL);
    }

    struct sockaddr_in addrin;
    socklen_t addr_len = sizeof(struct sockaddr_in);
	
    char buffer[MAXLINE];
    while ( 1 )
	{
        int length = recvfrom( fd, buffer, sizeof(buffer) - 1, 0, (struct sockaddr *)&addrin, &addr_len );
        if ( length < 0 ) {
            perror( "recvfrom failed" );
            break;
        }
		printf( "%d bytes: '%s' from: %s\n", length, buffer, ip );
	}
	return (NULL);
}

int main( int argc, char *argv[], char *env[] )
{
	if (argc != 2)
	{
		printf("no server IP provided\n");
		return (1);
	}
	
    int fd;
    if ( (fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        perror("socket failed");
        return 1;
    }

    struct sockaddr_in serveraddr;
    memset( &serveraddr, 0, sizeof(serveraddr) );
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons( 50037 );              
    serveraddr.sin_addr.s_addr = htonl( ip_to_hostl(argv[1]) );  //my_pc=0x0a0c0307 //loclhost=0x7f000001

	ip_to_hostl("127.0.0.1");

	//replace-next
	/* if (sendto( fd, "0x0a0c0307", 10, 0, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0 ) {
		perror( "sendto failed" );
	} */

	t_player player;
	memset(&player, 0, sizeof(t_player));
	
	//player init
	memmove(&player.sockaddr, &serveraddr, sizeof(struct sockaddr_in));
	memmove(&player.ip, get_my_ip(env), 15);
	memmove(&player.name, "pippo", 5);
	player.socket = fd;
	printf("fd=%i\n", fd);

	if (sendto( player.socket, "My name is pippo", 21, 0, (struct sockaddr *)&player.sockaddr, sizeof(struct sockaddr_in)) < 0 )
		perror( "sendto failed" );

	pthread_t	tid;
	
    //reciever
	if (pthread_create(&tid, NULL, &reciever, NULL) < 0)
		perror( "minigame launch failed" );

    minigame(&player);

	pthread_join(tid, NULL);
    close( fd );
}
