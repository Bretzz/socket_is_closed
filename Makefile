SERVER = ServerUDP.c
CLIENT = ClientUDP.c
SCRIPT = config.sh

all: client server

client: $(CLIENT)
	./$(SCRIPT)
	gcc -o client $(CLIENT)

server: $(SERVER)
	gcc -o server $(SERVER)

clean:

fclean:
	rm -f client server

re: fclean all