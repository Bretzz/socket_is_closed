/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientUDP.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: totommi <totommi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 22:59:15 by topiana-          #+#    #+#             */
/*   Updated: 2025/03/17 10:38:41 by totommi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "socket_is_closed.h"

static void	*client_reciever(void *arg)
{
	t_recenv	*recenv;
	char		buffer[MAXLINE];
	int			len;

	recenv = (t_recenv *)arg;
	while ( 1 )
	{
		ft_memset(buffer, 0, MAXLINE);
		ft_printf("talk to me...\n");
		len = recv( recenv->player[0].socket, buffer, MAXLINE - 1, 0 );
		if ( len < 0 ) {
			perror(ERROR"recv failed"RESET);
			break;
		}
		ft_printf(YELLOW"%d bytes: '%s' from Server\n"RESET, len, buffer);
		if (handle_client_players(buffer, recenv) < 0)
		{
			ft_printf(HOST"A NEW HOST WILL RISE%s\n", RESET);
			return (close(recenv->player[0].socket), free(recenv), NULL);
		}
	}
	close(recenv->player[0].socket);
	free(recenv);
	return (NULL);
}

static int client_player_init(t_player *player, char **env)
{
	int		servfd;
	char	buffer[MAXLINE];
	
	if ((servfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror(ERROR"socket failure"RESET);
		return (0);
	}
	
	struct sockaddr_in serveraddr;

	ft_memset(&serveraddr, 0, sizeof(struct sockaddr_in));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons ( MYPORT );
	serveraddr.sin_addr.s_addr = inet_addr(get_serv_ip(env)); //192.168.1.5 //INADDR_ANY

	if (connect(servfd, (struct sockaddr *)&serveraddr, sizeof(struct sockaddr)))
	{
		perror(ERROR"connection failed"RESET);
		return (0);
	}
	ft_printf(GREEN"connection accepted!!!\n"RESET);

	//player signature name:ip
	ft_memset(buffer, 0, 30);
	ft_strlcpy(buffer, get_my_name(env), 11);
	ft_strlcat(buffer, ":", ft_strlen(buffer) + 2);
	ft_strlcat(buffer, get_locl_ip(env), ft_strlen(buffer) + 16);
	
	//send test
	if (send(servfd, buffer, 30, 0) < 0)
		perror(ERROR"send failure"RESET);

	//recieving server data name:ip (ip defaults to 'host')
	if (recv(servfd, buffer, MAXLINE - 1, 0) < 0)
		perror(ERROR"recv failed"RESET);

	//host init
	ft_memmove(&player[0].sockaddr, &serveraddr, sizeof(struct sockaddr_in));
	ft_memmove(&player[0].name, buffer, ft_strichr(buffer, ':') - 1);
	ft_memmove(&player[0].ip, "host", 4);
	player[0].socket = servfd;
	//player init
	ft_memmove(&player[1].name, get_my_name(env), 10);
	ft_memmove(&player[1].ip, get_locl_ip(env), 15);
	player[1].num = 1;
	return (1);
}

//0 ok, 1 error
int client_routine(t_player *player, int *id, char *argv[], char *env[])
{
	(void)argv;
	//player[0] = server, player[1] = client

	ft_printf("LOCAL_IP=%s, SERVER_IP=%s\n", get_locl_ip(env), get_serv_ip(env));

	//initialize the data to connect to the server
	if (!client_player_init(&player[0], env))
		return (1);

	ft_printf("===  HOST  ===\n");
	player_specs(player[0]);
	ft_printf("=== PLAYER ===\n");
	player_specs(player[1]);
	ft_printf("= == == == = =\n");

	t_recenv	*recenv;
	recenv = (t_recenv *)ft_calloc(1, sizeof(t_recenv));
	if (recenv == NULL)
		return (ft_printf("malloc failure\n"), 1);
	
	recenv->env = env;
	recenv->id = id;
	recenv->player = &player[0];
	
	//reciever
	pthread_t	tid;
	if (pthread_create(&tid, NULL, &client_reciever, recenv) < 0)
		perror(ERROR"reciever launch failed"RESET);
	pthread_detach(tid);

	//minigame(1, &player[0]);

	//pthread_join(tid, NULL);
	return (0);
}
