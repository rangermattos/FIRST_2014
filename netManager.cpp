#include "netManager.hpp"
#include "semLib.h"
#define SERVER_PORT_NUM 1180
#define SERVER_MAX_CONNECTIONS 1
void netManager::taskFunc(int netManPtr)
{
	netManager * netMan = (netManager *) &netManPtr;
	printf("%s","task made sucessfully\n");
	struct sockaddr_in serverAddr;
	struct sockaddr_in clientAddr;
	int sockAddrSize;
	int sfd;
	int clifd;
	//int ix = 0;
	//char workname[16];
	sockAddrSize = sizeof(struct sockaddr);
	bzero((char *) &serverAddr, sockAddrSize);
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_len = (u_char) sockAddrSize;
	serverAddr.sin_port = htons(SERVER_PORT_NUM);
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	//create tcp socket
	if((sfd = socket(AF_INET, SOCK_STREAM, 0)) == ERROR)
	{
		perror("socket error ");
		return;
	}
	if(bind(sfd, (struct sockaddr*)&serverAddr, sockAddrSize) == ERROR)
	{
		perror("bind fail ");
		return;
	}
	
	//listen for driver station
	if(listen(sfd, 1) == ERROR)
	{
		perror("Hey! Listen! ");
		close(sfd);
		return;
	}
	printf("for once, I listen\n");
	while(true)
	{
		if((clifd = accept(sfd, (struct sockaddr *) &clientAddr, &sockAddrSize)) == ERROR)
		{
			perror("cannot accpet matchmaking!");
			close(sfd);
			return;
		}
		printf("Client has connected successfully");
		char packet[1500];
		int recAmnt = 0;
		while(recAmnt != -1)
		{
			recAmnt = recv(clifd, packet, 1500, 0);
			netMan->goalStatus = packet[0];
			//printf("received data %i\n", (int)packet[0]);
			if(netMan->goalStatus == 1)
				printf("goal is hot, we can shoot boss!\n");
			else if(netMan->goalStatus == 0)
				printf("goal is not hot, we wait\n");
			else if(netMan->goalStatus == 2)
				printf("Error : Camera stream is not working\n");
		}
		
	}
}
netManager::netManager()
{
	//gMan = guiMan;
	goalStatus = 0;
}
bool netManager::getGoalIsHot()
{
	return goalStatus;
}
int netManager::connect(const char * ipAddress, short port)
{
	//gMan->print(0, "%s\n", "creating socket...");
	//gMan->update();
	if((sfd = socket(AF_INET, SOCK_STREAM, 0)) == ERROR)
	{
		printf("%s\n", "[SEVERE] UNABLE TO CREATE SOCKET");
		return 1;
	}
	//gMan->print(1, "%s\n", "socket created");
	//gMan->update();
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
	//gMan->print(2, "%s\n", "ip address valid");
	//gMan->update();
	//set socket options to avoid issues
	//now we connect, with a timeout of only 1 second
	if(connectWithTimeout(sfd, (struct sockaddr *) &managerAddress, addrSize, NULL) == ERROR)
	{
		printf("%s%s\n", "[ERROR] Unable to connect to ip address : ", ipAddress);
		return 3;
	}
	//gMan->print(3, "%s\n", "connection established\n");
	//gMan->update();
	//send an FRCV ENABLE message to the FRCV server
	message FRCV_ENABLE;
	FRCV_ENABLE.dataSize = 4;
	memcpy(FRCV_ENABLE.data, "ENBL", 4);
	if(sendMsg(FRCV_ENABLE, false) == ERROR)
	{
		printf("%s\n", "Could not send ENBL message to robot");
		return 4;
	}
	//gMan->print(4, "%s%s%s%i\n", "Sucessfully connected to ip : ", ipAddress, " : ", port);
	//gMan->update();
	//init socket listening task
	//taskHandle = taskSpawn("Network Task", 100, 0, 10000, taskFunc, (int)this);
}
void netManager::spawnServer()
{
	printf("spawning Server\n");
	taskHandle = taskSpawn("network Task", 100, 0, 10000, (FUNCPTR)taskFunc, (int)this, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	printf("spawn successful\n");
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
