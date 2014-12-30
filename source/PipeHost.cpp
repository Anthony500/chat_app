#include "stdafx.h"
#include "PipeHost.h"


PipeHost::PipeHost()
{
	cout << "HOST" << endl;
	// CREATES PIPE 
	pipe = CreateNamedPipe(
		L"\\\\.\\pipe\\y",
		PIPE_ACCESS_OUTBOUND, 
		PIPE_TYPE_BYTE,
		2,// change later
		0,
		0,
		0,
		NULL);

	if(pipe == NULL || pipe == INVALID_HANDLE_VALUE)
	{
		std::cout << "Failed to create host to pipe" << std::endl;
		error = false;
		return;
	}

}


PipeHost::~PipeHost()
{
}
