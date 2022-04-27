
#ifndef CRP_H
#define CRP_H

#include <Winsock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include "AccountReg.h"
#pragma comment (lib,"ws2_32.lib")


const size_t MAXBUFFERSIZE=1024;

class CRP //chating room packet
{    
private:
    unsigned int psw;
public:
    //0 register new account; 1 log in; 2 log out; 3 show friend list; 4 connect with friend
    //5 message; 6 file transport
    int type; //packet type
    unsigned int id;
    char* data; 
    CRP()=default;
    CRP(Account *acc,int type,char* data=NULL);
    ~CRP();

    Account registerAccount(unsigned int id);
    int sendPacket(SOCKET &socket);
    int receivePacket(SOCKET &socket);
};

#endif