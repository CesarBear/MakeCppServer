#include "code/src/util.h"
#include "code/src/Socket.h"
#include "code/src/Buffer.h"

#include <unistd.h>
#include <string.h>


int main() {
    Socket *clnt_sock = new Socket();
    InetAddress *clnt_addr = new InetAddress("127.0.0.1", 8888);
    clnt_sock->connect(clnt_addr);

    int sockfd = clnt_sock->getFd();

    Buffer *sendBuffer = new Buffer();
    Buffer *readBuffer = new Buffer();

    while (true) {
        sendBuffer->getline();
        ssize_t write_bytes = write(sockfd, sendBuffer->c_str(), sendBuffer->size());
        if (write_bytes == -1) {
            printf("socket already disconnected, can't write any more!\n");
            break;
        }

        int already_read = 0;
        char buf[1024];

        while (true) {
            bzero(&buf, sizeof(buf));
            ssize_t read_bytes = read(sockfd, buf, sizeof(buf));

            if (read_bytes > 0) {
                readBuffer->append(buf, read_bytes);
                already_read += read_bytes;
            } else if (read_bytes == 0) {
                printf("server disconnected!\n");
                exit(EXIT_SUCCESS);
            }

            if (already_read >= sendBuffer->size()) {
                printf("message from server: %s\n", readBuffer->c_str());
                break;
            }
        }

        readBuffer->clear();
    }

    delete clnt_addr;
    delete clnt_sock;
    return 0;
}
