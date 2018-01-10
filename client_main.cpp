


#include "Client.h"

bool SENDER = false;


void* readFromUDPStream(void* client_v){
	Client* client = (Client*)client_v;
	string data = client->ReadStream();
	cout << data << endl;
}


void* HandleNewConnection(void* client_v){
	Client* client = (Client*)client_v;

	string ip = client->ReadFromServer();
	cout << "Incoming IP :: " << ip << endl;
	const char *cip = ip.c_str();

	if(SENDER){
		// send UDP hello msg
		cout << "I am sender \nsending hello\n";
		client->SendStream(cip, "hello");
		cout << "inform server about sending hello\n";
		client->WriteToServer("hello sent");
		cout << "sending UDP packet stream\n";
		client->SendStream(cip, "this is a packet stream");


	} else{

		// wait for UDP hello msg
		cout << "I am receiver \nwaiting for hello from server\n";
		string hello_sent = client->ReadFromServer();
		cout << "sending hello to peer\n";
		client->SendStream(cip, "hello");
		cout << "inform server about sending hello\n";
		client->WriteToServer("hello sent");

	}
	return NULL;
}

void* StartConnection(void* client_v){
	Client* client = (Client*)client_v;

	int ID;	
	cout << "Enter the ID to connect to: " << endl;
	cin >> ID;

	SENDER = true;

	client->WriteToServer(to_string(ID));
}

int main(int argc, char const *argv[])
{

	pthread_t pid1, pid2, pid3;
	Client C;

	C.CreateUDPSocket(atoi(argv[2]));

	cout << "Connecting to 165.227.175.2 \n";

	C.CreateTCPSocket("165.227.175.2", atoi(argv[1]));

	cout << "My ID = " << C.ReadFromServer();

	int err;
	
	err = pthread_create(&pid1, NULL, &HandleNewConnection, &C);
	err = pthread_create(&pid2, NULL, &StartConnection, &C);
	err = pthread_create(&pid3, NULL, &readFromUDPStream, &C);


	pthread_join(pid1, NULL);
	pthread_join(pid2, NULL);

	return 0;
}