/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerUDP.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 22:59:44 by topiana-          #+#    #+#             */
/*   Updated: 2025/03/11 13:35:52 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h> 
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAXLINE 1000 
#define MAXPLAYERS 2

int main( void )
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

    struct sockaddr_in clientaddr[MAXPLAYERS], addrin;
    socklen_t addr_len = sizeof(struct sockaddr_in);
    /* memset(&clientaddr[0], 0, sizeof(clientaddr[0]));
    memset(&clientaddr[1], 0, sizeof(clientaddr[1])); */

	int	player_count = 0;
	int	i;
    char buffer[MAXLINE];
    while ( 1 )
	{
        int length = recvfrom( fd, buffer, sizeof(buffer) - 1, 0, (struct sockaddr *)&addrin, &addr_len );
        if ( length < 0 ) {
            perror( "recvfrom failed" );
            break;
        }
		buffer[length] = '\0';
		if (player_count != 0)
		{
			i = 0;
			while (i < player_count && strcmp(inet_ntoa(addrin.sin_addr), inet_ntoa(clientaddr[i].sin_addr)) != 0) //searches if the socket is already registered
				i++;
			if (i == player_count - 1 && player_count < MAXPLAYERS -1) //adds a new client
			{
				memmove(&clientaddr[player_count], &addrin, addr_len);
				player_count++;
				printf("a new player has arrived!!!\nPLAYER COUNT: %i\n", player_count);
			}
		}
		else //just adds the first player
		{
			memmove(&clientaddr[player_count], &addrin, addr_len);
			player_count++;
			printf("a new player has arrived!!!\nPLAYER COUNT: %i, IP: %s\n", player_count, inet_ntoa(clientaddr[player_count - 1].sin_addr));
		}
        printf( "%d bytes: '%s' from: %s\n", length, buffer, inet_ntoa(addrin.sin_addr) );
    }

    close( fd );
}
