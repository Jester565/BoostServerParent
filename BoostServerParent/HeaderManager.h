#pragma once
#include "Macros.h"
#include <boost/shared_ptr.hpp>
#include <vector>

class OPacket;
class IPacket;

class HeaderManager
{
public:
	HeaderManager();

	virtual boost::shared_ptr<std::vector<unsigned char>> encryptHeader(boost::shared_ptr<OPacket> pack);

	virtual boost::shared_ptr<IPacket> decryptHeader(boost::shared_ptr<std::vector<unsigned char>> data, unsigned int size, IDType cID);
	
	virtual ~HeaderManager();

protected:
	virtual boost::shared_ptr<std::vector<unsigned char>> encryptHeaderAsBigEndian(boost::shared_ptr<OPacket> pack) = 0;

	virtual boost::shared_ptr<std::vector<unsigned char>> encryptHeaderToBigEndian(boost::shared_ptr<OPacket> pack) = 0;

	virtual boost::shared_ptr<IPacket> decryptHeaderAsBigEndian(boost::shared_ptr<std::vector<unsigned char>> data, unsigned int size, IDType cID) = 0;

	virtual boost::shared_ptr<IPacket> decryptHeaderFromBigEndian(boost::shared_ptr<std::vector<unsigned char>> data, unsigned int size, IDType cID) = 0;

	bool bEndian;
};

