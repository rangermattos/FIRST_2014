#ifndef NETMANAGER_HPP_INCLUDED
#define NETMANAGER_HPP_INCLUDED
#include "message.hpp"
#include "vxWorks.h"
#include "sockLib.h"
#include "inetLib.h"
#include "strLib.h"
#include "sockLib.h"
#include "ioLib.h"
#include "guiManager.hpp"
#include <queue>
///impliments clash's FRCV protocol
class netManager
{
private:
	//received packets, packets are filled with the update() function
	std::queue<message *> packets;
	//vxworks socket file descriptor
	int sfd;
	struct sockaddr_in managerAddress;
	//VXWORKS task function (no threading, just tasks)
	void taskFunc(int netManagerPtr);
	int semaphore;
	int taskHandle;
	FRC::guiManager * gMan;
public:
	netManager(FRC::guiManager * guiMan);
	///@brief connects to the remote server
	///
	///if the connection drops, will just drop connection, and return 0s, but
	///will not cause the robot to stop working....
	///sends an ENABLE FRCV message
	int connect(const char * ipAddress, short port);
	//must be called in the main loop to grab new packets
	void update();
	//sends a message to the remote server
	int sendMsg(message & msg, bool sendToDS);
	void close();
	message * getMsg();
};

#endif // NETMANAGER_HPP_INCLUDED
