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

int mp::sendMessage(SOCKET &socket,Account *acc,string &str)
{
    char* cstr=NULL;
    strTocstr(str,cstr);
    CRP packet(acc,5,cstr);
    packet.sendPacket(socket);
    return 0;
}

string mp::receiveMessage(SOCKET &socket)
{
    CRP packet;
    packet.receivePacket(socket);
    if(packet.pktype()==5)
        return string(packet.data);
}

int mp::sendFile(SOCKET &socket,string filename,int stream,Account *acc)
{
    ifstream infile(filename,ios::binary|ios::in);
    if(!infile)
        return 1;

    char* cstr=NULL;
    strTocstr(filename,cstr);
    char buffer[MAXBUFFERSIZE]={0};
    char *ptr=buffer;
    int offset=-1;

    
    infile.seekg(0,ios::end);
    int totalsize=infile.tellg();

    memcpy(ptr,&stream,sizeof(int));
    memcpy(ptr+=sizeof(int),&offset,sizeof(int));
    memcpy(ptr+=sizeof(int),&totalsize,sizeof(int));
    memcpy(ptr+=sizeof(int),cstr,strlen(cstr)+1);
    CRP packet_name(acc,6,buffer);
    packet_name.sendPacket(socket); 
    //send total size and the name of the file to be sent first
    offset+=1;

    ptr=buffer+sizeof(int);
    memset(buffer,0,MAXBUFFERSIZE);
    memcpy(ptr,&offset,sizeof(int));
    ptr+=sizeof(int);
    
    infile.seekg(0,ios::beg);
    size_t MAXCONTENTSIZE=MAXBUFFERSIZE-sizeof(int)*2-1;
    while(infile.read(ptr,MAXCONTENTSIZE))
    {
        int count=infile.gcount();
        if(count<MAXCONTENTSIZE)
        {
            buffer[sizeof(int)*2+count]='\0';
        }
        buffer[MAXBUFFERSIZE-1]='\0';
        CRP packet(acc,6,buffer);
        if(packet.sendPacket(socket)!=0)
            return 2;

        offset+=count;
        ptr=buffer+sizeof(int);
        memset(buffer,0,MAXBUFFERSIZE);
        memcpy(ptr,&offset,sizeof(int));
        ptr+=sizeof(int);
    }
    return 0;
}

int mp::saveFile(SOCKET &socket,vector<string> streams)
{
    char buffer[MAXBUFFERSIZE]={0};
    

    string filename;
    fstream file;
    int totalsize=0;
    int savedsize=0;

    while(savedsize<totalsize || (totalsize==0 && savedsize==0))
    {
        CRP packet;
        packet.receivePacket(socket);
        int stream=0,offset=-2;
        char* ptr=packet.data;
        memcpy(&stream,ptr,sizeof(int));
        memcpy(&offset,ptr+=sizeof(int),sizeof(int));

        if(stream>streams.size() && offset==-1)
        {
            memcpy(&totalsize,ptr+=sizeof(int),sizeof(int));
            ptr+=sizeof(int);
            filename=string(ptr);
            streams.push_back(filename);
            ofstream out(filename,ios::out);
            out.close();
        }
        if(stream<streams.size() && offset>=0)
        {
            file.open(streams[stream],ios::binary|ios::in|ios::out);
            file.seekp(offset,ios::beg);

            ptr+=sizeof(int);
            int segsize=strlen(ptr);
            file.write(ptr,segsize);
            savedsize+=segsize;
        }
    }
    file.close();
    return 0;
}