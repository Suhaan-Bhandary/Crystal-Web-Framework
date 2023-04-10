// The ifndef is used so that the same File is not loaded again
#ifndef INCLUDED_HTTP_TCPSERVER_LINUX
#define INCLUDED_HTTP_TCPSERVER_LINUX

#include <arpa/inet.h>
#include <sys/socket.h>

#include <string>

namespace http {
class TcpServer {
   private:
    int server_socket;
    const int server_port;
    const std::string server_ip_address;
    struct sockaddr_in server_socketAddress;

   public:
    TcpServer(const std::string &ip_address, int port);
    ~TcpServer();

    int startServer();
    void closeServer();
    void startListen();
};
}  // namespace http
#endif
