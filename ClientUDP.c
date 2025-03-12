/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientUDP.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 22:59:15 by topiana-          #+#    #+#             */
/*   Updated: 2025/03/12 21:24:42 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "socket_is_closed.h"

static int	update_player(const char *ip, const char *coords, t_player *player)
{
	char	**split;

	printf("updating player %s\n", ip);
	if (ip == NULL)
	{
		perror( "bad ip" );
		return (0);
	}
	split = ft_split(coords, '-');
	if (split == NULL)
	{
		perror( "bad coords" );
		ft_freentf("2", split);
		return (0);
	}
	player->pos.x = ft_atoi(split[0]);
	player->pos.y = ft_atoi(split[1]);
	player->pos.z = ft_atoi(split[2]);
	ft_freentf("2", split);
	return (1);
}

/* moves the server's player 'player[0]'. 
NOTE: the 'player[1]' is moved by the minigame. */
static int	handle_players(const char *buffer, t_recenv *recenv)
{
	char	**split;

	split = ft_split(buffer, ':');
	if (split == NULL || recenv == NULL)
		return (0);
	if (!update_player(split[0], split[1], &recenv->player[0]))
		return (0);
	printf("%s at %f\n", recenv->player[0].ip, recenv->player[0].pos.x);
	printf("%s at %f\n", recenv->player[1].ip, recenv->player[1].pos.x);
	ft_freentf("2", split);
	return (1);
}

/* static  */void	*client_reciever(void *arg)
{
	t_recenv	*recenv;

	recenv = (t_recenv *)arg;
	int recvfd;
    if ( (recvfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 )
	{
        perror( "reciever socket failed" );
        return (NULL);
    }
	printf("recvfd=%i\n", recvfd);

    struct sockaddr_in recvaddr;
    memset( &recvaddr, 0, sizeof(recvaddr) );
    recvaddr.sin_family = AF_INET;
    recvaddr.sin_port = htons( 50037 );
	recvaddr.sin_addr.s_addr = htonl( (uint32_t)ip_to_uns(get_serv_ip(recenv->env)) ); //client setup

	//binding reciever socket to server address
	if (1/* ft_strncmp("127.0.0.1", get_serv_ip(recenv->env), 9) != 0 */)
	{
		if ( bind(recvfd, (struct sockaddr *)&recvaddr, sizeof(recvaddr)) < 0 )
		{
			perror( "reciever bind failed" );
			return (NULL);
		}
	}

    struct sockaddr_in addrin;
    socklen_t addr_len = sizeof(struct sockaddr_in);
	
    char buffer[MAXLINE];
    while ( 1 )
	{
		ft_printf("listening...\n");
        int length = recvfrom( recvfd, buffer, sizeof(buffer) - 1, 0, (struct sockaddr *)&addrin, &addr_len );
        if ( length < 0 ) {
			perror( "recvfrom failed" );
            break;
        }
		printf( "%d bytes: '%s' from %s\n", length, buffer, inet_ntoa(addrin.sin_addr));
		handle_players(buffer, recenv);
	}
	close(recvfd);
	return (NULL);
}

static void	client_player_init(t_player *player, char **env)
{
	int sendfd;
	if ( (sendfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
		perror("socket failed");
		return ;
	}
	printf("sendfd=%i\n", sendfd);
	
	//serveraddr setup (bind to socket to send message to the server)
	struct sockaddr_in serveraddr;
	memset( &serveraddr, 0, sizeof(serveraddr) );
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons( 50037 );              
	serveraddr.sin_addr.s_addr = htonl( (uint32_t)ip_to_uns(get_serv_ip(env)) );
	
	//send test
	if (sendto( sendfd, get_locl_ip(env), 15, 0, (struct sockaddr *)&serveraddr, sizeof(struct sockaddr_in)) < 0 )
		perror( "sendto failed" );
		
	memmove(&player->sockaddr, &serveraddr, sizeof(struct sockaddr_in));
	memmove(&player->ip, get_locl_ip(env), 15);
	memmove(&player->name, "pippo", 5);
	player->socket = sendfd;
	player->num = 1;
}

int client_routine( int argc, char *argv[], char *env[])
{
	(void)argc; (void)argv;

	ft_printf("LOCAL_IP=%s, SERVER_IP=%s\n", get_locl_ip(env), get_serv_ip(env));

	//player[0] = server, player[1] = client
	t_player player[2];
	memset(&player, 0, 2 * sizeof(t_player));
	client_player_init(&player[1], env);

	player_specs(player[1]);

	t_recenv	recenv;
	
	recenv.env = env;
	recenv.player = &player[0];
	recenv.max_players = 2;
	
    //reciever
	pthread_t	tid;
	if (pthread_create(&tid, NULL, &client_reciever, &recenv) < 0)
		perror( "reciever launch failed" );

    minigame(1, &player[0]);

	pthread_join(tid, NULL);
	return (0);
}
