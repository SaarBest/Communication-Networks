CC = gcc
SERVER_OBJS = server.o utils.o
CLIENT_OBJS = client.o utils.o
EXEC_SERVER = seker_server
EXEC_CLIENT = seker_client
FLAGS = -std=c99 -Wall -g

all: $(EXEC_SERVER) $(EXEC_CLIENT)
$(EXEC_SERVER): $(SERVER_OBJS)
	$(CC) $(SERVER_OBJS) -o $@
$(EXEC_CLIENT): $(CLIENT_OBJS)
	$(CC) $(CLIENT_OBJS) -o $@
utils.o: utils.c utils.h
	$(CC) $(COMP_FLAG) -c utils.c
server.o: server.c server.h utils.h
	$(CC) $(COMP_FLAG) -c server.c
client.o: client.c client.h utils.h
	$(CC) $(COMP_FLAG) -c client.c
clean:
	rm -f $(SERVER_OBJS) $(CLIENT_OBJS) $(EXEC_SERVER) $(EXEC_CLIENT)