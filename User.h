
#ifndef USER
#define USER

#include <unistd.h>
#include <sys/types.h>
#include <string>
#include <string.h>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <stdio.h>
#include <stdlib.h>

using namespace std;

class User{

	int ID, index;
	int tcp_descriptor;
	bool busy;
	string IP;

public:

	User();
	
	void setBusy(bool);
	bool isBusy() const;

	void setID(int);
	int getID() const;

	void setIP(string);
	string getIP() const;

	void setIndx(int);
	int getIndx() const;

	void setTCPDescriptor(int);
	int getTCPDescriptor() const;

	void writeToClient(string);
	string readFromClient();


	void connectToPeer(User*);

};



#endif