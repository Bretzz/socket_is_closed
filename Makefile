# define standard colors
BOLD			= \033[1m
BLACK    		= \033[30m    # Black
RED      		= \033[31m    # Red
GREEN    		= \033[32m    # Green
YELLOW   		= \033[33m    # Yellow
BLUE     		= \033[34m    # Blue
MAGENTA  		= \033[35m    # Magenta
CYAN     		= \033[36m    # Cyan
WHITE    		= \033[37m    # White
LIGHT_RED		= \033[91m
LIGHT_GREEN		= \033[92m
LIGHT_CYAN		= \033[96m
RESET			= \033[0m

NAME			:= game
SERVER 			:= ServerUDP.c
CLIENT 			:= ClientUDP.c
UNAME			:= $(shell uname)
CC				:= cc
CFLAGS			:= -Wall -Wextra -Werror

SCRIPT			= config.sh

#Libs
LIBFT_DIR		:= libft
LIBFT			= $(LIBFT_DIR)/libft.a

#Linkers
LINKS			= -L/usr/lib -L$(MLX_DIR) -lXext -lX11 -lm -lz -lpthread

#Includes
INKS			= -I$(CURDIR) -I$(LIBFT_DIR) -I$(MLX_DIR) -O3

#source files (expected in the root folder)
SRCS_DIR		=
SRC_FILES		= main.c \
				ClientUDP.c ServerUDP.c \
				minigame.c ft_freentf.c convert_stuff.c inet_stuff.c
SRCS			= $(addprefix $(SRCS_DIR), $(SRC_FILES))

# Objects
OBJS_DIR		= obj/
OBJ_FILES		= $(SRCS:.c=.o)
OBJS			= $(addprefix $(OBJS_DIR), $(OBJ_FILES))

VPATH 			= src

ifeq ($(UNAME),Darwin)
	MLX_DIR		= minilibx_opengl
	MLX			= $(MLX_DIR)/libmlx.a
	URL			= https://cdn.intra.42.fr/document/document/31539/minilibx_opengl.tgz
	DEFS		=
	INKS		+= -I/usr/X11/include -I$(MLX_DIR)
	LINKS		+= -I/opt/homebrew/include -I/usr/X11/include -L/usr/X11/lib -framework OpenGL -framework AppKit
else ifeq ($(UNAME),Linux)
	MLX_DIR		= minilibx-linux
	MLX			= $(MLX_DIR)/libmlx_$(UNAME).a
	URL			= https://cdn.intra.42.fr/document/document/31538/minilibx-linux.tgz
	DEFS		=
	INKS		+= -I/usr/include
	LINKS		+= -lmlx_Linux -I$(MLX_DIR)
else
	UNAME = Error
endif

all: $(NAME)

$(OBJS_DIR):
	@mkdir -p $(OBJS_DIR)

$(OBJS_DIR)%.o: $(SRCS_DIR)%.c | $(OBJS_DIR) 
	@$(CC) $(CFLAGS) $(INKS) $(DEFS) -c $< -o $@

$(MLX_DIR):
	@echo "${BOLD}creating $(MLX_DIR)...${RESET}"
	@curl $(URL) --output $(MLX_DIR).tgz \
	&& tar -xf $(MLX_DIR).tgz \
	&& rm -f $(MLX_DIR).tgz \
	&& ls $(MLX_DIR) || mv `ls | grep $(MLX_DIR)` $(MLX_DIR) \
	&& $(MAKE) -C $(MLX_DIR)

$(MLX): $(MLX_DIR)
	@rm -f $(MLX_DIR)/libmlx.a
	@$(MAKE) -C $(MLX_DIR) --quiet

$(LIBFT):
	@echo "${BOLD}creating libft...${RESET}"
	@$(MAKE) -C $(LIBFT_DIR) --quiet

minigame: minigame.c $(MLX) $(LIBFT)
	@echo "${BOLD}compiling minigame...${RESET}"
	@$(CC) $(CFLAGS) minigame.c $(MLX) $(LIBFT) $(LINKS) $(INKS) -o minigame \
	&& echo "${LIGHT_GREEN}DONE${RESET}"

$(NAME): $(MLX) $(LIBFT) $(OBJS)
	@echo "${BOLD}compiling the $(NAME)...${RESET}"
	@$(CC) $(CFLAGS) $(OBJS_DIR)* $(MLX) $(LIBFT) $(LINKS) -o $(NAME) \
	&& echo "${LIGHT_GREEN}DONE${RESET}"

miniserver: miniserver.c
	@echo "${BOLD}compiling miniserver...${RESET}"
	@$(CC) $(CFLAGS) miniserver.c $(LINKS) $(INKS) -o miniserver \
	&& echo "${LIGHT_GREEN}DONE${RESET}"

miniclient: miniclient.c
	@echo "${BOLD}compiling miniclient...${RESET}"
	@$(CC) $(CFLAGS) miniclient.c $(LINKS) $(INKS) -o miniclient \
	&& echo "${LIGHT_GREEN}DONE${RESET}"

tar:
	@ls | grep -q "$(NAME).tar" && rm -f $(NAME).tar || true
	@tar -cf $(NAME).tar --exclude=".git" --exclude="$(NAME)" --exclude="$(OBJS_DIR)" --exclude="$(MLX_DIR)" ./*

clean:
	@rm -f client server $(OBJS_DIR)* game minigame
fclean: clean
	@rm -rf $(MLX) $(LIBFT) $(OBJS_DIR)

lre: clean all

re: fclean all