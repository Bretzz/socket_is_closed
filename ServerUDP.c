/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerUDP.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: totommi <totommi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 22:59:44 by topiana-          #+#    #+#             */
/*   Updated: 2025/03/11 01:28:12 by totommi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h> 
#include <string.h>
#include <sys/socket.h> 
#include <netinet/in.h> 
#define PORT 8080
#define MAXLINE 1000 
#define MAXPLAYERS 2

int main( void )
{
    int fd;
    if ( (fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        perror( "socket failed" );
        return 1;
    }

    struct sockaddr_in serveraddr;
    memset( &serveraddr, 0, sizeof(serveraddr) );
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons( 50037 );
    serveraddr.sin_addr.s_addr = htonl( INADDR_ANY );

    if ( bind(fd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0 ) {
        perror( "bind failed" );
        return 1;
    }

    struct sockaddr clientaddr;
    socklen_t clientaddr_len = sizeof(clientaddr);
    /* memset(&clientaddr[0], 0, sizeof(clientaddr[0]));
    memset(&clientaddr[1], 0, sizeof(clientaddr[1])); */

    char buffer[200];
    while ( 1 ) {
        int length = recvfrom( fd, buffer, sizeof(buffer) - 1, 0, &clientaddr, &clientaddr_len );
        if ( length < 0 ) {
            perror( "recvfrom failed" );
            break;
        }
        buffer[length] = '\0';
        printf( "%d bytes: '%s' from: %s\n", length, buffer, clientaddr.sa_data );
    }

    close( fd );
}