


#include "Client.h"

void* readFromUDPStream(void* client_v){

	Client* client = (Client*)client_v;
	
	while(true){
		string data = client->ReadStream();
		cout << data << endl;	
	}
	return NULL;
}


void* HandleNewConnection(void* client_v){

	Client* client = (Client*)client_v;

	while(true){

		string data = client->ReadFromServer();
		char token = data[0];

		string ip;

		switch(token){
			case '1': // ip and i am sender
				cout << "HandleNewConnection Case[1]" << endl;
				ip = data.substr(1, data.length() - 1);
				client->setPeerIP(ip);
				client->SendStream("hello");
				client->WriteToServer("2" + client->getPeerIP());
				break;

			case '2': // ip and i am receiver
				cout << "HandleNewConnection Case[2]" << endl;
				ip = data.substr(1, data.length() - 1);
				client->setPeerIP(ip);
				break;
				
			case '3': // confirmation to sender
				cout << "HandleNewConnection Case[3]" << endl;
				client->SendStream("this is the stream");
				break;
			
			case '4': // confirmation to receiver
				cout << "HandleNewConnection Case[4]" << endl;
				client->SendStream("hello");
				client->WriteToServer("3" + client->getPeerIP());

			default:
				cout << "invalid token" << endl;
				break;

		}
	}
	return NULL;
}

void* StartConnection(void* client_v){
	
	Client* client = (Client*)client_v;

	int ID;	
	cout << "Enter the ID to connect to: " << endl;
	cin >> ID;
	client->WriteToServer("1" + to_string(ID));

	return NULL;
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