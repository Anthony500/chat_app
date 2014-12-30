#pragma once

#include "SocketBase.h"

class ClientSoc: public SocketBase
{
	private:
		std::string ip; // holds the ip address the socket points to
	public:
		ClientSoc(); // creates a basic client socket setup
		ClientSoc(int ports, std::string ip); // creates the full socket setup 


		void Connect_to_host(); // connects socket to host socket

		void set_ip(std::string ip_a);// set ip 
		
		~ClientSoc();
};

