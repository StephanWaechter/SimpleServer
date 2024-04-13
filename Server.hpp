#pragma once
#include <boost/asio.hpp>
#include <iostream>

#include "Connection.hpp"

namespace sw::net
{
    class Server
    {
        public:
            Server(boost::asio::ip::tcp::endpoint ep)
            : mConntext{1}, mAcceptor{mConntext, ep}
            {}
            
            void start()
            {
                accept();
                mConntext.run();
            }

        private:
            void accept()
            {
                mAcceptor.async_accept(
                    [this](
                        boost::system::error_code ec,
                        boost::asio::ip::tcp::socket socket
                    )
                    {
                        if (!ec) 
                        {
                            // let's see where we created our session
                            std::cout << "creating session on: " 
                                << socket.remote_endpoint().address().to_string() 
                                << ":" << socket.remote_endpoint().port() << '\n';
                            Connection::create(std::move(socket));
                        }
                        else 
                        {
                            std::cout << "error: " << ec.message() << std::endl;
                        }
                        
                        accept();
                    });
            }

            boost::asio::io_context mConntext;
            boost::asio::ip::tcp::acceptor mAcceptor;
            Connection::pointer mConnection;
            
    };
}