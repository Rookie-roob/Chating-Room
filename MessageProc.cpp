#include "MessageProc.h"

char* strTocstr(string &str)
{
    int strsize=str.size();
    char* cstr=new char[strsize+1];
    strcpy(cstr,str.c_str());
    return cstr;
}

string getfilename(string file)
{
    int size = file.size();
    char name[101] = { 0 }; //length won't be very long usually
    int i, len = 0;
    for (i = size - 1;i >= 0;i--)
    {
        if (file[i] == '/')
            break;
        name[100 - (size - i)] = file[i];
        len++;
    }
    char* p = name + 100 - len;
    return string(p);
}

int mp::sendMessage(SOCKET &socket,Account *acc,string &str)
{
    char* cstr=strTocstr(str);
    CRP packet(acc,5,cstr);
    packet.sendPacket(socket);
    return 0;
}

string mp::receiveMessage(SOCKET &socket)
{
    CRP packet;
    packet.receivePacket(socket);
    if (packet.type == 5)
    {
        packet.data[packet.datalen] = '\0';
        return string(packet.data);
    }
}

int mp::sendFile(SOCKET& socket, string filename, Account* acc)
{
    ifstream infile(filename, ios::binary | ios::in);
    if (!infile)
        return 1;

    string filenameNoPath = getfilename(filename);
    char* cstr = strTocstr(filenameNoPath);
    char buffer[MAXBUFFERSIZE] = { 0 };
    char* ptr = buffer;
    int offset = -1;


    infile.seekg(0, ios::end);
    int totalsize = infile.tellg();

    memcpy(ptr, &offset, sizeof(int));
    memcpy(ptr += sizeof(int), &totalsize, sizeof(int));
    memcpy(ptr += sizeof(int), cstr, strlen(cstr) + 1);
    CRP packet_name(acc, 6, buffer, sizeof(int) * 2 + strlen(cstr));
    packet_name.sendPacket(socket);
    //send total size and the name of the file to be sent first
    offset += 1;

    if (cstr) delete[] cstr;

    ptr = buffer;
    memset(buffer, 0, MAXBUFFERSIZE);
    memcpy(ptr, &offset, sizeof(int));
    ptr += sizeof(int);

    infile.seekg(0, ios::beg);
    size_t MAXCONTENTSIZE = MAXBUFFERSIZE - sizeof(int) - 1;
    while (infile.read(ptr, MAXCONTENTSIZE))
    {
        int count = infile.gcount();
        if (count < MAXCONTENTSIZE)
        {
            buffer[sizeof(int) + count] = '\n';
        }
        buffer[MAXBUFFERSIZE - 1] = '\n';
        CRP packet(acc, 6, buffer, sizeof(int) + count);
        if (packet.sendPacket(socket) != 0)
            return 2;

        offset += count;
        ptr = buffer;
        memset(buffer, 0, MAXBUFFERSIZE);
        memcpy(ptr, &offset, sizeof(int));
        ptr += sizeof(int);
    }
    return 0;
}

int mp::saveFile(SOCKET &socket)
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
        int offset=-2;
        char* ptr=packet.data;
        memcpy(&offset,ptr,sizeof(int));

        if(offset==-1)
        {
            memcpy(&totalsize,ptr+=sizeof(int),sizeof(int));
            ptr+=sizeof(int);
            char filenameNoPath[100] = { 0 };
            memcpy(filenameNoPath, ptr, packet.datalen - sizeof(int) * 2);
            filename=string(filenameNoPath);
            cout << filename << endl;
            ofstream out(filename,ios::out);
            out.close();
        }
        else
        {
            file.open(filename,ios::binary|ios::in|ios::out);
            file.seekp(offset,ios::beg);

            ptr+=sizeof(int);
            int segsize=packet.datalen - sizeof(int);
            file.write(ptr,segsize);
            savedsize+=segsize;
        }
    }
    file.close();
    return 0;
}

unsigned int mp::registerReq(SOCKET &socket,string nickname,unsigned int psw)
{
    char* cstr=strTocstr(nickname);
    Account acc(0,psw);
    CRP packet(&acc,0,cstr);
    packet.sendPacket(socket);

    delete [] cstr;

    CRP packet_rpy;
    packet_rpy.receivePacket(socket);
    if(packet.type==0)
    {
        return packet.id;
    }
}

Account mp::registerRpy(SOCKET &socket,unsigned int id)
{
    CRP packet;
    packet.receivePacket(socket);

    Account acc=packet.registerAccount(id);
    acc.setNickname(packet.data);

    CRP packet_rpy(&acc,0);
    packet_rpy.sendPacket(socket);

    return acc;
}

int mp::signInReq(SOCKET &socket,Account &acc)
{
    CRP packet(&acc,1);
    packet.sendPacket(socket);

    CRP packet_rpy;
    packet_rpy.receivePacket(socket);
    if(packet_rpy.type==1)
    {
        if(packet_rpy.data && packet_rpy.data[0]=='t')
        {
            cout<<"sign in successfully"<<endl;
            return 0;
        }
        else if(packet_rpy.data && packet_rpy.data[0]=='f')
        {
            cout<<"id or password wrong"<<endl;
            return 0;
        }
    }
}

int mp::signInRpy(SOCKET &socket,AccountReg &accounts)
{
    return 0;
}