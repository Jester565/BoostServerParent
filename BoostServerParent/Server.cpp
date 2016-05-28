#include "Server.h"
#include "PacketManager.h"
#include "ClientManager.h"
#include "TCPAcceptor.h"
#include "IPacket.h"
#include "OPacket.h"
#include "Client.h"
#include "HeaderManager.h"
#include <iostream>
#include <google/protobuf/service.h>

Server::Server()
	:ioServiceThread(nullptr), ioService(nullptr), pm(nullptr), cm(nullptr), tcpAcceptor(nullptr)
{
	
}

void Server::createManagers()
{
	ioService = new boost::asio::io_service();
	pm = new PacketManager(this);
	cm = new ClientManager(ioService, this);
}

void Server::asyncInitServer(uint16_t port)
{
	tcpAcceptor = boost::make_shared <TCPAcceptor>(this);
	tcpAcceptor->detach(port);
	ioServiceThread = new std::thread(&Server::asyncIOService, this);
	ioServiceThread->detach();
}

void Server::syncInitServer(uint16_t port)
{
	tcpAcceptor = boost::make_shared <TCPAcceptor>(this);
	tcpAcceptor->detach(port);
	ioService->run();
}

void Server::asyncIOService()
{
	ioService->run();
}


HeaderManager* Server::createHeaderManager()
{
	return nullptr;
}
boost::shared_ptr<OPacket> Server::createOPacket()
{
	return boost::make_shared<OPacket>();
}
boost::shared_ptr<IPacket> Server::createIPacket()
{
	return boost::make_shared<IPacket>();
}
boost::shared_ptr<OPacket> Server::createOPacket(boost::shared_ptr<IPacket> iPack, bool copyData)
{
	return boost::make_shared<OPacket>(&(*iPack), copyData);
}

Client * Server::createClient(boost::shared_ptr<TCPConnection> tcpConnection, IDType id)
{
	return new Client(tcpConnection, this, id);
}

Server::~Server()
{
	ioService->stop();

	if (cm != nullptr)
	{
		delete cm;
		cm = nullptr;
	}
	if (pm != nullptr)
	{
		delete pm;
		pm = nullptr;
	}

	google::protobuf::ShutdownProtobufLibrary();
}