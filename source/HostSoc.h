#pragma once

#include "SocketBase.h"

class HostSoc: public SocketBase
{

public:
	
	HostSoc();

	//void send(std::string message);
	

	void wait_for_connection();



	~HostSoc();
};

