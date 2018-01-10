

#ifndef CLIENT
#define CLIENT

#include "User.h"
#include <netdb.h>


class Client{
	
	int TCP_PORT_NUM;
	int UDP_PORT_NUM;

	int tcp_descriptor, udp_descriptor;
	struct sockaddr_in udp_addr;

public:

// ------------- TCP ------------------------
	void CreateTCPSocket(const char*, int);
	void setTCPDescriptor(int);
	int getTCPDescriptor() const;
	string ReadFromServer();
	void WriteToServer(string);

// ------------- UDP ------------------------

	void CreateUDPSocket(int);
	void setUDPaddress();
	sockaddr* getUDPaddress() const;
	void setUDPDescriptor(int);
	int getUDPDescriptor() const;
	void SendStream(const char*, string);
	string ReadStream();

};



#endif