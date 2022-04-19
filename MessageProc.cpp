#include "MessageProc.h"

int strTocstr(string &str,char* &cstr)
{
    if(cstr!=NULL)
        return 1;
    size_t strsize=str.size();
    cstr=new char[strsize+1];
    strcpy(cstr,str.c_str());
    return 0;
}

int sendMessage(SOCKET &socket,Account *acc,string &str)
{
    char* cstr=NULL;
    strTocstr(str,cstr);
    CRP packet(acc,5,cstr);
    packet.sendPacket(socket);
}