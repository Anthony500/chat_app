#pragma once


/* Pipe class creates a pipe and gives the user
// the option to send or receive data with
// methods. Pipe class can communicate with other pipes
// as long as the pipe name is the same
*/

/*
#include <Windows.h>
#include <string>
#include <iostream>
*/

#include "SocketBase.h"

using std::cin;
using std::cout;
using std::endl;
using std::string;

class Pipe
{
	protected:
		std::string filename; // name of pipe, using named pipe for communication
		bool error; // if error
		HANDLE pipe; /// holds pipe info

	public:

		// connects host pipe for communication with client pipe end
		bool connect_pipe();

		// send data through pipe
		bool send(string message);

		// receive data
		string receive();

		// close pipe connection
		void close();

		virtual ~Pipe() = 0;
};

