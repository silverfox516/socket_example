CC = gcc
CFLAGS = -lpthread

SERVER_OBJS = server.o
CLIENT_OBJS = client.o

all: server client

server: $(SERVER_OBJS)
	$(CC) -o $@ $(SERVER_OBJS) $(CFLAGS)

client: $(CLIENT_OBJS)
	$(CC) -o $@ $(CLIENT_OBJS) $(CFLAGS)

clean:
	@rm server client *.o
