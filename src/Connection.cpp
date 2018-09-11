//
// Created by zynfly on 2018/9/5.
//

#include "Connection.h"
#include "ConnectionManager.h"


Connection::Connection(boost::asio::ip::tcp::socket socket, ConnectionManager &manager, Handle &handle) :
        _socket(std::move(socket)),
        _connection_manager(manager),
        _handle(handle)
{

}

void Connection::start()
{
    doRead();
}

void Connection::stop()
{
    _socket.close();
}

void Connection::doRead()
{
    auto self = shared_from_this();
    _socket.async_read_some(
            boost::asio::buffer(_buffer),
            [self, this](boost::system::error_code ec, std::size_t bytes_transferred) {
                if (!ec) {
                    RequestParser::result_type result;
                    std::tie(result, std::ignore) = _parser.parse(_request, _buffer.begin(), _buffer.end());
                    if (RequestParser::good == result) {
                        boost::system::error_code rep_ec;
                        _request.remote_endpoint = _socket.remote_endpoint(rep_ec);
                        if (!rep_ec) {
                            _handle.handleRequest(_request, _reply);
                            doWrite();
                        }
                    } else if (RequestParser::bad == result) {
                        _reply = Reply::stockReply(Reply::bad_request);
                        doWrite();
                    } else {
                        doRead();
                    }
                } else if (ec != boost::asio::error::operation_aborted) {
                    _connection_manager.stop(self);
                }
            }
    );
}

void Connection::doWrite()
{
    auto self = shared_from_this();
    _socket.async_write_some(
            _reply.to_buffers(),
            [self, this](boost::system::error_code ec, std::size_t bytes_transferred) {
                if (!ec) {
                    boost::system::error_code ignored_ec;
                    _socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
                }

                if (boost::asio::error::operation_aborted != ec) {
                    _connection_manager.stop(self);
                }
            }
    );

}
