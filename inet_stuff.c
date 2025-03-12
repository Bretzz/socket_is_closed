/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inet_stuff.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 15:35:22 by topiana-          #+#    #+#             */
/*   Updated: 2025/03/12 15:51:13 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "socket_is_closed.h"

/* static int	express_sendto(char *ip)
{	
	int fd;
    if ( (fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        perror("socket failed");
        return 1;
    }

    struct sockaddr_in serveraddr;
    memset( &serveraddr, 0, sizeof(serveraddr) );
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons( 50037 );              
    serveraddr.sin_addr.s_addr = htonl( (uint32_t)ip_to_uns(ip) );  //my_pc=0x0a0c0307 //loclhost=0x7f000001

	if (ft_strncmp("127.0.0.1", ip, 9) != 0
		&& ft_strncmp("10.12.1.10", ip, 9) != 0)
	{
		if ( bind(fd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0 )
		{
			perror( "bind failed" );
			return (1);
		}
	}

	//ip_to_uns("127.0.0.1");
	printf("fd=%i\n", fd);

	if (sendto( fd, "I see you...", 12, 0, (struct sockaddr *)&serveraddr, sizeof(struct sockaddr_in)) < 0 )
		perror( "sendto failed" );
	return (0);
} */

int server_duty(void)
{
	int fd;
    if ( (fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 )
	{
        perror( "socket failed" );
        return (1);
    }

    struct sockaddr_in serveraddr;
    memset( &serveraddr, 0, sizeof(serveraddr) );
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons( 50037 );
    serveraddr.sin_addr.s_addr = htonl( INADDR_ANY );

    if ( bind(fd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0 )
	{
        perror( "bind failed" );
        return (1);
    }

	t_player player;
	memset(&player, 0, sizeof(t_player));
    struct sockaddr_in addrin;
    socklen_t addr_len = sizeof(struct sockaddr_in);

	int	player_count = 0;
    char buffer[MAXLINE];
    while ( 1 )
	{
        int length = recvfrom( fd, buffer, sizeof(buffer) - 1, 0, (struct sockaddr *)&addrin, &addr_len );
        if ( length < 0 ) {
            perror( "recvfrom failed" );
            break;
        }
		buffer[length] = '\0';
		if (player_count == 0) //just adds the first player
		{
			memmove(&player.sockaddr, &addrin, addr_len);
			memmove(&player.ip, inet_ntoa(addrin.sin_addr), 15);
			memmove(&player.name, buffer + 11, 10);
			player.socket = fd;
			player_count++;
			printf("a new player has arrived!!!\nPLAYER COUNT: %i, IP: %s, NAME: %s\n", player_count, player.ip, player.name);
		}
		if (strncmp(player.ip, inet_ntoa(addrin.sin_addr), 15) != 0)
			printf("someone that isn't the player is trying to conntect!!!\n");
		else
		{
			printf( "%d bytes: '%s' from: %s\n", length, buffer, player.name );
			//express_sendto(player.ip);
		}
    }
    close( fd );
	return (0);
}
