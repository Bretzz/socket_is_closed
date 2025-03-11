/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientUDP.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 22:59:15 by topiana-          #+#    #+#             */
/*   Updated: 2025/03/12 00:15:40 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "socket_is_closed.h"

int main( void )
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
    serveraddr.sin_addr.s_addr = htonl( 0x7f000001 );  //my_pc=0x0a0c0307 //loclhost=0x7f000001

	//replace-next
	/* if (sendto( fd, "0x0a0c0307", 10, 0, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0 ) {
		perror( "sendto failed" );
	} */

	t_player player;
	memset(&player, 0, sizeof(t_player));
	
	//player init
	memmove(&player.sockaddr, &serveraddr, sizeof(struct sockaddr_in));
	memmove(&player.ip, "0.0.0.0", 15);
	memmove(&player.name, "pippo", 5);
	player.socket = fd;
	printf("fd=%i\n", fd);

	pthread_t	tid;
	
	if (pthread_create(&tid, NULL, &minigame, &player) < 0)
		perror( "minigame launch failed" );

    /* for ( int i = 0; i < 4; i++ ) {
        if (sendto( fd, "hello", 5, 0, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0 ) {
            perror( "sendto failed" );
            break;
        }
        printf( "message sent\n" );
    } */

	pthread_join(tid, NULL);
    close( fd );
}
