#include "HeaderManager.h"



HeaderManager::HeaderManager()
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

boost::shared_ptr<IPacket> HeaderManager::decryptHeader(boost::shared_ptr<std::vector<unsigned char>> data, unsigned int size, IDType cID)
{
	if (bEndian)
	{
		return decryptHeaderAsBigEndian(data, size, cID);
	}
	return decryptHeaderFromBigEndian(data, size, cID);
}

HeaderManager::~HeaderManager()
{
}