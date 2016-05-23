#pragma once
#include "Macros.h"
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio/streambuf.hpp>
#include <vector>

class Server;
class OPacket;
class HeaderManager;

class TCPConnection : public boost::enable_shared_from_this<TCPConnection>
{
public:
	TCPConnection(Server* server, boost::asio::ip::tcp::socket* boundSocket);

	virtual void start();

	virtual void send(boost::shared_ptr<OPacket> oPack);

	virtual void send(boost::shared_ptr<std::vector<unsigned char>> sendData);

	virtual void read();

	void setErrorMode(int mode)
	{
		this->errorMode = mode;
	}

	boost::asio::ip::tcp::socket* getSocket()
	{
		return socket;
	}

	void setCID(IDType cID)
	{
		this->cID = cID;
	}

	void kill()
	{
		this->alive = false;
	}

	virtual void asyncReceiveHandler(const boost::system::error_code& error, unsigned int bytes);
	
	virtual void asyncSendHandler(const boost::system::error_code& error);

	virtual ~TCPConnection();

protected:
	std::vector<unsigned char>* receiveStorage;
	std::vector<unsigned char>* sendStorage;

	boost::asio::ip::tcp::socket* socket;
	Server* server;
	HeaderManager* hm;
	IDType cID;
	int errorMode;
	bool alive;
};

