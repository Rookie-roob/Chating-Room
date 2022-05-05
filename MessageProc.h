//this header defines available service provided by server and available requests clients can use


#ifndef MESSAGEPROC_H
#define MESSAGEPROC_H

#include <iostream>
#include <string>
#include <thread>
#include <fstream>
#include <Winsock2.h>
#include <WS2tcpip.h>
//#include "Account.h"
//#include "AccountReg.h"
#include "CRP.h"
//#include <Windows.h>

using namespace std;

namespace mp{
int sendMessage(SOCKET &socket,Account *acc,string &str); //client

string receiveMessage(SOCKET &socket); //client

int sendFile(SOCKET &socket,string filename,Account *acc); //client or server

int saveFile(SOCKET &socket); //client or server

int signInReq(SOCKET &socket,Account &acc); //client
int signInRpy(SOCKET &socket,AccountTable &accountable); //server

int signOutReq(SOCKET &socket,Account &acc); //client
int signOutRpy(SOCKET& socket, AccountTable& accountable); //server

unsigned int registerReq(SOCKET &socket,string nickname,unsigned int psw); //client
int registerRpy(SOCKET &socket, AccountTable& accountable); //server

int communicateFriendReq(thread *threads, SOCKET &socket, Account &acc); //client
int communicateFriendRpy(SOCKET& socket, AccountTable& accountable);
//server (send to both of the two client hosts which want to connect with each other)

int getFriendsStateReq(SOCKET& socket, Account& acc); //client
int getFriendsStateRpy(SOCKET& socket, AccountTable& accountable); //server

int clientReqHandler(thread *threads); //server £¨new thread to handle client requests£©

}

#endif