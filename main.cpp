#include "Server.hpp"



int main(int /* argc */, char** /* argv */)
{
    sw::net::Server server({boost::asio::ip::tcp::v4(), 25000});
    server.start();
}