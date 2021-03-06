#pragma once
#include <vector>
#include <list>
#include <mutex>
#include <queue>
#include <boost/shared_ptr.hpp>

class PKey;
class IPacket;
class Server;

class PacketManager
{
public:
	static const int USE_PKEYS_SIZE = -1;
	static const bool THROW_KEY_NOT_FOUND_EXCEPTION = false;
	PacketManager(Server* server);

	void addPKey(PKey*);

	void addPKeys(std::vector <PKey*>);

	bool removePKey(PKey*);

	int removePKeys(std::vector <PKey*>);

	void process(boost::shared_ptr<IPacket> iPack);

	~PacketManager();

protected:
	void serverProcess(boost::shared_ptr<IPacket> iPack);

	int binarySearchKey(bool& found, std::string key, int f = 0, int l = USE_PKEYS_SIZE);
	std::vector <std::list <PKey*>> pKeys;
	std::mutex pKeysMutex;
	Server* server;
};