#ifndef ONLINE_H
#define ONLINE_H

#include <WinSock2.h>
#include <windows.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include "utils.h"

#pragma comment(lib, "ws2_32.lib")

extern WSADATA wsaData;

extern SOCKET sock;
extern SOCKET otherSock;
extern SOCKET discoverySock;

extern SOCKADDR_IN addr;
extern SOCKADDR_IN otherAddr;
extern SOCKADDR_IN discoveryAddr;

void ErrExit(const char* msg);
  
void InitWinSock();

void InitServer(int port, int discoveryPort);

void ServerListenDiscovery();

void AcceptClient();

void CleanServer();

void InitClient(int discoveryPort);

int DiscoverServer(u_long* addr);

void ConnectToServer(u_long addr, int port);

void CleanClient();

void SendGameState(int** board, int score, int linesCleared, int level, int nextPiece, int holdPiece, int rubbish, int sentRubbish, int running, int isServer, int length, int width);

int ReceiveGameState(int** board, int* score, int* linesCleared, int* level, int* nextPiece, int* holdPiece, int* rubbish, int* receivedRubbish, int isServer, int length, int width);

void ConnectAsServer();

void ConnectAsClient();

#endif