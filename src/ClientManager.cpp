#include "ClientManager.h"
#include "Server.h"
#include "IPacket.h"
#include "OPacket.h"
#include "TCPConnection.h"
#include "Client.h"
#include <boost/bind.hpp>
using namespace boost::asio::ip;

ClientManager::ClientManager(boost::asio::io_service* ioService, Server* server)
	:ioService(ioService), server(server), errorMode(DEFAULT_ERROR_MODE)
{

}

Client* ClientManager::addClient(boost::shared_ptr <TCPConnection> tcpConnection)
{
	Client* client = nullptr;
	clientMapMutex.lock();
	IDType id = aquireNextID();
	if (id < MAX_CLIENTS)
	{
		client = server->createClient(tcpConnection, id);
		clients.emplace(id, client);
	}
	else
	{
		std::cerr << "Maximum amount of clients has been exceeded... unable to add" << std::endl;
	}
	clientMapMutex.unlock();
	return client;
}

Client* ClientManager::getClient(IDType id)
{
	clientMapMutex.lock();
	std::map <IDType, Client*>::iterator mapFind = clients.find(id);
	if (mapFind != clients.end())
	{
		clientMapMutex.unlock();
		return mapFind->second;
	}
	clientMapMutex.unlock();
	return nullptr;
}

bool ClientManager::removeClient(IDType id)
{
	clientMapMutex.lock();
	std::map <IDType, Client*>::iterator idFind = clients.find(id);
	if (idFind == clients.end())
	{
		clientMapMutex.unlock();
		std::cerr << "Could not find the client to remove" << std::endl;
		return false;
	}
	delete idFind->second;
	clients.erase(idFind);
	clientMapMutex.unlock();
	return true;
}

IDType ClientManager::aquireNextID()
{
	for (int i = 1; i < MAX_CLIENTS; i++)
	{
		if (clients.find(i) == clients.end())
		{
			return i;
		}
	}
	return 0;
}

void ClientManager::send(boost::shared_ptr<OPacket> oPack)
{
	for (int i = 0; i < oPack->getSendToIDs().size(); i++)
	{
		Client* client = getClient(oPack->getSendToIDs().at(i));
		if (client != nullptr)
		{
			send(oPack, client);
		}
		else
		{
			std::cerr << "Error occured in ClientManager:send, could not find the client id: " << oPack->getSendToIDs().at(i) << std::endl;
		}
	}
}

void ClientManager::send(boost::shared_ptr<OPacket> oPack, IDType sendToID)
{
	Client* client = getClient(sendToID);
	if (client == nullptr)
	{
		std::cerr << "Error occured in ClientManager:send, could not find client nubmer" << sendToID << std::endl;
		switch (errorMode)
		{
		case THROW_ON_ERROR:
			throw "Error in ClientManager:send";
			break;
		case RETURN_ON_ERROR:
			return;
			break;
		case RECALL_ON_ERROR:
			send(oPack);
			return;
		};
	}
	client->getTCPConnection()->send(oPack);
}


void ClientManager::send(boost::shared_ptr<std::vector<unsigned char>> sendData, IDType sendToID)
{
	Client* client = getClient(sendToID);
	if (client == nullptr)
	{
		std::cerr << "Error occured in ClientManager:send, could not find client nubmer" << sendToID << std::endl;
		switch (errorMode)
		{
		case THROW_ON_ERROR:
			throw "Error in ClientManager:send";
			break;
		case RETURN_ON_ERROR:
			return;
			break;
		case RECALL_ON_ERROR:
			//send(sendData, sendToID);
			return;
		};
	}
	client->getTCPConnection()->send(sendData);
}

void ClientManager::send(boost::shared_ptr<OPacket> oPack, Client* client)
{
	if (client == nullptr)
	{
		std::cerr << "Error in ClientManager::send, the client is nullptr" << std::endl;
		switch (errorMode)
		{
		case THROW_ON_ERROR:
			throw "Error in ClientManager:send";
			break;
		case RETURN_ON_ERROR:
			return;
			break;
		case RECALL_ON_ERROR:
			send(oPack);
			return;
		};
	}
	client->getTCPConnection()->send(oPack);
}

void ClientManager::sendToAll(boost::shared_ptr<OPacket> oPack)
{
	for (auto iter = clients.begin(); iter != clients.end(); iter++)
	{
		oPack->setSenderID(iter->first);
		send(oPack, iter->second);
	}
}

void ClientManager::sendToAllExceptSender(boost::shared_ptr<OPacket> oPack)
{
	sendToAllExcept(oPack, oPack->getSenderID());
}

void ClientManager::sendToAllExcept(boost::shared_ptr<OPacket> oPack, IDType excludeID)
{
	for (auto iter = clients.begin(); iter != clients.end(); iter++)
	{
		if (iter->first != excludeID)
		{
			oPack->setSenderID(iter->first);
			send(oPack, iter->second);
		}
	}
}

void ClientManager::sendToAllExcept(boost::shared_ptr<OPacket> oPack, IDType* excludeIDs, unsigned int size)
{
	for (auto iter = clients.begin(); iter != clients.end(); iter++)
	{
		for (int i = 0; i < size; i++)
		{
			if (iter->first == excludeIDs[i])
			{
				continue;
			}
		}
		oPack->setSenderID(iter->first);
		send(oPack);
	}
}

ClientManager::~ClientManager()
{
	for (std::map <IDType, Client*>::iterator mapIter = clients.begin(); mapIter != clients.end(); mapIter++)
	{
		delete mapIter->second;
		mapIter->second = nullptr;
	}
	clients.clear();
}
