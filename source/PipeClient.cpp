#include "stdafx.h"
#include "PipeClient.h"


PipeClient::PipeClient()
{

		// CREATES CLIENT PIPE 
	pipe = CreateFile(
		L"\\\\.\\pipe\\y",
		GENERIC_READ,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if(pipe == INVALID_HANDLE_VALUE)
	{
		cout << "Failed to connect to pipe " << GetLastError() << endl;
		return;
	}


	
}


PipeClient::~PipeClient()
{
}
