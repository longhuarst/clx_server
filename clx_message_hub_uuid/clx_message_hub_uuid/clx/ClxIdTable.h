#pragma once





#include <set>
#include <string>
#include <boost/asio.hpp>



#include <boost/serialization/singleton.hpp>




class ClxIdTable
{
public:
	ClxIdTable();
	~ClxIdTable();


	void insert(std::string uuid, boost::asio::ip::tcp::socket& socket);

	void delete_by_id(std::string uuid);

	void delete_by_socket(boost::asio::ip::tcp::socket& socket);

	boost::asio::ip::tcp::socket* get_socket_by_id(std::string uuid);

	std::string get_id_by_socket(boost::asio::ip::tcp::socket& socket);




private:

	typedef std::map<std::string, boost::asio::ip::tcp::socket*> table_id_type;
	typedef std::map<boost::asio::ip::tcp::socket*, std::string> table_socket_type;

	table_id_type tables_id_; //ID对照表
	table_socket_type tables_socket_; //ID对照表


};


typedef boost::serialization::singleton<ClxIdTable> ClxIdTableInstance;


