#include "stdafx.h"
#include "Pipe.h"

// this is waiting for a pipe connection
// BLOCKING for connection
bool Pipe::connect_pipe()
{
	// waiting for connection from other app, Blocking
	bool result = ConnectNamedPipe(pipe, NULL);
	return result;
}

// sends a string to the other end of the pipe
// converts string to wchar* and returns true if no error
// else false for error
bool Pipe::send(string message)
{
	// converts string to wstring
	std::wstring x;
	x.assign(message.begin(), message.end());
	
	// sends the input to the pipe 
	const wchar_t * data = LPCWSTR(x.c_str());
	DWORD numBytesWritten = 0;

	BOOL result = WriteFile(pipe, data, wcslen(data) * sizeof(data), &numBytesWritten, NULL);

	if(!result)
	{
		cout << "error with sending!" <<endl;
		return false;
	}
	return result;
}

// receives a message from the other end of the pipe
string Pipe::receive()
{
	string output;
	wchar_t buffer[128];
	DWORD numBytesRead = 0;
	BOOL result = ReadFile(pipe, buffer, 127* sizeof(wchar_t), &numBytesRead, NULL);
	

	if(result == 0)
	{
		cout << "Failed to read data from pipe " <<  GetLastError() << endl;
		string x;
		cin >> x;
		return "-1";
	}
	


	//buffer[sizeof(buffer)] = '\0'; // null terminated string;
	std::wstring d = buffer;
	output.assign(d.begin(), d.end());

	return output;
}

void Pipe::close()
{
	CloseHandle(pipe);
}

Pipe::~Pipe()
{
	
}
