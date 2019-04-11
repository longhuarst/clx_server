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
	//boost::asio::ip::tcp::socket *socket_self_;//�Լ�
	//boost::asio::ip::tcp::socket *socket_peer_;//�Եȶ�socket
	//ClxSession * session_peer_;//�Եȶ˵�session
	
	
	
	enum {max_length = 1024};
	char data_[max_length];
	char data_send_[max_length];//���͵�����
	boost::asio::streambuf outputstream_;
	boost::asio::streambuf inputstream_;



	bool inited_;//uuid�Ƿ���֤���


	std::string uuid_;//�ÿͻ���UUID
	std::string uuid_peer_;//�Եȵ�UUID


};

