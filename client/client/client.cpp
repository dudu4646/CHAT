#include "stdafx.h"
#include <stdio.h>
#include<string.h>
#include<winsock2.h> // / Winsock Library contains definitions to be used with the WinSock2 DLL and WinSock 2 applications.
#define IP "192.168.43.118"

#pragma comment(lib,"ws2_32.lib") // The #pragma comment indicates to the linker that the Ws2_32.lib file is needed.
#pragma warning(disable : 4996) // Visual Studio Compiler Directive - turn warning off - e.g. with fopen()


int main(int argc, char *argv[])
{	
	WSADATA wsa; //  structure contains information about the Windows Sockets implementation.
	SOCKET s; // 

	struct sockaddr_in server;  // structure containing an internet address
	const char *messagey;
	char send_m[2049], exit[] = { "EXIT" }, buff[2049], ip[20] = { " " }, name[21] = { " " };
	char  FN[30], server_reply[2049];
	int recv_size = 0, num;
	printf("testing");
	printf("insert ip addr: ");
	fgets(ip, 16, stdin);

	printf("\nInitiaSFSFGFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF555Flisinsdfsdfg Winsock...\n");
	printf("test2");
	printf("\nInitialising Winsock...\n");
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) // 	Initialise Winsock
	{
		printf("Failed. Error Code : %d", WSAGetLastError());
		return 1;
	}
	printf("Socket Initialised.\n");

	if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) // create a socket
	{
		printf("Could not create socket : %d", WSAGetLastError());
	}
	printf("Socket created.\n");

	printf("connecting socket to server....\n");
	server.sin_addr.s_addr = inet_addr(ip); // inet_addr () converts string in IPv4 dotted decimal notation, to an integer  suitable for use as an Internet address.
	server.sin_family = AF_INET; // AF_INET == address family designates type of addresses socket can communicate with == IPv4 
	server.sin_port = htons(8888);  //convert server port from host to network BIG Endian format

									//Connect to remote server
	if (connect(s, (struct sockaddr *)&server, sizeof(server)) < 0)
	{
		printf("connect error");
		return 1;
	}
	send(s, name, strlen(name), 0);

	printf("Connected to server \n");



	listen(s, 3);
	fd_set set;
	FD_ZERO(&set);
	FD_SET(s, &set);

	printf("insert your name: ");
	fgets(name, 20, stdin);
	send(s, name, strlen(name), 0);
	recv_size = recv(s, server_reply, 2048, 0);
	if (strcmp(server_reply, "Connected")) {
		printf("name allready taken!!!\nclossed");
		closesocket(s);
		getchar();
		return 1;

	}



	while (1) {
		select(0, &set, nullptr, nullptr, nullptr);
		recv_size = recv(set.fd_array[0], buff, 2048, 0);
		buff[recv_size] = '\0';
		printf("~~~~\n%s\n~~~~~~\n", buff);
	}
	closesocket(s);
	printf("chat closed!!!");
	WSACleanup();
	getchar();
}
