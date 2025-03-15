/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniclient.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: totommi <totommi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 01:20:06 by totommi           #+#    #+#             */
/*   Updated: 2025/03/15 01:37:40 by totommi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//std libraries
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h> 
# include <string.h>

//net libraries
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
//# include <sys/types.h>
# include <netdb.h>

# define MYPORT 8080

int main(void)
{
    int servfd;
    
    if ((servfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		perror("socket failure");
    
    struct sockaddr_in serveraddr;

	memset(&serveraddr, 0, sizeof(struct sockaddr_in));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons ( MYPORT );
	serveraddr.sin_addr.s_addr = inet_addr("192.168.1.5"); //192.168.1.5 //INADDR_ANY

    if (connect(servfd, (struct sockaddr *)&serveraddr, sizeof(struct sockaddr)))
        perror("connection failed");
    ft_printf("connection accepted!!!\n");

    if (send(servfd, "Hi there, my name is pippo", 26, 0))
        perror("send failure");

    int len;
    char buffer[1024] = { 0 };
    if ((len = recv(servfd, buffer, 1023, 0)) < 0)
        perror("recv failure");
    else
        ft_printf("%d bytes: '%s' from Server\n", len, buffer);

    close(servfd);
    return (0);    
}
