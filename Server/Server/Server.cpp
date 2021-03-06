#include <iostream>
#include <WS2tcpip.h>
#include "stdafx.h"
#include<io.h>
#include<stdio.h>
#include<winsock2.h>
#include <stdlib.h>
#include <cstdlib>

#pragma warning(disable : 4996)
#pragma comment(lib,"ws2_32.lib") //Winsock Library

using namespace std;

//player struct
typedef struct CONTACT {
	SOCKET s,*vs;
	bool inGame;
	struct CONTACT *next, *prev;
}CONTACT;

//add new contacts
void addContact(CONTACT *head, CONTACT *newContact) {
	CONTACT *current = head->next, *prev = head;

	while (current != NULL) { //getting the last item
		prev = current;
		current = current->next;
	}

	prev->next = newContact;
	newContact->prev = prev;
	newContact->next = NULL;
}

//serching tor waiting player
CONTACT* checkFreePlayer(CONTACT *player,CONTACT *list) {
	CONTACT *current = list->next;
	while (current) {
		if ((current!=player)&&(!current->inGame) ) return current;
		else current = current->next;
	}
	return nullptr;
}

//finding a player by socket
CONTACT *findPlayerBySocket(SOCKET s, CONTACT *list) {
	CONTACT *current = list->next;
	while (current) {
		if (s == current->s) return current;
		current = current->next;
	}

	return NULL;
}

int main() {

	// Initialze winsock
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);
	const char *message;
	char buf[11];
	int buf_size, wsOk = WSAStartup(ver, &wsData);
	if (wsOk != 0)
	{
		printf("Can't Initialize winsock! Quitting \n");
		return 1;
	}

	// Create a socket
	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == INVALID_SOCKET)
	{
		printf("Can't create a socket! Quitting\n");
		return 1;
	}

	// Bind the ip address and port to a socket
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(8888); //54000
	hint.sin_addr.S_un.S_addr = INADDR_ANY; // Could also use inet_pton .... 

	bind(listening, (sockaddr*)&hint, sizeof(hint));

	// Tell Winsock the socket is for listening 
	listen(listening, SOMAXCONN);

	//create linked list of CONTACTS the first Cell will be the listeninig socket
	CONTACT *contacts = (CONTACT*)malloc(sizeof(CONTACT));
	contacts->s = listening;
	contacts->inGame = NULL;
	contacts->vs = NULL;
	contacts->next = contacts->prev = NULL;


	// Create the "fd_set" master file descriptor set and zero it
	fd_set master;
	FD_ZERO(&master); // at this point there are no sockets in the fd_set array
	FD_SET(listening, &master);

	bool running = true;

	while (running) {
		printf("~~~~ %d users online!!! ~~~~\n", master.fd_count - 1);

		fd_set copy = master;

		int num = select(0, &copy, nullptr, nullptr, nullptr);
		for (int i = 0; i < num; i++) {
			SOCKET s = copy.fd_array[i];
			if (s == listening) {
				printf("\ngetting new conncetion...\n");
				SOCKET player = accept(listening, nullptr, nullptr);
				FD_SET(player, &master);

				//creating new player node
				CONTACT *newCon = (CONTACT *)malloc(sizeof(CONTACT));
				newCon->s = player;
				newCon->vs = nullptr;
				newCon->inGame = false;
				addContact(contacts, newCon);

				//match with another waiting olayer
				CONTACT *vs = (CONTACT*)malloc(sizeof(CONTACT));
				vs = checkFreePlayer(newCon, contacts);
				if (vs != nullptr) {
					newCon->vs = &vs->s;
					newCon->inGame = true;
					vs->inGame = true;
					vs->vs = &player;
					send(vs->s, "1", 1, 0);
					send(player, "0", 1, 0);
				}

			}
			else {
				//sending masagge to the VS socket
				CONTACT *player = (CONTACT *)malloc(sizeof(CONTACT));
				player = findPlayerBySocket(s, contacts);
				ZeroMemory(buf, 11);
				buf_size = recv(s, buf, 11, 0);
				if (buf_size > 0) {
					buf[buf_size] = '\0';
					send(*(player->vs), buf, strlen(buf), 0);
				}
				else{
					if (buf_size < 0) 
					{
						strcpy(buf, "-1");
						send(*(player->vs), buf, strlen(buf), 0);
					}
					(player->prev)->next = player->next;
					FD_CLR(s, &master);
				}
			}
		}

	}

	return 1;
}