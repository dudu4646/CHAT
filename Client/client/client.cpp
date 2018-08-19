#include "stdafx.h"
#include <stdio.h>
#include<string.h>
#include<winsock2.h> // / Winsock Library contains definitions to be used with the WinSock2 DLL and WinSock 2 applications.
#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#include<fcntl.h>
#include<io.h>
#include<conio.h>
#define blue(ch) { SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);wprintf(L"%c",ch);SetConsoleTextAttribute(hConsole, saved_attributes);}
#define red(ch) {SetConsoleTextAttribute(hConsole, FOREGROUND_RED);wprintf(L"%c",ch);SetConsoleTextAttribute(hConsole, saved_attributes);}
#define yellow(ch) {SetConsoleTextAttribute(hConsole, 0x6);wprintf(L"%c",ch);SetConsoleTextAttribute(hConsole, saved_attributes);}
#define START HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);CONSOLE_SCREEN_BUFFER_INFO consoleInfo;WORD saved_attributes;GetConsoleScreenBufferInfo(hConsole, &consoleInfo);saved_attributes = consoleInfo.wAttributes;
#define left 75
#define right 77
#define Enter 13



#pragma comment(lib,"ws2_32.lib") // The #pragma comment indicates to the linker that the Ws2_32.lib file is needed.
#pragma warning(disable : 4996) // Visual Studio Compiler Directive - turn warning off - e.g. with fopen()

//global var
char board[13][15], sign, ch;
int p_j, flg, turn, win;
SOCKET s;

//print board
void pb() {
	int i, j;

	START

		for (i = 0; i < 13; i++)
		{
			for (j = 0; j < 15; j++)
				switch (board[i][j])
				{
				case '|':
					blue(board[i][j])
						break;
				case '-':
					blue(board[i][j])
						break;
				default:
					wprintf(L" ");
					break;
				case 'X':
					yellow(board[i][j]);
					break;
				case 'O':
					red(board[i][j]);
					break;
				}

			wprintf(L"\n");
		}
}

void second_print() {
	system("CLS");
	wprintf(L"\n\n");
	pb();
	Sleep(50);
}

void check_j() {
	int i;
	for (i = 11; i >= 3; i -= 2) {
		if (board[i][p_j * 2 - 1] == ' ') break;
	}
	flg = (board[i][p_j * 2 - 1] == ' ') ? 1 : 0;
}


void enter_new() {
	int i;
	char msg[10];
	for (i = 1; i <= 11 && board[i][p_j * 2 - 1] == ' '; i += 2) {
		board[i][p_j * 2 - 1] = sign;
		if (i > 1)board[i - 2][p_j * 2 - 1] = ' ';
		second_print();
	}
	//sendig j column to the server
	itoa(p_j * 2 - 1, msg, 10);
	send(s, msg, strlen(msg), 0);
}

void enterFromServer(int s_j) {
	int i;
	char *send = NULL;
	for (i = 1; i <= 11 && board[i][s_j * 2 - 1] == ' '; i += 2) {
		board[i][s_j * 2 - 1] = sign;
		if (i > 1)board[i - 2][s_j * 2 - 1] = ' ';
		p_j = s_j;
		second_print();

	}
}

int use_arrow() {
	int j, temp;
	char dir;

	for (j = 0; j != p_j * 2 - 1; j++) wprintf(L" ");
	wprintf(L"\x2191\n");
	//	getch();
	dir = getch();
	if ((dir == 0) || (dir == -32))
	{
		dir = getch();
		switch (dir) {
		case right:
			if (p_j < 7) p_j++;
			break;
		case left:
			if (p_j > 1)p_j--;
			break;
		}
	}
	if ((dir == '\r') || (dir == '\n')) check_j();
	if (!flg) return 0;
	flg = 0;
	enter_new();
	return 1;
}

void play() {
	int j,recv_size;
	char buf[11];

	sign = (turn % 2) ? 'X' : 'O';
	system("CLS");

	if (!turn % 2) {
		wprintf(L"Player %d, choose where to put %c:\n", turn % 2 + 1, sign);
		for (j = 0; j != p_j * 2 - 1; j++) wprintf(L" ");
		wprintf(L"\x2193\n");
		pb();
		if (use_arrow() == 0)play();
	}
	else {
		ZeroMemory(buf, 11);
		recv_size = recv(s, buf, 11, 0);
		buf[recv_size] = '\0';

	}

}

int UP(int i, int j, int num) {
	if (board[i][j] == sign)
		if (num == 3) return 1;
		else UP(i - 2, j, num + 1);
	else return 0;

}
int DOWN(int i, int j, int num) {
	if (board[i][j] == sign)
		if (num == 3) return 1;
		else DOWN(i + 2, j, num + 1);
	else return 0;
}
int RIGHT(int i, int j, int num) {
	if (board[i][j] == sign)
		if (num == 3) return 1;
		else RIGHT(i, j + 2, num + 1);
	else return 0;
}
int LEFT(int i, int j, int num) {
	if (board[i][j] == sign)
		if (num == 3) return 1;
		else LEFT(i, j - 2, num + 1);
	else return 0;
}
int UP_LEFT(int i, int j, int num) {
	if (board[i][j] == sign)
		if (num == 3) return 1;
		else UP_LEFT(i - 2, j - 2, num + 1);
	else return 0;
}
int UP_RIGHT(int i, int j, int num) {
	if (board[i][j] == sign)
		if (num == 3) return 1;
		else UP_RIGHT(i - 2, j + 2, num + 1);
	else return 0;
}
int DOWN_LEFT(int i, int j, int num) {
	if (board[i][j] == sign)
		if (num == 3) return 1;
		else DOWN_LEFT(i + 2, j - 2, num + 1);
	else return 0;
}
int DOWN_RIGHT(int i, int j, int num) {
	if (board[i][j] == sign)
		if (num == 3) return 1;
		else DOWN_RIGHT(i + 2, j + 2, num + 1);
	else return 0;
}

int check_win() {
	int i, j;
	for (i = 1; i < 12; i += 2) {
		for (j = 1; j < 14; j += 2)
			if (board[i][j] == sign) {
				if (i < 7) {
					if (DOWN(i + 2, j, 1)) return 1;
					if ((j > 5) && (DOWN_LEFT(i + 2, j - 2, 1))) return 1;
					if ((j < 9) && (DOWN_RIGHT(i + 2, j + 2, 1))) return 1;
				}
				if (i > 5) {
					if (UP(i - 2, j, 1)) return 1;
					if ((j > 5) && (UP_LEFT(i - 2, j - 2, 1))) return 1;
					if ((j < 9) && (UP_RIGHT(i - 2, j + 2, 1))) return 1;
				}
				if ((j > 5) && (LEFT(i, j - 2, 1))) return 1;
				if ((j < 9) && (RIGHT(i, j + 2, 1)))return 1;
			}
	}
	return 0;
}

void print_win() {
	int i, j;
	START
		system("cls");
	wprintf(L"\n-= The winner is player %d - ", turn % 2 + 1);
	if (turn % 2) yellow(sign)
	else red(sign)
		wprintf(L" =-\n");

	for (i = 0; i < 13; i++)
	{
		for (j = 0; j < 15; j++)
			if ((board[i][j] == '|') || (board[i][j] == '-'))
				blue(board[i][j])
			else	if (board[i][j] == sign)
				if (turn % 2) yellow(sign)
				else red(sign)
			else wprintf(L" ");

		wprintf(L"\n");
	}
	wprintf(L"\nTo play again press Y or anything else to Quit: ");
	ch = getche();
	if (ch == 'y')ch = 'Y';
}

void print_tie() {
	win = 1;
	system("CLS");
	wprintf(L"-= Its a tie!!!! =-\n\n");
	pb();
	wprintf(L"\nTo play again press Y or anything else to Quit: ");
	ch = getche();
	if (ch == 'y')ch = 'Y';
}

void waiting() {
	char buf[10];
	int recv_size,player_j,i;

	wprintf(L"waiting for player to move:\n");
	pb();
	ZeroMemory(buf,10);
	recv_size = recv(s, buf, 9, 0);
	buf[recv_size] = '\0';
	player_j = atoi(buf);
	for (i = 1; i <= 11 && board[i][player_j * 2 - 1] == ' '; i += 2) {
		board[i][player_j * 2 - 1] = sign;
		if (i > 1)board[i - 2][player_j * 2 - 1] = ' ';
		second_print();
	}
	
}


int main(int argc, char *argv[])
{
	WSADATA wsa; //  structure contains information about the Windows Sockets implementation.
  

	struct sockaddr_in server;  // structure containing an internet address
	const char *messagey;
	char send_m[2049], exit[] = { "EXIT" }, buff[2049], ip[20] = { " " };
	char  FN[30];
	int recv_size = 0, num;

	int i, j;

	printf("insert ip addr: ");
	fgets(ip, 16, stdin);

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

	printf("Connected to server \n");
	listen(s, 3);

	fd_set set;
	FD_ZERO(&set);
	FD_SET(s, &set);

	//_setmode(_fileno(stdout), _O_U16TEXT);
	//START

	ch = 'Y';

	while (ch == 'Y') {
		printf("waiting for another player...\n");
		recv_size = recv(s, buff, 2048, 0);
		buff[1] = '\0';
		turn = atoi(buff);

		p_j = 1, flg = 0, win = 0;
		//reset the board
		for (i = 0; i < 13; i++)
			for (j = 0; j < 15; j++)
				if ((i % 2 == 0) && (j % 2 == 1)) board[i][j] = '-';
				else if ((i % 2 == 1) && (j % 2 == 0)) board[i][j] = '|';
				else board[i][j] = ' ';

		while (!win) {
			_flushall();
		//	play();
			if (turn) {
				waiting();
			}
			else {
				play();
			}
			if (win = check_win())
				print_win();
			else if (turn < 41)
				turn++;
			else
				print_tie();
		}


		if (!turn) {
			waiting();
		}
		else {
			play();
		}
	}

	closesocket(s);
	printf("chat closed!!!");
	WSACleanup();
	getchar();

}
