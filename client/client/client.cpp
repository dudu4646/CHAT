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

//checking if the column is full
void check_j() {
	int i;

	for (i = 11; i >= 3; i -= 2) {
		if (board[i][p_j * 2 - 1] == ' ') break;
	}
	flg = (board[i][p_j * 2 - 1] == ' ') ? 1 : 0;
}

//drop in & sending the sign 
void enter_new() {
	int i;
	char msg[10];

	//sendig j column to the server
	itoa(p_j * 2 - 1, msg, 10);
	send(s, msg, strlen(msg), 0);

	//drop the sign into the column
	for (i = 1; i <= 11 && board[i][p_j * 2 - 1] == ' '; i += 2) {
		board[i][p_j * 2 - 1] = sign;
		if (i > 1)board[i - 2][p_j * 2 - 1] = ' ';
		second_print();
	}
}

//drop the sign from the server into the column
void enterFromServer(int s_j) {
	int i;

	sign = (turn % 2) ? 'O' : 'X';
	for (i = 1; i <= 11 && board[i][s_j] == ' '; i += 2) {
		board[i][s_j] = sign;
		if (i > 1)board[i - 2][s_j] = ' ';
		second_print();
	}
}

//using arrow keys to select colunm
int use_arrow() {
	int j, temp;
	char dir;

	for (j = 0; j != p_j * 2 - 1; j++) wprintf(L" ");
	wprintf(L"\x2191\n");
	dir = getch();
	if ((dir == 0) || (dir == -32))
	{	//making sure to stay in the border
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
	START
	int j,recv_size;
	char buf[11];

	sign = (turn % 2) ? 'O' : 'X';
	system("CLS");

	wprintf(L"Its your move (%c)\n",sign);
	pb();
	if (use_arrow() == 0)play();
}

//cheking it there's a victory in any side
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

//printing wining sign
void print_win() {
	int i, j;
	START
		system("cls");
	wprintf(L"\n-= The winner is ");
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

}

//printing tie
void print_tie() {
	win = 1;
	system("CLS");
	wprintf(L"-= Its a tie!!!! =-\n\n");
	pb();
}

//when the other player is playing
void waiting(int num) {
	int i;

	system("cls");
	wprintf(L"Oponnet move:\n");
	pb();

	enterFromServer(num);	
}


int main(int argc, char *argv[])
{
	WSADATA wsa; //  structure contains information about the Windows Sockets implementation.
	struct sockaddr_in server;  // structure containing an internet address
	const char *messagey;
	char buff[10], ip[20] = { " " };
	int recv_size = 0, num,i,j,reset_game=1;
	
	//test
	printf("Initialising Winsock...\n");
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) // 	Initialise Winsock
	{
		printf("Failed. Error Code : %d", WSAGetLastError());
		return 1;
	}
	printf("Socket Initialised.\n");

	Sleep(500);

	system("cls");
	printf("insert ip addr: ");
	fgets(ip, 16, stdin);

	do{
		system("cls");
		if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) // create a socket
		{
			printf("Could not create socket : %d", WSAGetLastError());
		}
		printf("Socket created.\n");
		Sleep(500);
		printf("connecting socket to server....\n");
		server.sin_addr.s_addr = inet_addr(ip); // inet_addr () converts string in IPv4 dotted decimal notation, to an integer  suitable for use as an Internet address.
		server.sin_family = AF_INET; // AF_INET == address family designates type of addresses socket can communicate with == IPv4 
		server.sin_port = htons(8888);  //convert server port from host to network BIG Endian format

		//Connect to remote server
		if (connect(s, (struct sockaddr *)&server, sizeof(server)) < 0)
		{
			printf("connect error...\nShuting down...");
			Sleep(2000);
			return 1;
		}

		printf("Connected to server \n");
		Sleep(500);
		listen(s, 3);

		printf("waiting for another player...\n");
		do {
			recv_size = recv(s, buff, 2048, 0);
		} while (recv_size==-1);
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

			if (turn % 2) {

				system("cls");
				printf("wayting for player move...\n");
				pb();
				recv_size = recv(s, buff, 2, 0);
				buff[recv_size] = '\0';
				int num = atoi(buff);
				if (num == -1)
				{
					win = 1;
					printf("\nPlayer log off\n");
					break;
				}
				waiting(num);
				if (win = check_win()) {
					print_win();
					continue;
				}
				else if (turn < 41)
					turn++;
				else {
					print_tie();
					continue;
				}

			}
			play();
			if (win = check_win())
				print_win();
			else if (turn <= 41)
				turn++;
			else
				print_tie();
		}
		getch();
		closesocket(s);
		printf("GAME OVER!!!\n");
		printf("\nTo play again press Y or anything else to Quit: ");
		ch = getche();
		reset_game = ((ch == 'y') || (ch == 'Y'));

	}while (reset_game);

	WSACleanup();
	printf("\n-= BYE =-\n");
	Sleep(500);

}
