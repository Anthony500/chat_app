#pragma once

//#ifndef __SOCKBASE_   // 
//#define __SOCKBASE_   //   



#include "stdafx.h"


/*
#include <winsock2.h>
#include <Windows.h>
#include <cstdlib>
#include <iostream>
*/
#pragma comment(lib, "Ws2_32.lib")

using std::cout;
using std::endl;

bool winsock_create();
void CloseConnection(SOCKET & s);

// need to launch WSADATA first before using this
// creates a winsock based socket abstract class that is used for class
// Host and class Client. sockets are using TCP to communicate string messages

class SocketBase
{
	// based class usage later on if needed
	protected:
		SOCKET acp_soc; // connected line socket for sending/receiving messages
		SOCKET s; // main socket for either client/server
		int port;
		SOCKADDR_IN addr;

	public:

		SocketBase();

		virtual void connect_sockets(std::string ipaddress);
		virtual void send(std::string message);
		virtual char* receive();
		virtual void set_port(int p);
		//virtual void set_ip(std::string ip);
		virtual void close();
	
		// abstract class
		virtual ~SocketBase() = 0;
};




//#endif 