/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientUDP.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 22:49:20 by topiana-          #+#    #+#             */
/*   Updated: 2025/03/10 22:55:47 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h> 
#include <strings.h> 
#include <sys/types.h> 
#include <arpa/inet.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#define PORT 8080 
#define MAXLINE 1000 
  
// Driver code 
int main() 
{    
    char buffer[100]; 
    char *message = "Hello Client"; 
    int listenfd, len;
	//socklen_t len; 
    struct sockaddr_in servaddr, cliaddr; 
    bzero(&servaddr, sizeof(servaddr)); 
  
    // Create a UDP Socket 
    listenfd = socket(AF_INET, SOCK_DGRAM, 0);         
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    servaddr.sin_port = htons(PORT); 
    servaddr.sin_family = AF_INET;  
   
    // bind server address to socket descriptor 
    bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)); 
       
    //receive the datagram 
    len = sizeof(cliaddr); 
    int n = recvfrom(listenfd, buffer, sizeof(buffer), 
            0, (struct sockaddr*)&cliaddr, &len); //receive message from server 
    buffer[n] = '\0'; 
    puts(buffer); 
           
    // send the response 
    sendto(listenfd, message, MAXLINE, 0, 
          (struct sockaddr*)&cliaddr, sizeof(cliaddr)); 
}
