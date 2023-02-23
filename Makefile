src=$(wildcard code/src/*.cpp)

server:
	g++ -std=c++11 -pthread -g \
	$(src) \
	server.cpp \
	-o server
	
client:
	g++ code/src/util.cpp code/src/Buffer.cpp code/src/Socket.cpp code/src/InetAddress.cpp client.cpp -o client

th:
	g++ -pthread code/src/ThreadPool.cpp ThreadPoolTest.cpp -o ThreadPoolTest

test:
	g++ code/src/util.cpp code/src/Buffer.cpp code/src/Socket.cpp code/src/InetAddress.cpp code/src/ThreadPool.cpp \
	-pthread \
	test.cpp -o test

clean:
	rm server && rm client && rm test