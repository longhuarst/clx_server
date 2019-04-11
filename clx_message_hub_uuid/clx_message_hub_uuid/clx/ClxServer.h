#pragma once



#include <boost/asio.hpp>




class ClxServer
{
public:
	ClxServer(boost::asio::io_context& io_context, short port);
	~ClxServer();



private:

	void do_accept();


	boost::asio::ip::tcp::acceptor acceptor_;


};

