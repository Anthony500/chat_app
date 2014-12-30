#include "stdafx.h"
#include "HostSoc.h"


HostSoc::HostSoc()
{
	addr.sin_addr.s_addr = INADDR_ANY; // set to users pc IP address for hosting
}



// waits for incoming connections
void HostSoc::wait_for_connection()
{
	// binds socket to port
	if(::bind(s, (LPSOCKADDR) & addr, sizeof(addr)) == SOCKET_ERROR)
	{
		CloseConnection(s);
		cout << "socket failed to bind to host" << endl;
	}

	// listen to as many incoming sockets max
	listen(s, SOMAXCONN);


	//If iMode!=0, non-blocking mode is enabled.
	//u_long iMode=1;
	///ioctlsocket(s,FIONBIO,&iMode);

	// blocking, waiting for incoming connection from other socket online
	acp_soc = accept(s, (LPSOCKADDR) & addr, NULL);

	cout << "onling user connected!" << endl;
}


HostSoc::~HostSoc()
{
}
