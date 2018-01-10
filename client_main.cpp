


#include "Client.h"

void* handleIncomingRequests(void* client_v){
	Client* client = (Client*)client_v;

	string ip = client->ReadFromServer();
	cout << "Incoming IP :: " << ip << endl;

	const char *cip = ip.c_str();

	// send and receive hello
	cout << "sending and receiving hello\n";
	client->SendStream(cip, "hello");
	string data = client->ReadStream(); // hello
	
	cout << "waiting for msg\n";

	client->SendStream(cip,"this is a msg");
	data = client->ReadStream(); // this is a msg
	cout << data << endl;
}

void* StartConnection(void* client_v){
	Client* client = (Client*)client_v;

	int ID;	
	cout << "Enter the ID to connect to: " << endl;
	cin >> ID;
	client->WriteToServer(to_string(ID));

	//string ip = client->ReadFromServer(); 
	//cout << "Your peer IP :: " << ip << endl;

	//const char *cip = ip.c_str();
	// send and receive hello

	//cout << "sending and receiving hello\n";
	//client->SendStream(cip,"hello");
	//string data = client->ReadStream(); // hello

	//cout << "send data packet\n";
	//client->SendStream(cip,"this is a msg");	

}

int main(int argc, char const *argv[])
{

	pthread_t pid1, pid2;
	Client C;

	C.CreateUDPSocket(atoi(argv[2]));

	cout << "Connecting to 165.227.175.2 \n";

	C.CreateTCPSocket("165.227.175.2", atoi(argv[1]));

	cout << "My ID = " << C.ReadFromServer();

	int err;
	
	err = pthread_create(&pid1, NULL, &handleIncomingRequests, &C);
	err = pthread_create(&pid2, NULL, &StartConnection, &C);

	pthread_join(pid1, NULL);
	pthread_join(pid2, NULL);

	return 0;
}