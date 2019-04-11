#include "ClxSession.h"

//#include <utility>
//#include <cstdlib>
//#include <iostream>
//#include <memory>

#include <boost/regex.hpp>

#include <boost/algorithm/string.hpp>

#include "ClxIdTable.h"


ClxSession::ClxSession(boost::asio::ip::tcp::socket socket)
	:socket_(std::move(socket)),
	inited_(false),
	socket_peer_(NULL)
{
}


ClxSession::~ClxSession()
{
}

void ClxSession::start()
{
	do_read();
}

void ClxSession::do_read()
{
	auto self(shared_from_this());
	


	if (!inited_) {
		boost::asio::async_read_until(socket_,
			inputstream_,
			boost::regex("\r\n"),
			[this, self](boost::system::error_code ec, std::size_t length) {
				if (!ec) {
				
						//还未收到UUID注入
						std::istream is(&inputstream_);
						std::cout << "输出前缓存长度 " << inputstream_.size()<<std::endl;
						is >> data_;
						std::cout << "输出后缓存长度 " << inputstream_.size() << std::endl;
						inputstream_.consume(2);//消耗两个分隔符

						std::cout << "读取到数据 - 长度 "<< length<<"[" <<data_<<"]" << std::endl;

						//解析注入帧
						//****connect****_[UUID接入端]_[UUID对等端]_****connect****
					
						std::vector<std::string> res;

						boost::split(res, data_, boost::is_any_of("_"), boost::token_compress_on);

						if (res.size() == 4) {
							if (res[0] == "****connect****" && res[3] == "****connect****") {
								std::cout << "ID帧正确" << std::endl;

								uuid_ = res[1];
								uuid_peer_ = res[2];

								ClxIdTableInstance::get_mutable_instance().insert(uuid_, socket_);//注入到ID表中
								 
								//查询目标端

								
								socket_peer_ = ClxIdTableInstance::get_mutable_instance().get_socket_by_id(uuid_peer_);

								if (socket_peer_ != NULL) {
									std::cout << "建立连接成功！" << std::endl;
								}
								else {
									std::cout << "连接建立失败，等待对方上线！" << std::endl;
								}



								inited_ = true;
								do_read();
								
								std::cout << "开始读取" << std::endl;
							}
							else {
								std::cout << "ID帧不正确" << std::endl;
							}
						}
						else {
							//关闭连接
							socket_.close();
							return;
						}

						//return;
				
					//do_write(length);
				}
			}
		);

	}
	else {


		socket_.async_read_some(
			boost::asio::buffer(data_, max_length),
			[this, self](boost::system::error_code ec, std::size_t length) {
				if (!ec) {
					std::cout << "读取到数据... 长度 "<< length << std::endl;
					if (socket_peer_ != NULL) {

						do_write(length);
					}
					else {

						socket_peer_ = ClxIdTableInstance::get_mutable_instance().get_socket_by_id(uuid_peer_);

						if (socket_peer_ != NULL) {
							std::cout << "建立连接成功！" << std::endl;

							do_write(length);
						}
						else {
							std::cout << "连接建立失败，等待对方上线！" << std::endl;
						}


						do_read();
					}



				}
			});





		
		
	}

	/*socket_.async_read_some(boost::asio::buffer(data_, max_length),
		[this, self](boost::system::error_code ec, std::size_t length) {
			if (!ec) {
				do_write(length);
			}
		});*/
}

void ClxSession::do_write(std::size_t length)
{
	auto self(shared_from_this());



	memcpy(data_send_, data_, length);



	boost::asio::async_write(*socket_peer_,boost::asio::buffer(data_send_,length),
		[this, self](boost::system::error_code ec, std::size_t /*length*/) {
			if (!ec) {
				std::cout << "[write]" << std::endl;
				do_read();
			}
			else {
				socket_peer_ = NULL;//清楚对等端
			}
		});
}
