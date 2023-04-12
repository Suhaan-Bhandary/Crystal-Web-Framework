#include <sys/socket.h>
#include <unistd.h>

namespace http {
class Router {
   private:
   public:
    Router();
    void route(int &client_socket);
};
}  // namespace http
