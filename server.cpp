#include <iostream>
#include <fstream>
#include <Winsock2.h>
#include <WS2tcpip.h> //inet_pton() is defined in here
#include <Windows.h>
#include "Account.h"
#include "AccountReg.h"
#include "MessageProc.h"
#pragma comment (lib,"ws2_32.lib")
using namespace std;




SOCKET client;
int sendMessage(char *str,int size)
{
    if(send(client,str,size,0))
    {
        cout<<"---------------------you"<<endl;
        cout<<"                     "<<str<<endl;
        return 0;
    }
    return 1;
}
DWORD WINAPI receiveMessage(LPVOID lpvThreadParam)
{
    while (1)
    {
        // char szBuffer[MAXBYTE] = { 0 };
        // int ret = recv(client, szBuffer, MAXBYTE, NULL);
        // if (ret == SOCKET_ERROR)
        //     return 0;
        string str=mp::receiveMessage(client);
        cout<<"---------------------client"<<endl;
        cout<<"                     "<<str<<endl;
    }
    return 0;
}

int main()
{
    initial();
    // newAccount(1,123456,"Ming");
    // newAccount(2,123456,"jackeylove");
    // newAccount(3,123456,"Xiaohu");
    // newFriend(1,3);
    // newFriend(1,2);
    // Account *acc=getAccountInfoByID(1);
    // getAccountInfoByID(2)->changeState();
    // showFriendState(acc);
    showAllAccount();

	WSADATA wsaData;
    int s = WSAStartup(MAKEWORD(2, 0), &wsaData);
	
    int error_rec=0;
    const char* ip_addr="127.0.0.1";
	SOCKET server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	sockaddr_in sock_addr;
    memset(&sock_addr, 0, sizeof(sock_addr));
    sock_addr.sin_family = AF_INET;
    error_rec=inet_pton(AF_INET, ip_addr,&sock_addr.sin_addr.s_addr);
    sock_addr.sin_port=htons(20);
    bind(server, (SOCKADDR*)&sock_addr, sizeof(SOCKADDR));
    listen(server, 20);

	SOCKADDR clientAddr;
    int nSize = sizeof(SOCKADDR);
    client = accept(server, (SOCKADDR*)&clientAddr, &nSize);
    if (client == SOCKET_ERROR)
    {
        cout << "connect error" << endl;
        WSACleanup();
        return 0;
    }
	cout << "connect with client successfully" << endl;
	
    Account acc(1,123456);
    /*HANDLE handle = CreateThread(NULL, 0, receiveMessage, NULL, 0, NULL);*/
    while(1)
    {
        /* char send_str[100];
         cin.getline(send_str,100);
         sendMessage(send_str,strlen(send_str));*/

        mp::saveFile(client);
    }

    /*CloseHandle(handle);*/
    closesocket(client);
    closesocket(server);
    WSACleanup();
    return 0;
}