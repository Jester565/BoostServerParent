#pragma once
#include "Macros.h"
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio/streambuf.hpp>
#include <vector>

class Server;
class OPacket;
class HeaderManager;

class TCPConnection : public boost::enable_shared_from_this<TCPConnection>
{
public:
	TCPConnection(Server* server, SocketType* boundSocket, HeaderManager* hm);

	virtual void start();

	virtual void send(boost::shared_ptr<OPacket> oPack);

	virtual void read();

	void setErrorMode(int mode)
	{
		this->errorMode = mode;
	}

	SocketType* getSocket()
	{
		return socket;
	}

	void setCID(IDType cID)
	{
		this->cID = cID;
	}

	virtual ~TCPConnection();

protected:
	virtual void asyncReceiveHandler(const boost::system::error_code& error, unsigned int bytes);
	virtual void asyncSendHandler(const boost::system::error_code& error);
	
	boost::shared_ptr<std::vector<unsigned char>> receiveStorage;
	boost::shared_ptr<std::vector<unsigned char>> sendStorage;

	SocketType* socket;
	Server* server;
	HeaderManager* hm;
	IDType cID;
	int errorMode;
};

