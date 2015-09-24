// all main include headers
#include "stdafx.h"


#include <thread>

// win namedpipe api classes
#include "PipeHost.h"
#include "PipeClient.h"

// winsocks classes
#include "HostSoc.h"
#include "ClientSoc.h"


using std::cin;
using std::string;

// Get the horizontal and vertical screen sizes in pixel
void GetDesktopResolution(int& horizontal, int& vertical)
{
	RECT desktop;
	// Get a handle to the desktop window
	const HWND hDesktop = GetDesktopWindow();
	// Get the size of screen to the variable desktop
	GetWindowRect(hDesktop, &desktop);
	// The top left corner will have coordinates (0,0)
	// and the bottom right corner will have coordinates
	// (horizontal, vertical)
	horizontal = desktop.right;
	vertical = desktop.bottom;
}


// changes text color in console
// if argument is set to blue the text is light blue
// else its yellow
void change_color(string color = "")
{


	// COLOR CHANGER
	HANDLE h = GetStdHandle ( STD_OUTPUT_HANDLE );
	WORD wOldColorAttrs;
	CONSOLE_SCREEN_BUFFER_INFO csbiInfo;

	/*
	* First save the current color information
	*/

	GetConsoleScreenBufferInfo(h, &csbiInfo);
	wOldColorAttrs = csbiInfo.wAttributes;

	/*
	* Set the new color information
	*/
	if(color == "blue")
		SetConsoleTextAttribute ( h, FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE);
	else // yellow
		SetConsoleTextAttribute ( h, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN );
	// END OF COLOR CHANGER
}

// WORKS!!!!!
// opens a new process
// it opens a new console window and sets the command line arguement to 1234
// if 1234 is the arguement then when main runs it sets itself as a displayer
// of incoming messages and user inputs
// the arguements are 1234 and username in argv[0] and argv[1]
void startup(LPCTSTR lpApplicationName, string username, bool if_host, string ip_port[] = NULL)
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory( &si, sizeof(si));
	//si.cb = sizeof(si);
	ZeroMemory( &pi, sizeof(pi));

	string holder = "1234 ";
	holder += username;

	// if client, argument is used to create a client socket setup next main opening
	if(!if_host)
	{
		holder += " client ";
		holder += ip_port[0];
		holder += " ";
		holder += ip_port[1];
	}
	std::wstring x;
	x.assign(holder.begin(), holder.end());


	if(!CreateProcess(lpApplicationName, LPWSTR(x.c_str()), NULL, NULL,
		NULL, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi))
	{
		std::cerr << "Uh-Oh! CreateProcess() failed to start program \"" << GetLastError() << "\"\n";
		//	exit(1);
	}
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
}

// WORKS no probs
// pipe chatlog
void show_chat1(Pipe * p, string username, SocketBase * s)
{

	// send username;
	//s->send(username);

	HANDLE h = GetStdHandle ( STD_OUTPUT_HANDLE );
	string output;
	while(true)
	{
		// get user input from pipe
		output = p->receive();

		// send user input to socket
		s->send(output);

		if(output == "quit")
		{
			cout << endl << username << " has quit/left" << endl;
			break;
		}
		SetConsoleTextAttribute ( h, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN );
		cout << username << ": " << output << endl;
	}
}

// socket chatlog
void show_chat2(SocketBase * s)
{
	string username = s->receive();
	cout <<  username << " joined chat!!!" << endl << endl;

	HANDLE h = GetStdHandle ( STD_OUTPUT_HANDLE );
	string output;
	while(true)
	{
		// get message from socket
		output = s->receive();

		// if message is quit end chatlog
		if(output == "quit")
		{
			cout << endl << username << " QUIT" << endl;
			break;
		}

		// if no error continue messaging
		if(output != "-1")
		{
			SetConsoleTextAttribute ( h, FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE);
			cout << username << ": " << output << endl;
		}

	}
	//exit(1);
}

// asks user if hosting/connecting to other host
// then asks user for host ip if joining host
bool prompt_user()
{
	string input;
	cout << "Hosting or joining other host?" << endl;


	while(true)
	{
		cin >> input;
		if(input == "host" || input == "hosting" || input == "h")
			return true;
		else if(input == "joining" || input == "join" || input == "j")
			return false;
	}
}

int main(int argc, char* argv[])
{

	int x;
	int y;
	GetDesktopResolution(x, y);
	HWND consoleWindow = GetConsoleWindow();
	SetWindowPos( consoleWindow, 0, x/2 - 300, y/2, 500, 150, SWP_NOZORDER );

	// run chat display process
	if(string(argv[0]) == "1234")
	{
		int x;
		int y;
		GetDesktopResolution(x, y);
		HWND consoleWindow = GetConsoleWindow();

		SetWindowPos( consoleWindow, 0, x/2 - 300, y/2 - 300, 500, 300, SWP_NOZORDER );
		SetConsoleTitle(TEXT("LOG DISPLAY ---------------------------------"));

		// creates client end of pipe
		PipeClient display_window;

		/*
		// creates client end of socket
		ClientSoc display_window2;

		// sets ports and connects to host socket
		display_window2.set_port(1234);
		display_window2.Connect_to_host();
		*/

		string username(argv[1]);

		SocketBase * display_window2 = NULL;

		if(argc == 2) // Host setup
		{
			cout << "HOST SETUP" << endl;
			// sets host socket for online communication
			HostSoc * main_soc = new HostSoc();
			main_soc->set_port(1234);
			cout << "Connecting socket ... " << endl;
			main_soc->wait_for_connection();
			cout << "Connected!!! ... " << endl << endl;;
			display_window2 = main_soc;
		}
		else if(argc == 5) // Client setup
		{
			cout << "CLIENT SETUP" << endl;
			// creates client end of socket
			ClientSoc * cli_soc = new ClientSoc();

			// sets ports and connects to host socket
			cli_soc->set_ip(string(argv[3]));
			cli_soc->set_port(std::stoi(argv[4]));
			cout << "Connectiong socket... " << endl;
			cli_soc->Connect_to_host();
			cout << "Connected!!!" << endl << endl;

			display_window2 = cli_soc;
		}

		// sends username for socket setup, print in log
		display_window2->send(username);



		///*
		// 2 threads for logging the chat coming from the pipe and from the socket
		std::thread pipe_chatlog(show_chat1, &display_window, username, display_window2);
		std::thread soc_chatlog(show_chat2, display_window2);
		pipe_chatlog.join();
		soc_chatlog.join();

		//*/


		// closes socket and pipe connections
		display_window.close();
		display_window2->close();

		// clear allocated memory from socketbased class
		delete display_window2;

		std::cout << "END OF APP" << std::endl;

		string d;
		cin >> d;
		return 0;
	}

	// if running client create client setup else host is default and prompt_user returns true
	string ip_port[2];
	bool if_host = prompt_user();
	if(!if_host)
	{


		cout << "Enter host ip address and port" << endl;
		cout << "IP address: ";
		cin >> ip_port[0];
		cout << "Port number: ";
		cin >> ip_port[1];
	}



	// get username input
	string username;
	cout << "Enter username" << endl;
	cin >> username;


	// get current directory
	char buffer[256];
	GetModuleFileNameA(NULL, buffer, 256);
	string::size_type pos = string(buffer).find_last_of("\\/");
	string buf = string(buffer).substr(0, pos);
	buf += "/chat_app.exe";
	std::wstring directory;
	directory.assign(buf.begin(), buf.end());
	LPCWSTR dir = directory.c_str();

	// create client process, chatlog console
	startup(dir, username, if_host, ip_port);

	// create and connect pipe to client process
	PipeHost chat_window;
	cout << "Connecting pipe ... " << endl;
	chat_window.connect_pipe();
	cout << "Connected!!! ... " << endl;




	// changes text title of console
	SetConsoleTitle(TEXT("CHAT ---------------------------------"));

	//main_soc.send("hamburger time");
	//return 1;
	// ask user for message input
	string input;
	while(true)
	{
		cout << "Enter message to send!" << endl;
		// get input
		while(true)
		{
			getline(cin, input);
			if(input != "")
				break;
		}
		// sends input to display_window from pipe
		chat_window.send(input);
		// sends input to other socket online
		//main_soc->send(input);
		// quit chat
		if(input == "quit")
			break;
	}

	// close pipe connection
	chat_window.close();



	std::cout << "END OF APP" << std::endl;

	char b;
	cin >> b;
	return 0;
}
