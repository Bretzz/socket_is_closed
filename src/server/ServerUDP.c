/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerUDP.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: totommi <totommi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 22:59:15 by topiana-          #+#    #+#             */
/*   Updated: 2025/03/17 01:24:50 by totommi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "socket_is_closed.h"

//split up server reciever pls
//static void	reciever_init(t_recenv *recenv){}

/* accept one connnection and starts to listen to it.
updates recenv->index so that the next thread can go onto
the next slot.
TEORICALLY just updates the data of the player selected, so no mutex is needed. */
static void	*server_reciever(void *arg)
{
	t_recenv	*recenv;
	int			connfd;
	int			my_index;
	char		dead;

	recenv = (t_recenv *)arg;
	my_index = recenv->index;

	struct sockaddr_in clientaddr;
	socklen_t addrlen;

	ft_memset(&clientaddr, 0, sizeof(struct sockaddr_in));
	if ((connfd = accept(recenv->socket, (struct sockaddr *)&clientaddr, &addrlen)) < 0)
		perror(ERROR"accept failed"RESET);
	else
		ft_printf(GREEN"connection accepted!\n"RESET);
	
	//pthread_mutex_lock(&recenv->player_mutex); //no other thread will accept another connection since it isn't spawned yet

	int	len;
	char buffer[MAXLINE] = { 0 };
	if ((len = recvfrom(connfd, buffer, MAXLINE - 1, 0, (struct sockaddr *)&clientaddr, &addrlen)) < 0) //expecting a message with ip:name
		perror(ERROR"recv failure"RESET);
	else
		ft_printf("%d bytes: %s from %s\n", len, buffer, inet_ntoa(clientaddr.sin_addr));

	recenv->player[my_index].mutex = &recenv->player_mutex;
	register_player(my_index, buffer, connfd, recenv->player); //register player one
	//server signature name:ip
	ft_memset(buffer, 0, MAXLINE);
	ft_strlcpy(buffer, get_my_name(recenv->env), 11); //last place with name length capped to 10
	ft_strlcat(buffer, ":", ft_strlen(buffer) + 2);
	ft_strlcat(buffer, get_locl_ip(recenv->env), ft_strlen(buffer) + 16);
	//send test "HTTP/1.0 200 OK\r\n\r\nHello sweety"
	if (send(recenv->player[my_index].socket, buffer, ft_strlen(buffer), 0) < 0)
		perror(ERROR"send failure"RESET);

	ft_memset(buffer, 0, MAXLINE);
	get_lobby_stats(buffer, recenv->player);
	if (send(recenv->player[my_index].socket, buffer, ft_strlen(buffer), 0) < 0)
		perror(ERROR"send failure"RESET);

	recenv->index++;
	//pthread_mutex_unlock(&recenv->player_mutex);
	
	//how do I listen to multiple players?
	//WAIT
	pthread_t	tid;

	//reciever
	if (pthread_create(&tid, NULL, &server_reciever, recenv) < 0)
		perror(ERROR"reciever launch failed"RESET);
	pthread_detach(tid);
	
	struct sockaddr_in addrin;

	dead = 0;
	while (!dead)
	{
		ft_memset(buffer, 0, MAXLINE);
		ft_printf("talk to me baby...\n");
		len = recvfrom( recenv->player[my_index].socket, buffer, MAXLINE - 1, 0, (struct sockaddr *)&addrin, &addrlen );
		if ( len < 0 ) {
			perror(ERROR"recvfrom failed"RESET);
			break;
		}
		ft_printf(YELLOW"%d bytes: '%s' from %s\n"RESET, len, buffer, inet_ntoa(addrin.sin_addr));
		handle_server_players(&dead, len, buffer, recenv);
	}
	ft_printf(KILL"player %i is dead%s\n", my_index, RESET); //because we are zeroing it...
	return (NULL);
}

/* initialize the listening fd */
static int	host_player_init(t_player *player, char **env)
{
	struct	sockaddr_in serveraddr;
	int 	listfd;
	
	if ((listfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		return (perror(ERROR"socket failure"RESET), -1);
	
	ft_memset(&serveraddr, 0, sizeof(struct sockaddr_in));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons ( MYPORT );
	serveraddr.sin_addr.s_addr = htonl( INADDR_ANY );

	if (bind(listfd, (struct sockaddr *)&serveraddr, sizeof(struct sockaddr_in)))
		return (perror(ERROR"bind failure"RESET), -1);

	if (listen(listfd, 10))
		return(perror(ERROR"listen failure"RESET), -1);
	
	ft_printf(LISTEN"I'm listening...%s\n", RESET);

	//player[0] 'host' data init
	ft_memmove(&player[0].ip, get_serv_ip(env), 15);
	ft_memmove(&player[0].name, get_my_name(env), 10);
	//ft_strlcat(player[0].name, "_host", ft_strlen(player[0].name) + 6);
	player[0].num = 0;
	return(listfd);
}

int server_routine(t_player *player, int *id, char *argv[], char *env[])
{
	(void)id; (void)argv;

	int listfd;
	ft_printf("LOCAL_IP=%s, SERVER_IP=%s\n", get_locl_ip(env), get_serv_ip(env));
	
	//player init
	if ((listfd = host_player_init(&player[0], env)) < 0)
		return (1);

	player_specs(player[0]);

	t_recenv	*recenv;
	recenv = (t_recenv *)ft_calloc(1, sizeof(t_recenv));
	if (recenv == NULL)
		return (ft_printf("malloc failure\n"), 1);

	recenv->env = env;
	recenv->player = player;
	recenv->socket = listfd;
	recenv->index = 1; //next player index

	if (pthread_mutex_init(&recenv->player_mutex, NULL) != 0)
		perror(ERROR"mutex init failed"RESET);

	//reciever
	pthread_t	tid;
	if (pthread_create(&tid, NULL, &server_reciever, recenv) < 0)
		perror(ERROR"reciever launch failed"RESET);
	pthread_detach(tid);	//since if we died as server we don't need to wait the other listener

	//minigame(0, &player[0]);

	//pthread_join(tid, NULL);
	//close( sendfd );
	return (0);
}
