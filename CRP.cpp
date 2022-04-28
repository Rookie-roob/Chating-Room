#include "CRP.h"

//this version is for string transmission only,packet data contains '\0'
CRP::CRP(Account *acc,int type,char* data) :
    id(acc->id),psw(acc->password),type(type),datalen(0)
{
    this->datalen=strlen(data) + 1;
    this->data=new char[this->datalen];
    strcpy(this->data,data);
}

CRP::CRP(Account* acc, int type, char* data, unsigned long long datalen) :
    id(acc->id),psw(acc->password),type(type),datalen(datalen)
{
    this->data = new char[datalen];
    memcpy(this->data, data, datalen);
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
           sdata=this->datalen,
           slen = sizeof(unsigned long long);
    size_t packetlen = slen + suint * 2 + sint + sdata;

    char *buffer=new char[packetlen+2];
    char *ptr=buffer;
    buffer[0] = '\n';
    memcpy(++ptr, &packetlen, slen);
    memcpy(ptr+=slen,&id,suint);
    memcpy(ptr+=suint,&psw,suint);
    memcpy(ptr += suint,&type,sint);
    memcpy(ptr+=sint,data,sdata);
    buffer[packetlen + 1] = '\n';
    if(send(socket, buffer, packetlen + 2, 0))
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
    size_t slen = sizeof(unsigned long long);
    char* buffer = NULL;

    char preview[MAXBUFFERSIZE + 100]={0};
    int ret = recv(socket,preview,MAXBUFFERSIZE + 100,MSG_PEEK); //Ô¤¶Á

    while (ret!=0 && ret!=SOCKET_ERROR)
    {
        if (preview[0] == '\n')
        {
            char* p = preview + 1;
            unsigned long long packetlen = 0;
            memcpy(&packetlen, p, slen);
            if (preview[packetlen + 1] == '\n')
            {
                buffer = new char[packetlen + 2];
                ret = recv(socket, buffer, packetlen + 2, 0);

                datalen = packetlen - slen - suint * 2 - sint;
                data = new char[datalen];
                p = buffer + 1 + slen;
                memcpy(&id, p, suint);
                memcpy(&psw, p += suint, suint);
                memcpy(&type, p += suint, sint);
                memcpy(data, p += sint, datalen);

                if (buffer) delete [] buffer;
                return 0;
            }
            else
            {
                char x[1];
                recv(socket, x, 1, 0);
                ret = recv(socket, preview, MAXBUFFERSIZE + 100, MSG_PEEK);
            }
        }
        else
        {
            char x[1];
            recv(socket, x, 1, 0);
            ret = recv(socket, preview, MAXBUFFERSIZE + 100, MSG_PEEK);
        }
    }
    return 1;
}