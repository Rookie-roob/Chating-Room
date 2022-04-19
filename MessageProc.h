//�����˿ͻ��˿���ʹ�õ������Լ��������ṩ�ķ���

#ifndef MESSAGEPROC_H
#define MESSAGEPROC_H

#include <iostream>
#include <string>
#include <thread>
#include <fstream>
#include <Winsock2.h>
#include <WS2tcpip.h>
#include "Account.h"
#include "CRP.h"
//#include <Windows.h>
using namespace std;

int sendMessage(SOCKET &socket,Account *acc,string &str); //client

string receiveMessage(); //client

int sendFile(string filename); //client or server

int saveFile(); //client or server

int signInReq(); //client
int signInRpy(); //server

int signOutReq(); //client
int signOutRpy(); //server

int registerReq(); //client
int registerRpy(); //server

int communicateFriendReq(thread *threads); //client
int communicateFriendRpy(); //server ��ͬʱ����������Ҫ�������ӵ�������

int getFriendsStateReq(); //client
int getFriendsStateRpy(); //server

int clientReqHandler(thread *threads); //server ��ÿ��һ���ͻ��˶��½�һ�������̣߳�


#endif