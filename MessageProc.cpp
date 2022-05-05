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
    size_t MAXCONTENTSIZE = MAXBUFFERSIZE - sizeof(int);
    while (1)
    {
        infile.read(ptr, MAXCONTENTSIZE);
        int count = infile.gcount();
        if (count == 0)
        {
            infile.close();
            return 0;
        }
        CRP packet(acc, 6, buffer, sizeof(int) + count);
        if (packet.sendPacket(socket) != 0)
            return 2;

        offset += count;
        ptr = buffer;
        memset(buffer, 0, MAXBUFFERSIZE);
        memcpy(ptr, &offset, sizeof(int));
        ptr += sizeof(int);

        cout << offset << " bytes have sent,total size: " << totalsize << endl;
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

            cout << savedsize << " bytes have saved,total size: " << totalsize << endl;
            file.close();
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

    if (cstr) delete[] cstr;

    CRP packet_rpy;
    packet_rpy.receivePacket(socket);
    if(packet.type==0)
    {
        cout << "register successfully" << endl;
        return packet_rpy.id;
    }
    return 0;
}

int mp::registerRpy(SOCKET &socket,AccountTable &accountable)
{
    CRP packet;
    packet.receivePacket(socket);

    accountable.newAccount(accountable.accountnum + 1, packet.psw, packet.data);

    CRP packet_rpy(&accountable.accounts[accountable.accountnum-1].account,0);
    packet_rpy.sendPacket(socket);

    return 0;
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

            size_t suint = sizeof(unsigned int);
            char* p = packet_rpy.data + 1;
            unsigned int friendnum;

            char name[20] = { 0 };
            memcpy(name, p, 20);
            memcpy(&friendnum, p += 20, suint);
            acc.setNickname(name);
            p += suint;

            for (int i = 0;i < friendnum;i++)
            {
                unsigned int friendid;
                char name[20] = { 0 }; 
                bool state = false; 
                memcpy(&friendid, p, suint);
                memcpy(name, p += suint, 20);
                p += 20;
                if (p[0] == 'y')
                    state = true;

                acc.friendinfoList.push_back(friendinfo(friendid,name,state));
                p++;
            }
            return 0;
        }
        else if(packet_rpy.data && packet_rpy.data[0]=='f')
        {
            cout<<"id or password wrong"<<endl;
            return 0;
        }
    }
}

int mp::signInRpy(SOCKET &socket,AccountTable &accountable)
{
    CRP packet;
    packet.receivePacket(socket);
    int index = accountable.findAccount(packet.id);
    if (index < 0)
    {
        Account acc(packet.id, packet.psw);
        char data[2];
        data[0] = 'f';data[1] = '\0';
        CRP packet_rpy(&acc,1,data);
        packet_rpy.sendPacket(socket);
        return 0;
    }
    else
    {
        if (accountable.accounts[index].account.checkPassword(packet.psw))
        {
            Account *account = &accountable.accounts[index].account;
            account->changeState(); //sign in successfully,change state: online
            int addrsize = sizeof(sockaddr_in);
            accountable.recordIPAddress(index, socket); //record ip address

            unsigned int friendnum = account->getFriendnum();
            //t or f;nickname;friendnum;friendlist:friend id,friend name,state;
            int datalen = 1 + 20 + sizeof(unsigned int) + friendnum * (sizeof(unsigned int) + 20 + 1);
            char* data = new char[datalen];
            memset(data, 0, datalen);
            char* p = data + 1;
            data[0] = 't';
            memcpy(p, account->getNickname(), 20);
            memcpy(p += 20, &friendnum, sizeof(unsigned int));
            p += sizeof(unsigned int);

            for (int i = 0;i < friendnum;i++)
            {
                unsigned int friendid = account->friendList[i];
                Account* friendacc = accountable.getAccountInfoByID(friendid);
                char* name = friendacc->getNickname();
                char state;
                if (friendacc->showState())
                    state = 'y';
                else
                    state = 'n';
                memcpy(p, &friendid, sizeof(unsigned int));
                memcpy(p += sizeof(unsigned int), name, 20);
                memcpy(p += 20, &state, 1);
                p++;
            }

            CRP packet_rpy(account, 1, data, datalen);
            packet_rpy.sendPacket(socket);
            return 0;
        }
        else
        {
            Account acc(packet.id, packet.psw);
            char data[2];
            data[0] = 'f';data[1] = '\0';
            CRP packet_rpy(&acc, 1, data);
            packet_rpy.sendPacket(socket);
            return 0;
        }
    }
    return 0;
}

int mp::signOutReq(SOCKET& socket, Account& acc)
{
    CRP packet(&acc, 2);
    packet.sendPacket(socket);

    CRP packet_rpy;
    packet_rpy.receivePacket(socket);
    if (packet_rpy.data[0] == 't')
    {
        cout << "sign out successfully" << endl;
        return 0;
    }
    else
    {
        cout << "sign out error" << endl;
        return 0;
    }
    return 0;
}

int mp::signOutRpy(SOCKET& socket, AccountTable& accountable)
{
    CRP packet;
    packet.receivePacket(socket);
    int index = accountable.findAccount(packet.id);
    if (index < 0)
    {
        Account acc(packet.id, packet.psw);
        char data[2];
        data[0] = 'f';data[1] = '\0';
        CRP packet_rpy(&acc, 2, data);
        packet_rpy.sendPacket(socket);
        return 0;
    }
    else
    {
        accountable.accounts[index].account.changeState(); //sign out successfully,change state: offline
        if (accountable.accounts[index].ipaddr)
            delete accountable.accounts[index].ipaddr;

        Account acc(packet.id, packet.psw);
        char data[2];
        data[0] = 't';data[1] = '\0';
        CRP packet_rpy(&acc, 2, data);
        packet_rpy.sendPacket(socket);
        return 0;
    }
    return 0;
}

int mp::getFriendsStateReq(SOCKET& socket, Account& acc)
{
    CRP packet(&acc, 3);
    packet.sendPacket(socket);

    CRP packet_rpy;
    packet_rpy.receivePacket(socket);
    if (packet_rpy.data[0] != 'f')
    {
        size_t suint = sizeof(unsigned int);
        char* p = packet_rpy.data;
        unsigned int friendnum;
        memcpy(&friendnum, p, suint);
        p += suint;

        for (int i = 0;i < friendnum;i++)
        {
            bool state = false;
            if (p[0] == 'y')
                state = true;

            acc.friendinfoList[i].state = state;
            p++;
        }
        return 0;
    }
    return 0;
}

int mp::getFriendsStateRpy(SOCKET& socket, AccountTable& accountable)
{
    CRP packet;
    packet.receivePacket(socket);
    int index = accountable.findAccount(packet.id);
    if (index < 0)
    {
        Account acc(packet.id, packet.psw);
        char data[2];
        data[0] = 'f';data[1] = '\0';
        CRP packet_rpy(&acc, 3, data);
        packet_rpy.sendPacket(socket);
        return 0;
    }
    else
    {
        Account *account = &accountable.accounts[index].account;
        int friendnum = account->getFriendnum();
        //friendnum;friendinfolist:state
        int datalen = sizeof(unsigned int) + friendnum;
        char* data = new char[datalen];
        char* p = data;
        memcpy(p, &friendnum, sizeof(unsigned int));
        p += sizeof(unsigned int);

        for (int i = 0;i < friendnum;i++)
        {
            unsigned int friendid = account->friendList[i];
            Account *friendacc = accountable.getAccountInfoByID(friendid);
            char state;
            if (friendacc->showState())
                state = 'y';
            else
                state = 'n';
            memcpy(p, &state, 1);
            p++;
        }

        CRP packet_rpy(account, 3, data, datalen);
        packet_rpy.sendPacket(socket);

        return 0;
    }
    return 0;
}

int mp::communicateFriendReq(thread* threads, SOCKET& socket, Account& acc)
{
    return 0;
}

int mp::communicateFriendRpy(SOCKET& socket, AccountTable& accountable)
{
    return 0;
}