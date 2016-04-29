#pragma once
#include "Macros.h"
#include <vector>
#include <string>
#include <iostream>
#include <boost/shared_ptr.hpp>

class dataframe;
class OPacket;

class IPacket
{
	friend class HeaderManager;
public:
	IPacket();

	IDType getSentFromID()
	{
		return sentFromID;
	}

	const char* getLocKey() const
	{
		return locKey;
	}

	std::vector <IDType>* getSendToClients()
	{
		return sendToClients;
	}

	bool getServerRead()
	{
		return serverRead;
	}

	friend std::ostream& operator << (std::ostream& oStream, IPacket& iPack)
	{
		oStream << "Printing out packet: " << std::endl << "loc key: " << iPack.locKey << std::endl;
		oStream << "Sent from: " << iPack.sentFromID << std::endl;
		if (iPack.sendToClients == nullptr)
		{
			oStream << "Send to clients: size = 0" << std::endl;
		}
		else
		{
			oStream << "Send to clients: size = " << iPack.sendToClients->size() << "  Items: ";
			for (int i = 0; i < iPack.sendToClients->size(); i++)
			{
				oStream << iPack.sendToClients->at(i) << " ";
			}
			oStream << std::endl;
		}
		return oStream;
	}

	virtual std::vector<boost::shared_ptr<OPacket>>* IPacket::convertToOPacks(bool copyData);

	virtual ~IPacket();

protected:
	IDType sentFromID;
	char locKey[3];
	bool serverRead;
	std::vector <IDType>* sendToClients;
};