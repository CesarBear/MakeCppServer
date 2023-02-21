server:
	g++ -g code/src/util.cpp client.cpp -o client && \
	g++ -g code/src/util.cpp server.cpp code/src/Epoll.cpp code/src/InetAddress.cpp \
	code/src/Socket.cpp code/src/Channel.cpp code/src/EventLoop.cpp code/src/Server.cpp \
	code/src/Acceptor.cpp  code/src/Connection.cpp \
	-o server 

clean:
	rm server && rm client 