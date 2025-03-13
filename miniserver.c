/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniserver.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: totommi <totommi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 01:03:17 by totommi           #+#    #+#             */
/*   Updated: 2025/03/13 01:39:38 by totommi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "socket_is_closed.h"

int main(void)
{
	int recvfd, connfd;

	if ((recvfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		perror("socket failure");

	struct sockaddr_in serveraddr;

	memset(&serveraddr, 0, sizeof(struct sockaddr_in));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons ( MYPORT );
	serveraddr.sin_addr.s_addr = htonl( INADDR_ANY );

	if (bind(recvfd, (struct sockaddr *)&serveraddr, sizeof(struct sockaddr_in)))
		perror("bind failure");

	if (listen(recvfd, 10))
		perror("listen failure");

	struct sockaddr_in clientaddr;
	socklen_t addrlen;

	memset(&clientaddr, 0, sizeof(struct sockaddr_in));
	if ((connfd = accept(recvfd, (struct sockaddr *)&clientaddr, &addrlen)) < 0)
		perror("accept failed");
	printf("connection accepted!\n");
	
	int	len;
	char buffer[1024] = { 0 };
	if ((len = recv(connfd, buffer, 1023, 0)) < 0)
		perror("recv failure");
	else
		printf("%d bytes: %s\n", len, buffer);
	//write(connfd, "HTTP/1.0 200 OK\r\n\r\nHello sweety", 35);
	if ((len = send(connfd, "HTTP/1.0 200 OK\r\n\r\nHello sweety", 35, 0)) < 0)
		perror("send failure");
	close(connfd);
}
