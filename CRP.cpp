#include "CRP.h"

CRP::CRP(Account *acc,int type,char* data) :
    id(acc->id),psw(acc->password),type(type),datalen(0)
{
    this->datalen=strlen(data);
    this->data=new char[this->datalen+1];
    strcpy(this->data,data);
}

CRP::CRP(Account* acc, int type, char* data, unsigned long long datalen) :
    id(acc->id),psw(acc->password),type(type),datalen(datalen)
{
    this->data = new char[datalen + 1];
    memcpy(this->data, data, datalen + 1);
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
           sdata=this->datalen + 1; //包括结尾的\n,这将被用于分割不同的报文
    char *buffer=new char[suint*2+sint+sdata];
    char *ptr=buffer;
    memcpy(ptr,&id,suint);
    ptr+=suint;
    memcpy(ptr,&psw,suint);
    ptr+=suint;
    memcpy(ptr,&type,sint);
    ptr+=sint;
    memcpy(ptr,data,sdata);
    buffer[suint * 2 + sint + sdata - 1] = '\n';
    if(send(socket,buffer,suint*2+sint+sdata,0))
        return 0;
    return 1;
}

//return the position of possible \n
int readline(char buffer[MAXBUFFERSIZE])
{
    int i;
    for (i = 0;i < MAXBUFFERSIZE;i++)
    {
        if (buffer[i] == '\n')
            break;
    }
    if (buffer[i] == '\n')
        return i;
    else
        return MAXBUFFERSIZE;
}
int CRP::receivePacket(SOCKET &socket)
{
    size_t suint=sizeof(unsigned int),
           sint=sizeof(int),
           sdata=MAXBUFFERSIZE+1;
    char *temp=new char[1];
    temp[0]='\n';

    while(sdata>MAXBUFFERSIZE)
    {
        char prebuffer[MAXBUFFERSIZE]={0};
        recv(socket,prebuffer,MAXBUFFERSIZE,MSG_PEEK); //预读
        sdata=readline(prebuffer)+1;
        
        int size = sdata;
        char *buffer=new char[size+1];
        recv(socket,buffer,size,0);
        buffer[size]='\n';
        char *p=temp;
        size_t sizep=readline(p);
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

    sdata=readline(temp)-suint*2-sint;
    data=new char[sdata];
    memcpy(data,ptr,sdata);

    datalen = sdata;

    return 0;
}