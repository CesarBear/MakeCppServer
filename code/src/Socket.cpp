#include "Socket.h"
#include "InetAddress.h"
#include "util.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>

Socket::Socket() : fd(-1)
{
    fd = socket( AF_INET, SOCK_STREAM, 0);
    errif( fd == -1, "socket create error");
}

Socket::Socket( int _fd ) : fd(_fd)
{
    errif( fd == -1, "socket create error");
}

Socket::~Socket()
{
    if( fd != -1)
    {
        close(fd);
        fd = -1;
    }
}












