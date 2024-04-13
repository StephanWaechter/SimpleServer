#pragma once
#include <boost/asio.hpp>
#include <iostream>

namespace sw::net
{
    class Connection : public std::enable_shared_from_this<Connection>
    {
        public:
            using pointer = std::shared_ptr<Connection>;
            static void create(boost::asio::ip::tcp::socket socket)
            {
                auto connection = std::shared_ptr<Connection>( new Connection(std::move(socket)) );
                connection->waitForRequest();
            }

        private:
            Connection(boost::asio::ip::tcp::socket socket) : mSocket(std::move(socket)) {}
            void waitForRequest()
            {
                boost::asio::async_read_until(
                    mSocket,
                    mBuffer,
                    '\n',
                    [self = shared_from_this()](
                        boost::system::error_code ec,
                        std::size_t /*length*/
                    )
                    {
                        if(!ec)
                        {
                            std::string data{
                                std::istreambuf_iterator<char>(&self->mBuffer), 
                                std::istreambuf_iterator<char>() 
                            };
                            std::cout << data;
                            self->respondToRequest();
                        }
                        else
                        {
                            std::cout << "error: " << ec.message() << std::endl;
                        }
                    }
                );
            }

            std::string buildResponse(std::string data)
            {
                std::stringstream ss;
                ss << "HTTP/1.1 200 OK\nContent-Length: " << data.size() << "\nConnection: Closed\n\n" << data;
                return ss.str();
            }

            void respondToRequest()
            {
                auto respond = buildResponse("Hello World!");
                std::cout << respond << std::endl;
                boost::asio::write(
                    mSocket,
                    boost::asio::buffer(respond.data(), respond.size())
                );
            }



            boost::asio::ip::tcp::socket mSocket;
            boost::asio::streambuf mBuffer;
    };
}