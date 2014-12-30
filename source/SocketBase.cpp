#include "stdafx.h"
#include "SocketBase.h"


// creates the WSAStartup object for winsock usage
// if there is an error creating wsastartup then it returns true 
// if no error it returns false.
bool winsock_create()
{
	WSADATA wsadata;
	int error = WSAStartup(MAKEWORD(2,2), &wsadata);

	if(error != 0)
	{
		cout << "error with startup" << endl;
		return true;
	}
	if(wsadata.wVersion != 0x0202)
	{
		WSACleanup();
		cout << "error in wsadata" << endl;
		return true;
	}
	return false;

}

// cleans up socket for closing or error in setup
void CloseConnection(SOCKET & s)
{
	if(s)
		closesocket(s);
	WSACleanup();
}



// socket class sets itself on default for a host based socket 
// sin_addr.s_addr is set to accecpt any IP address trying to
// connect
SocketBase::SocketBase(): s(NULL), port(0)
{
	acp_soc = INVALID_SOCKET;
	winsock_create(); // creates the WSADATA first

	addr.sin_family = AF_INET;
	addr.sin_port = htons(port); // port setup for socket

	// accepts any ip incoming for listening
	addr.sin_addr.s_addr = INADDR_ANY;


	s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if(s == INVALID_SOCKET)
	{
		CloseConnection(s);
		cout << "SOCKET CREATION FAILED " << endl;
		return;//exit(1); // ERROR close process
	}
}

// WORKS!!!!
// host socket must be already running 
void SocketBase::connect_sockets(std::string IPaddress)
{
	// sets the socket to connection to the IPaddress
	addr.sin_addr.s_addr = inet_addr(IPaddress.c_str()); 

	
	//If iMode!=0, non-blocking mode is enabled.
	//u_long iMode=1;
	//ioctlsocket(s,FIONBIO,&iMode);

	// if connect works the socket is connected else error
	// program closes and prints win last error
	if(connect(s, (SOCKADDR *) &addr, sizeof(addr)) == SOCKET_ERROR)
	{
		CloseConnection(s);
		cout << "Connection failed: lol " << GetLastError() << endl;
		return;//exit(1);
	}
}

// WORKS!!!!
// Sends a string message to the other end of the connected socket
// socket s must be already connected else error
void SocketBase::send(std::string message)
{
	/* NOT WORKING
	// checks if socket is connected else error
	int optsize = sizeof(int);
	int checker; 
	if(SOCKET_ERROR == getsockopt(s,  SOL_SOCKET, SO_ACCEPTCONN, (char*) &checker, &optsize))
	{
		cout << "socket not connected! failed to send message " << GetLastError() << endl;
		return;//exit(1);
	}
	*/

	// turns message to const char *
	const int d = message.size();
	char m[128];
	for(int i = 0; i < d; ++i)
		if(message[i] != NULL)
			m[i] = message[i];
	m[d] = NULL;
	
	
	if(acp_soc == INVALID_SOCKET)
		acp_soc = s;

	// using SOCKET send
	if(::send(acp_soc, m, sizeof(m), 0) == SOCKET_ERROR)
	{
		cout << "sent message failed! " << GetLastError() << endl;
		return;//exit(1);
	}

	
}

// WORKS!!!!
// receives a message and stores it in char array k
// if there is an error it prints the last error
char* SocketBase::receive()
{
	char k[2000];
	int size = 0;

	if(acp_soc == INVALID_SOCKET)
		acp_soc = s;

	
	if(SOCKET_ERROR == (size = recv(acp_soc, k, 2000, 0)))
	{
		CloseConnection(s);
		cout << "Failed to receive data " << GetLastError() << endl;
		return "-1";
	}

	return k;
}

// sets port for socket
void SocketBase::set_port(int p)
{
	addr.sin_port = htons(p);
	port = p;
}

/*
void SocketBase::set_ip(std::string ip_a)
{
	addr.sin_addr.s_addr = inet_addr(ip_a.c_str());
}
*/
void SocketBase::close()
{
		// closes the socket s and runs WSAcleanup
	CloseConnection(s);
}

SocketBase::~SocketBase()
{

}
