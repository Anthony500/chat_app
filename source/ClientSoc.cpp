#include "stdafx.h"
#include "ClientSoc.h"


ClientSoc::ClientSoc()
{
	winsock_create();// wsastartup

	// assigns setup
	addr.sin_addr.s_addr = inet_addr(ip.c_str());
	addr.sin_port = htons(port);

	// acp_soc isn't needed 
	acp_soc == INVALID_SOCKET;
	
}

ClientSoc::ClientSoc(int ports, std::string ipaddress)
{
	winsock_create();

	addr.sin_addr.s_addr = inet_addr(ipaddress.c_str());
	addr.sin_port = htons(ports);

	acp_soc == INVALID_SOCKET;
}

void ClientSoc::Connect_to_host()
{
	this->connect_sockets(ip);
	cout << "IP IS " << ip << endl;
	cout << "PORT IS " << port << endl;
}

void ClientSoc::set_ip(std::string ip_a)
{
	ip = ip_a;
	addr.sin_addr.s_addr = inet_addr(ip_a.c_str());
}


ClientSoc::~ClientSoc()
{

}

