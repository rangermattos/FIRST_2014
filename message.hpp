//max MTU for ethernet
#define PACKET_SIZE 1500
struct message
{
	unsigned short dataSize;
	char data[PACKET_SIZE];
};
