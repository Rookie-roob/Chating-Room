#ifndef CRP_H
#define CRP_H

#include <Winsock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include "Account.h"
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

CRP::CRP(Account *acc,int type,char* data) :
    id(acc->id),psw(acc->password),type(type)
{
    size_t len=strlen(data);
    this->data=new char[len+1];
    strcpy(this->data,data);
}

CRP::~CRP()
{
    if(this->data)
        delete [] this->data;
}

Account CRP::registerAccount(unsigned int id)
{
    return Account(id,this->psw);
}

int CRP::sendPacket(SOCKET &socket)
{
    size_t suint=sizeof(unsigned int),
           sint=sizeof(int),
           sdata=strlen(data)+1; //包括结尾的\0,这将被用于分割不同的报文
    char *buffer=new char[suint*2+sint+sdata];
    char *ptr=buffer;
    memcpy(ptr,&id,suint);
    ptr+=suint;
    memcpy(ptr,&psw,suint);
    ptr+=suint;
    memcpy(ptr,&type,sint);
    ptr+=sint;
    memcpy(ptr,data,sdata);
    if(send(socket,buffer,suint*2+sint+sdata,0))
        return 0;
    return 1;
}
int CRP::receivePacket(SOCKET &socket)
{
    size_t suint=sizeof(unsigned int),
           sint=sizeof(int),
           sdata=MAXBUFFERSIZE+1;
    char *temp=new char[1];
    temp[0]='\0';

    while(sdata>MAXBUFFERSIZE)
    {
        char prebuffer[MAXBUFFERSIZE]={0};
        recv(socket,prebuffer,MAXBUFFERSIZE,MSG_PEEK); //预读
        sdata=strlen(prebuffer)+1;

        size_t size=sdata>MAXBUFFERSIZE ? MAXBUFFERSIZE:sdata;
        char *buffer=new char[size+1];
        recv(socket,buffer,size,0);
        buffer[size]='\0';
        char *p=temp;
        size_t sizep=strlen(p);
        temp=new char[sizep+size+1];
        for(int i=0;i<sizep;i++)
        {
            temp[i]=p[i];
        }
        for(int i=sizep;i<sizep+size+1;i++)
        {
            temp[i]=buffer[i-sizep];
        }
        delete [] p;
        delete [] buffer;
    }
    char *ptr=temp;
    memcpy(&id,ptr,suint);
    ptr+=suint;
    memcpy(&psw,ptr,suint);
    ptr+=suint;
    memcpy(&type,ptr,sint);
    ptr+=sint;

    sdata=strlen(temp)-suint*2-sint+1;
    data=new char[sdata];
    memcpy(data,ptr,sdata);

    return 0;
}

#endif