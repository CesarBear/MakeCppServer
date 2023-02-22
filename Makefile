server:
	g++ -g code/src/util.cpp code/src/Buffer.cpp code/src/Socket.cpp code/src/InetAddress.cpp client.cpp -o client && \
	g++ -g code/src/util.cpp server.cpp code/src/Epoll.cpp code/src/InetAddress.cpp \
	code/src/Socket.cpp code/src/Channel.cpp code/src/EventLoop.cpp code/src/Server.cpp \
	code/src/Acceptor.cpp  code/src/Connection.cpp code/src/Buffer.cpp \
	-o server 

clean:
	rm server && rm client 