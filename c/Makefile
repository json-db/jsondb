LIBS = http/httpd.c http/net.c docDB.c
OBJS = docDBServer
OPTS = -Wall -D_DEBUG

all: $(OBJS)

docDBServer:
	gcc $(OPTS) $(LIBS) docDBServer.c -lpthread -o docDBServer

clean:
	rm $(OBJS)
