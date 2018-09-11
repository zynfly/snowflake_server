//
// Created by zynfly on 2018/9/5.
//

#include "Server.h"
#include <vector>
#include <thread>

Server::Server(
        uint32_t max_connection,
        uint32_t thread_num,
        std::string const &address,
        std::string const &port,
        nlohmann::json config
) :
        _thread_num(thread_num),
        _io_context(thread_num),
        _acceptor(_io_context),
        _connection_manager(),
        _handle(config)
{
    boost::asio::ip::tcp::resolver resolver(_io_context);
    boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve(address, port).begin();
    _acceptor.open(endpoint.protocol());
    _acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
    _acceptor.bind(endpoint);
    _acceptor.listen(max_connection);

    doAccept();
}

void Server::run()
{
    std::vector<std::thread> v_thread;

    for (int i = 0; i < _thread_num; ++i) {
        v_thread.emplace_back(
                [this]() {
                    _io_context.run();
                }
        );
    }

    for (auto &&t : v_thread) {
        t.join();
    }
}

void Server::doAccept()
{
    _acceptor.async_accept(
            [this](boost::system::error_code ec, boost::asio::ip::tcp::socket socket) {
                if (!_acceptor.is_open()) {
                    return;
                }

                doAccept();
                if (!ec) {
                    _connection_manager.start(
                            std::make_shared<Connection>(
                                    std::move(socket),
                                    _connection_manager,
                                    _handle
                            )
                    );
                }
            }
    );
}

