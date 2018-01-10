
#include "User.h"

User::User(){
	setBusy(false);
}

void User::setBusy(bool busy){
	this->busy = busy;
}

bool User::isBusy() const{
	return busy;
}

void User::setIndx(int index){
	this->index = index;
}

int User::getIndx() const{
	return index;
}


void User::setID(int ID){
	this->ID = ID;
}

int User::getID() const{
	return ID;
}

void User::setIP(string IP){
	this->IP = IP;
}

string User::getIP() const{
	return IP;
}

void User::setTCPDescriptor(int tcp_descriptor){
	this->tcp_descriptor = tcp_descriptor;
	if(tcp_descriptor < 0) perror("FAILED TO CREATE TCP SOCKET");
}

int User::getTCPDescriptor() const{
	return tcp_descriptor;
}

void User::connectToPeer(User* peer){
	string s;
	if(!peer->isBusy()) {
		cout << "peer is free\n";
		s = peer->getIP();
		setBusy(true);
		peer->setBusy(true);
	}
	else s = "busy";
	cout << "send to client " << s << endl;
	writeToClient(s);
}

void User::writeToClient(string data){
    char buffer[1024];
    strcpy(buffer, data.c_str());
    cout << "User socket " << tcp_descriptor << endl;
	cout << "Buffer to be sent: " << buffer << " " << " -- Length: " << strlen(buffer) << endl;
    int x = write(tcp_descriptor, buffer, 1023); 
		if (x < 0)
			perror("WRITING FAILED");

    cout << "write error = " << x << endl;
    cout << errno << endl;
}



string User::readFromClient(){
	char buffer[1024];
	if(read(tcp_descriptor, buffer, 1023) < 0)perror("READING FAILED");
	string data(buffer);
	return data;
}
