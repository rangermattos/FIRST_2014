#include "netManager.hpp"
#include "semLib.h"
void netManager::taskFunc(int netManager)
{
	//netManager * netMan = (netManager *) &netManager;
	printf("%s","task made sucessfully\n");
}
netManager::netManager(FRC::guiManager * guiMan)
{
	gMan = guiMan;
}
int netManager::connect(const char * ipAddress, short port)
{
	gMan->print(0, "%s\n", "creating socket...");
	gMan->update();
	if((sfd = socket(AF_INET, SOCK_STREAM, 0)) == ERROR)
	{
		printf("%s\n", "[SEVERE] UNABLE TO CREATE SOCKET");
		return 1;
	}
	gMan->print(1, "%s\n", "socket created");
	gMan->update();
	int addrSize = sizeof(struct sockaddr_in);
	bzero((char *) &managerAddress, addrSize);
	managerAddress.sin_family = AF_INET;
	managerAddress.sin_len = (u_char)addrSize;
	managerAddress.sin_port = htons(port);
	
	managerAddress.sin_addr.s_addr = inet_addr((char *)ipAddress);
	if(managerAddress.sin_addr.s_addr == ERROR)
	{
		printf("%s\n", "[ERROR] Unable to resolve target ip address\n");
		return 2;
	}
	gMan->print(2, "%s\n", "ip address valid");
	gMan->update();
	//set socket options to avoid issues
	//now we connect, with a timeout of only 1 second
	if(connectWithTimeout(sfd, (struct sockaddr *) &managerAddress, addrSize, NULL) == ERROR)
	{
		printf("%s%s\n", "[ERROR] Unable to connect to ip address : ", ipAddress);
		return 3;
	}
	gMan->print(3, "%s\n", "connection established\n");
	gMan->update();
	//send an FRCV ENABLE message to the FRCV server
	message FRCV_ENABLE;
	FRCV_ENABLE.dataSize = 4;
	memcpy(FRCV_ENABLE.data, "ENBL", 4);
	if(sendMsg(FRCV_ENABLE, false) == ERROR)
	{
		printf("%s\n", "Could not send ENBL message to robot");
		return 4;
	}
	gMan->print(4, "%s%s%s%i\n", "Sucessfully connected to ip : ", ipAddress, " : ", port);
	gMan->update();
	//init socket listening task
	//taskHandle = taskSpawn("Network Task", 100, 0, 10000, taskFunc, (int)this);
}
int netManager::sendMsg(message & msg, bool sendToDS)
{
	int flags = 0;
	if(!sendToDS)
	{
		//make sure we do not use routing tables, we do not want
		//to break rules
		flags = MSG_DONTROUTE;
	}
	int sentBytes = 0;
	while(sentBytes < msg.dataSize)
		sentBytes += send(sfd, msg.data, msg.dataSize, flags);
	return 0;
}
message * netManager::getMsg()
{
	return NULL;
}
