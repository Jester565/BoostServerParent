#include "TCPConnection.h"
#include "Server.h"
#include "PacketManager.h"
#include "HeaderManager.h"
#include "ClientManager.h"
#include <boost/make_shared.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <iostream>

TCPConnection::TCPConnection(Server* server, boost::asio::ip::tcp::socket* boundSocket)
	:server(server), socket(boundSocket), cID(cID), errorMode(DEFAULT_ERROR_MODE), receiveStorage(nullptr), sendStorage(nullptr), alive(true)
{
	hm = server->createHeaderManager();
}

void TCPConnection::start()
{
	read();
}

void TCPConnection::read()
{
	if (alive)
	{
		if (receiveStorage != nullptr)
		{
			delete receiveStorage;
			receiveStorage = nullptr;
		}
		receiveStorage = new std::vector<unsigned char>();
		receiveStorage->resize(MAX_DATA_SIZE);
		socket->async_read_some(boost::asio::buffer(*receiveStorage, MAX_DATA_SIZE), boost::bind(&TCPConnection::asyncReceiveHandler, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
	}
}

void TCPConnection::asyncReceiveHandler(const boost::system::error_code& error, unsigned int nBytes)
{
	std::cout << "AsyncReceive" << std::endl;
	if (error)
	{
		if (error == boost::asio::error::connection_reset)
		{
			std::cout << "Connection Closed" << std::endl;
			server->getClientManager()->removeClient(cID);
			return;
		}
		std::cerr << "Error occured in TCP Reading: " << error << " - " << error.message() << std::endl;
		switch (errorMode)
		{
		case THROW_ON_ERROR:
			throw error;
			break;
		case RETURN_ON_ERROR:
			return;
		case RECALL_ON_ERROR:
			read();
			return;
		};
	}
	boost::shared_ptr<IPacket> iPack = hm->decryptHeader(receiveStorage, nBytes, cID);
	if (iPack != nullptr)
	{
		server->getPacketManager()->process(iPack);
	}
	read();
}

void TCPConnection::send(boost::shared_ptr<OPacket> oPack)
{
	sendStorage = new std::vector<unsigned char>(*(hm->encryptHeader(oPack)));
	socket->async_write_some(boost::asio::buffer(*sendStorage, sendStorage->size()), boost::bind(&TCPConnection::asyncSendHandler, shared_from_this(), boost::asio::placeholders::error));
}

void TCPConnection::send(boost::shared_ptr<std::vector<unsigned char>> sendData)
{
	sendStorage = new std::vector<unsigned char>(*sendData);
	socket->async_write_some(boost::asio::buffer(*sendStorage), boost::bind(&TCPConnection::asyncSendHandler, shared_from_this(), boost::asio::placeholders::error));
}

void TCPConnection::asyncSendHandler(const boost::system::error_code& error)
{
	if (error)
	{
		if (error == boost::asio::error::connection_reset)
		{
			server->getClientManager()->removeClient(cID);
			return;
		}
		std::cerr << "An error occured in TCP Sending: " << error.message() << std::endl;
		switch (errorMode)
		{
		case THROW_ON_ERROR:
			throw error;
			break;
		case RETURN_ON_ERROR:
			return;
			break;
		};
	}
	delete sendStorage;
	sendStorage = nullptr;
}

TCPConnection::~TCPConnection()
{
	std::cout << "TCP CONNECTION DESTRUCTOR CALLED" << std::endl;
	if (socket != nullptr)
	{
		boost::system::error_code ec;
		socket->shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
		if (ec)
		{
			std::cerr << "Error when closing TCPConnection: " << ec.message() << std::endl;
		}
	}
	if (hm != nullptr)
	{
		delete hm;
		hm = nullptr;
	}
	if (socket != nullptr)
	{
		delete socket;
		socket = nullptr;
	}
	if (receiveStorage != nullptr)
	{
		delete receiveStorage;
		delete sendStorage;
	}
}
