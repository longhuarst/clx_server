#pragma once

#include <iostream>

#include <boost/asio.hpp>

class ClxSession 
	: public std::enable_shared_from_this<ClxSession>
{
public:
	ClxSession(boost::asio::ip::tcp::socket socket);
	~ClxSession();


	void start();

private:



	void do_read();

	void do_write(std::size_t length);




	boost::asio::ip::tcp::socket socket_;
	boost::asio::ip::tcp::socket *socket_peer_;//对等端socket

	enum {max_length = 1024};
	char data_[max_length];
	char data_send_[max_length];//发送的数据
	boost::asio::streambuf outputstream_;
	boost::asio::streambuf inputstream_;

	bool inited_;//uuid是否验证完毕


	std::string uuid_;//该客户的UUID
	std::string uuid_peer_;//对等的UUID


};

