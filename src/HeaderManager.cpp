#include "HeaderManager.h"
#include "Server.h"
#include "IPacket.h"

HeaderManager::HeaderManager(Server* server)
	:server(server)
{
	union {
		uint32_t i;
		char c[4];
	} tbend = { 0x01020304 };
	bEndian = (tbend.c[0] == 1);
}


boost::shared_ptr<std::vector<unsigned char>> HeaderManager::encryptHeader(boost::shared_ptr<OPacket> oPack)
{
	if (bEndian)
	{
		return encryptHeaderAsBigEndian(oPack);
	}
	return encryptHeaderToBigEndian(oPack);
}

boost::shared_ptr<IPacket> HeaderManager::decryptHeader(std::vector<unsigned char>* data, unsigned int size, IDType cID)
{
	if (bEndian)
	{
		return decryptHeaderAsBigEndian(data, size, cID);
	}
	return decryptHeaderFromBigEndian(data, size, cID);
}


boost::shared_ptr<std::vector<unsigned char>> HeaderManager::encryptHeaderAsBigEndian(boost::shared_ptr<OPacket> pack)
{
	return boost::make_shared<std::vector<unsigned char>>();
}

boost::shared_ptr<std::vector<unsigned char>> HeaderManager::encryptHeaderToBigEndian(boost::shared_ptr<OPacket> pack)
{
	return boost::make_shared<std::vector<unsigned char>>();
}

boost::shared_ptr<IPacket> HeaderManager::decryptHeaderAsBigEndian(std::vector<unsigned char>* data, unsigned int size, IDType cID)
{
	return boost::make_shared<IPacket>();
}

boost::shared_ptr<IPacket> HeaderManager::decryptHeaderFromBigEndian(std::vector<unsigned char>* data, unsigned int size, IDType cID)
{
	return boost::make_shared<IPacket>();
}

HeaderManager::~HeaderManager()
{
}
