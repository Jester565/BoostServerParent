#include "Client.h"


Client::Client(boost::shared_ptr<TCPConnection> tcpConnection, Server* server, IDType id)
	:tcpConnection(tcpConnection), server(server), id(id)
{

}


Client::~Client()
{
}
