#pragma once
#include <stdint.h>
#include <thread>
#include <boost/asio/io_service.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

class ClientManager;
class PacketManager;
class TCPAcceptor;

class Server
{
public:
	Server();

	virtual void createManagers();
	virtual void asyncInitServer(uint16_t port);
	virtual void syncInitServer(uint16_t port);
	
	ClientManager* getClientManager()
	{
		return cm;
	}

	PacketManager* getPacketManager()
	{
		return pm;
	}

	boost::asio::io_service* getIOService()
	{
		return ioService;
	}
	virtual ~Server();

protected:
	void asyncIOService();

	boost::asio::io_service* ioService;
	std::thread* ioServiceThread;
	PacketManager* pm;
	ClientManager* cm;
	boost::shared_ptr<TCPAcceptor> tcpAcceptor;
};

