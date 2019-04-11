#include "ClxIdTable.h"



ClxIdTable::ClxIdTable()
{
}


ClxIdTable::~ClxIdTable()
{
}

void ClxIdTable::insert(std::string uuid, boost::asio::ip::tcp::socket& socket)
{
	tables_id_.insert(std::pair<std::string, boost::asio::ip::tcp::socket*>(uuid,&socket));
	tables_socket_.insert(std::pair<boost::asio::ip::tcp::socket *, std::string>(&socket, uuid));

}

void ClxIdTable::delete_by_id(std::string uuid)
{

	table_id_type::iterator itr = tables_id_.find(uuid);
	if (itr != tables_id_.end()) {
		table_socket_type::iterator itr2 = tables_socket_.find(itr->second);
		if (itr2 != tables_socket_.end()) {
			tables_socket_.erase(itr2);
		}
		tables_id_.erase(itr);
	}
	
}


void ClxIdTable::delete_by_socket(boost::asio::ip::tcp::socket& socket)
{
	table_socket_type::iterator itr = tables_socket_.find(&socket);
	if (itr != tables_socket_.end()) {

		table_id_type::iterator itr2 = tables_id_.find(itr->second);
		if (itr2 != tables_id_.end()) {

			tables_id_.erase(itr2);
		}
		tables_socket_.erase(itr);
	}
}

boost::asio::ip::tcp::socket* ClxIdTable::get_socket_by_id(std::string uuid)
{
	table_id_type::iterator itr = tables_id_.find(uuid);
	if (itr != tables_id_.end()) {
		return itr->second;
	}
	else {
		return NULL;
	}
}

std::string ClxIdTable::get_id_by_socket(boost::asio::ip::tcp::socket& socket)
{
	table_socket_type::iterator itr = tables_socket_.find(&socket);
	if (itr != tables_socket_.end()) {
		return itr->second;
	}
	else {
		return "";
	}
}
