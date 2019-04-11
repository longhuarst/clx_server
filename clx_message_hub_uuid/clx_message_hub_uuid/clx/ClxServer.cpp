#include "ClxServer.h"

#include "ClxSession.h"


ClxServer::ClxServer(boost::asio::io_context & io_context, short port)
	: acceptor_(io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
{
	do_accept();
}


ClxServer::~ClxServer()
{



}

void ClxServer::do_accept()
{
	acceptor_.async_accept([this](boost::system::error_code ec, boost::asio::ip::tcp::socket socket) {
		if (!ec) {

			std::cout << "[accept] - " << socket.remote_endpoint().address().to_string()  << "(" << socket.remote_endpoint().port() << ")" << std::endl;


			std::make_shared<ClxSession>(std::move(socket))->start();
		}
		do_accept();
		});
}
