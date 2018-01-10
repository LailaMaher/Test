
#include "Server.h"

Server* Server::s_instance = NULL;

Server::Server(){

}

Server* Server::instance(){
	if(!s_instance)
		s_instance = new Server;
	return s_instance;
}

void Server::setDescriptor(int descriptor){
	this->descriptor = descriptor;
	if(descriptor < 0) perror("FAILED TO CREATE TCP SOCKET");
}

int Server::getDescriptor() const{
	return descriptor;
}

void Server::setAddress(){
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT_NUM);
}

void Server::createSocket(int p){
	PORT_NUM = p;
	setDescriptor( socket(AF_INET, SOCK_STREAM, 0) ); 
	setAddress();
	if( bind(getDescriptor(), (struct sockaddr *)&address, sizeof(address)) < 0 ) 
		perror("TCP BIND ERROR");
	listen(descriptor, 100);
}

User* Server::acceptUser(){

	struct sockaddr_in user_address;
	int len = sizeof(user_address);

	int user_fd = accept(descriptor, (struct sockaddr *)&user_address, (socklen_t*)&len);
	if (user_fd < 0) 
	  perror("ACCEPT ERROR");

	string IP(inet_ntoa(user_address.sin_addr));

	cout << "server desc = " << descriptor << endl;
	cout << "client desc = " << user_fd << endl;

	cout << "New User IP = " << IP << endl;

	return createUser(user_fd, IP);
	
}

// handle users array
User* Server::createUser(int user_fd, string IP){
	User* new_user = new User;
	new_user->setTCPDescriptor(user_fd);
	new_user->setIP(IP);
	
	return new_user;
}

void Server::addUser(User* new_user){

	pthread_mutex_lock(&users_mutex);
	users[current_users] = new_user;
	
	new_user->setIndx(current_users);
	new_user->setID(current_users);

	current_users++;
	pthread_mutex_unlock(&users_mutex);

	cout << "tcp desc = " << new_user->getTCPDescriptor() << endl;

	cout << "server sends ID\n";
	new_user->writeToClient(to_string(new_user->getID()));

}

void Server::removeUser(User* rem_user){
	pthread_mutex_lock(&users_mutex);
	users[rem_user->getIndx()] = users[current_users];
	users[current_users]->setIndx(rem_user->getIndx());
	current_users--;
	pthread_mutex_unlock(&users_mutex);
	delete rem_user;
}


User* Server::isExist(int ID){
	pthread_mutex_lock(&users_mutex);

	for(int i=0; i<current_users; i++){
		if(ID == users[i]->getID()){
			pthread_mutex_unlock(&users_mutex);
			return users[i];
		}
	}

	pthread_mutex_unlock(&users_mutex);
	return NULL;
}

void Server::startConnection(User* user){
	string peer_ID = user->readFromClient();

	cout << "peer ID = " << peer_ID << endl;

	User* peer = isExist(stoi(peer_ID));

	if(peer != NULL){
		cout << "peer not null\n";
		user->connectToPeer(peer);
	}
}

