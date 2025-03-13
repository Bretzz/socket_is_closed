/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientUDP_mixed.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: totommi <totommi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 22:59:15 by topiana-          #+#    #+#             */
/*   Updated: 2025/03/13 18:33:04 by totommi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "socket_is_closed.h"

/*check for the value 'c' in the string 's',
if found returns the pos (index + 1) of the first instance of 'c'*/
/* static int	ft_strichr(const char *s, int c)
{
	unsigned int	i;

	c = (unsigned char)c;
	i = 0;
	while (s[i] != '\0' && s[i] != c)
		i++;
	if (s[i] == '\0' && c != '\0')
		return (0);
	return (i + 1);
} */

/* returns a pointer to the value of the IP var in the env */
static char	*get_locl_ip(char **env)
{
	int	i;

	if (env == NULL)
		return (NULL);
	i = 0;
	while (env[i] != NULL)
	{
		if (!ft_strncmp("LOCAL_IP=", env[i], 9))
			return (env[i] + 9);
		i++;
	}
	return ("ip not found");
}

static char	*get_serv_ip(char **env)
{
	int	i;

	if (env == NULL)
		return (NULL);
	i = 0;
	while (env[i] != NULL)
	{
		if (!ft_strncmp("SERVER_IP=", env[i], 10))
			return (env[i] + 10);
		i++;
	}
	return ("ip not found");
}

static void	update_player(const char *ip, const char *coords, t_player *player)
{
	char	**split;

	printf("updating player %s\n", ip);
	if (ip == NULL)
	{
		perror( "bad ip" );
		return ;
	}
	split = ft_split(coords, '-');
	if (split == NULL)
	{
		ft_freentf("2", split);
		perror( "bad coords" );
		return ;
	}
	player->pos.x = ft_atoi(split[0]);
	player->pos.y = ft_atoi(split[1]);
	player->pos.z = ft_atoi(split[2]);
	ft_freentf("2", split);
}

static void	register_player(int socket, const char *ip, const char *coords, t_player *player)
{
	char	**split;

	printf("registering new player...\n");
	if (ip == NULL)
	{
		perror( "bad ip" );
		return ;
	}
	memmove(player->ip, ip, 15);
	player->socket = socket;

	struct sockaddr_in playeraddr;
	memset( &playeraddr, 0, sizeof(playeraddr) );
	
    playeraddr.sin_family = AF_INET;
    playeraddr.sin_port = htons( 50037 );              
    playeraddr.sin_addr.s_addr = htonl( (uint32_t)ip_to_uns(player->ip) );

	//binding the out-socket to the player/server addr
	if ( bind(socket, (struct sockaddr *)&playeraddr, sizeof(playeraddr)) < 0 )
	{
		perror( "sender bind failed" );
		return ;
	}

	split = ft_split(coords, '-');
	if (split == NULL)
	{
		ft_freentf("2", split);
		perror( "bad coords" );
		return ;
	}
	player->pos.x = ft_atoi(split[0]);
	player->pos.y = ft_atoi(split[1]);
	player->pos.z = ft_atoi(split[2]);
	ft_freentf("2", split);
	printf("new player registered");
}

static int	handle_players(const char *buffer, t_recenv *recenv)
{
	unsigned int	i;
	int				found;
	char			**split;

	split = ft_split(buffer, ':');
	if (split == NULL || recenv == NULL)
	return (0);
	found = 0;
	i = 0;
	while (i < recenv->max_players)
	{
		printf("handling player: %s\n", recenv->player[i].ip);
		if (recenv->player[i].ip[0] != '\0'
			&& !ft_strncmp(recenv->player[i].ip, split[0], 15))
		{
			update_player(split[0], split[1], &recenv->player[i]);
			found = 1;
		}
		else if (recenv->player[i].ip[0] == '\0' && found == 0)
			register_player(recenv->socket_out, split[0], split[1], &recenv->player[i]);
		i++;
	}
	printf("current players: %i\n", i);
	if (i >= 0)
		printf("%s at %f\n", recenv->player[0].ip, recenv->player[0].pos.x);
	if (i >= 1)
		printf("%s at %f\n", recenv->player[1].ip, recenv->player[1].pos.x);
	ft_freentf("2", split);
	return (1);
}

void	*reciever(void *arg)
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
	if (!ft_strncmp("host", get_serv_ip(recenv->env), 4))
		recvaddr.sin_addr.s_addr = htonl( INADDR_ANY ); //host setup
	else
		recvaddr.sin_addr.s_addr = htonl( (uint32_t)ip_to_uns(get_serv_ip(recenv->env)) ); //client setup

		
	if ( bind(recvfd, (struct sockaddr *)&recvaddr, sizeof(recvaddr)) < 0 )
	{
		perror( "reciever bind failed" );
		return (NULL);
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
	return (NULL);
}

static void	client_player_init(t_player *player, int fd, struct sockaddr_in *serveraddr, char **env)
{
	//serveraddr setup (bind to socket to send message to the server)
	serveraddr->sin_family = AF_INET;
	serveraddr->sin_port = htons( 50037 );              
	serveraddr->sin_addr.s_addr = htonl( (uint32_t)ip_to_uns(get_serv_ip(env)) );
	memmove(&player->sockaddr, serveraddr, sizeof(struct sockaddr_in));
	memmove(&player->ip, get_locl_ip(env), 15);
	memmove(&player->name, "pippo", 5);
	player->socket = fd;
	printf("sendfd=%i\n", fd);
}

static void	host_player_init(t_player *player, int fd, struct sockaddr_in *serveraddr, char **env)
{
	(void)serveraddr; (void)env;
	memmove(&player->ip, "host", 4);
	memmove(&player->name, "pippo host", 10);
	printf("sendfd=%i\n", fd);
}

int main( int argc, char *argv[], char *env[] )
{
	(void)argc; (void)argv;

	ft_printf("LOCAL_IP=%s, SERVER_IP=%s\n", get_locl_ip(env), get_serv_ip(env));

	int sendfd;
	if ( (sendfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
		perror("socket failed");
		return (1);
	}

    struct sockaddr_in serveraddr;
    memset( &serveraddr, 0, sizeof(serveraddr) );
    /* serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons( 50037 );              
    serveraddr.sin_addr.s_addr = htonl( (uint32_t)ip_to_uns(get_serv_ip(env)) );  //my_pc=0x0a0c0307 //loclhost=0x7f000001 */

	//ip_to_uns("127.0.0.1");

	t_player player[2];
	memset(&player, 0, 2 * sizeof(t_player));
	
	//player init
	if (!ft_strncmp("host", get_serv_ip(env), 4))
		host_player_init(&player[0], sendfd, &serveraddr, env);
	else
	{
		client_player_init(&player[0], sendfd, &serveraddr, env);

		//binding the client out-socket to the server addr
		/* if ( bind(sendfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0 )
		{
			perror( "reciever bind failed" );
			return (1);
		} */

		if (sendto( player[0].socket, get_locl_ip(env), 21, 0, (struct sockaddr *)&player[0].sockaddr, sizeof(struct sockaddr_in)) < 0 )
			perror( "sendto failed" );
	}

	pthread_t	tid;
	t_recenv	recenv;

	recenv.env = env;
	recenv.player = &player[0];
	recenv.max_players = 2;
	recenv.socket_out = sendfd;

    //reciever
	if (pthread_create(&tid, NULL, &reciever, &recenv) < 0)
		perror( "reciever launch failed" );

    minigame(&player[0]);

	pthread_join(tid, NULL);
    close( sendfd );
}
