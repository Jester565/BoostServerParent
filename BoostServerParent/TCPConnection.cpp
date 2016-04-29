#include "TCPConnection.h"
#include "Server.h"
#include "PacketManager.h"
#include "HeaderManager.h"
#include "ClientManager.h"
#include <boost/make_shared.hpp>
#include <boost/bind.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/asio/placeholders.hpp>
#include <iostream>

TCPConnection::TCPConnection(Server* server, SocketType* boundSocket, HeaderManager* hm)
	:server(server), socket(boundSocket), cID(cID), errorMode(DEFAULT_ERROR_MODE)
{
	
}

void TCPConnection::start()
{
	read();
}

void TCPConnection::read()
{
	receiveStorage = boost::make_shared<std::vector<unsigned char>>();
	socket->async_read_some(boost::asio::buffer(*receiveStorage), boost::bind(&TCPConnection::asyncReceiveHandler, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

void TCPConnection::asyncReceiveHandler(const boost::system::error_code& error, unsigned int nBytes)
{
	if (error)
	{
		if (error == boost::asio::error::connection_reset)
		{
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
	server->getPacketManager()->process(iPack);
	read();
}

void TCPConnection::send(boost::shared_ptr<OPacket> oPack)
{
	sendStorage = hm->encryptHeader(oPack);
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
}

TCPConnection::~TCPConnection()
{
	boost::system::error_code ec;
	socket->cancel();
	socket->shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
	if (ec)
	{
		std::cerr << "Error when closing TCPConnection: " << ec.message() << std::endl;
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
}
