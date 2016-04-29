#include "Server.h"
#include "PacketManager.h"
#include "ClientManager.h"
#include "TCPAcceptor.h"
#include <iostream>
#include <google/protobuf/service.h>

Server::Server()
	:ioServiceThread(nullptr), ioService(nullptr), pm(nullptr), cm(nullptr), tcpAcceptor(nullptr)
{

}

void Server::createManagers()
{
	ioService = new boost::asio::io_service();
	pm = new PacketManager();
	cm = new ClientManager(ioService, this);
	pm->setClientManager(cm);
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

Server::~Server()
{
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
	if (ioService != nullptr)
	{
		delete ioService;
		ioService = nullptr;
	}
	if (ioServiceThread != nullptr)
	{
		delete ioServiceThread;
		ioServiceThread = nullptr;
	}
	google::protobuf::ShutdownProtobufLibrary();
}