chat_app
========

Console messenger program 

The user is asked if they want to host the connection or join a current hosted connection. They have to type either "h" for hosting or "j" for joining another hosted connection. 

If the user is hosting, the port is hardcoded to port 1234. I haven't got around to changing that yet for the user to set a specific port number. And if the user is joining a connection they need to enter the ip address and port number of the current host. 

In order to quit the program the user must type the word "quit" in the chat window to close the socket connection. 

And if the chat window is closed before the chatlog window or the user didn't quit properly, the chatlog window loops a readfile error because the named pipe is closed but it's still reading for input. 

I created this just to understand winsocks and IPC on windows. This program does not encrypt any messages that might be a figure I add over time. And I am still working on the main file and UI, nothing is complete. 
