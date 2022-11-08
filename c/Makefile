OBJS = server

all: $(OBJS)

server:
	gcc -Wall -D_DEBUG http/httpd.c http/net.c server.c -lpthread -o server

clean:
	rm $(OBJS)
