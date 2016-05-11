#pragma once
#include "Macros.h"
#include <stdint.h>
#include <thread>
#include <boost/asio/io_service.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

class ClientManager;
class PacketManager;
class TCPAcceptor;
class TCPConnection;
class HeaderManager;
class IPacket;
class OPacket;
class Client;

#ifndef IPACKET_TYPE
#define IPACKET_TYPE
typedef IPacket IPacketType;
#endif

#ifndef OPACKET_TYPE
#define OPACKET_TYPE
typedef OPacket OPacketType;
#endif

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
	
	virtual HeaderManager* createHeaderManager();
	virtual boost::shared_ptr<OPacket> createOPacket();
	virtual boost::shared_ptr<IPacket> createIPacket();
	virtual boost::shared_ptr<OPacket> createOPacket(boost::shared_ptr<IPacket> iPack, bool copyData);
	virtual Client* createClient(boost::shared_ptr<TCPConnection> tcpConnection, IDType id);

	void asyncIOService();

	virtual ~Server();

protected:
	boost::asio::io_service* ioService;
	std::thread* ioServiceThread;
	PacketManager* pm;
	ClientManager* cm;
	boost::shared_ptr<TCPAcceptor> tcpAcceptor;
};

