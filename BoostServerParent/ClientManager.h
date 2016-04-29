#pragma once
#include "Macros.h"
#include <boost/asio/io_service.hpp>
#include <mutex>

class TCPConnection;
class PacketManager;
class IPacket;
class OPacket;
class Client;
class Server;

class ClientManager
{
public:
	static const unsigned int MAX_CLIENTS = 255;

	ClientManager(boost::asio::io_service* ioService, Server* server);

	virtual Client* addClient(boost::shared_ptr <TCPConnection> tcpConnection);

	virtual Client* getClient(IDType id);

	virtual bool removeClient(IDType id);


	std::map <IDType, Client*> getIDClientMap() const
	{
		return clients;
	}

	virtual void send(boost::shared_ptr<OPacket> oPack);

	virtual void send(boost::shared_ptr<OPacket> oPack, IDType sendTo);

	virtual void send(boost::shared_ptr<OPacket> oPack, Client* sendToClient);

	virtual void sendToAll(boost::shared_ptr<OPacket> oPack);

	virtual void sendToAllExceptSender(boost::shared_ptr<OPacket> oPack);

	virtual void sendToAllExcept(boost::shared_ptr<OPacket> oPack, IDType excludeID);

	virtual void sendToAllExcept(boost::shared_ptr<OPacket> oPack, IDType* excludeIDs, unsigned int size);

	void setErrorMode(int mode)
	{
		errorMode = mode;
	}

	int getErrorMode() const
	{
		return errorMode;
	}

	virtual ~ClientManager();

protected:

	std::map <IDType, Client*> clients;
	std::mutex clientMapMutex;
	IDType aquireNextID();
	Server* server;
	boost::asio::io_service* ioService;
	int errorMode;
};

