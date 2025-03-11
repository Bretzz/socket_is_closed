SERVER = ServerUDP.c
CLIENT = ClientUDP.c

all:
	gcc -o server $(SERVER)
#	./client_setup.sh
	gcc -o client $(CLIENT)

clean:

fclean:
	rm -f client server

re: fclean all