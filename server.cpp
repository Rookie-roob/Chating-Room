#include <iostream>
#include <fstream>
#include <Winsock2.h>
#include <WS2tcpip.h> //inet_pton() is defined in here
#include <Windows.h>
#include <atlstr.h>
#include "Account.h"
#pragma comment (lib,"ws2_32.lib")
using namespace std;

const size_t BlockSize=sizeof(unsigned int)*2+sizeof(char [20])+sizeof(unsigned int)+
                        sizeof(unsigned int)*10;

class AccountReg{
public:
    Account account;
    sockaddr ipaddr;
    int port;

    AccountReg(Account account) : account(account){}
};
vector<AccountReg> accounts; //used to keep all accounts information and link to ip address
int accountnum=0; //record the number of accounts


Account* getAccountInfoByID(unsigned int id)
{
    for(int i=0;i<accountnum;i++)
    {
        if(accounts[i].account.getID()==id)
        {
            return &accounts[i].account;
        }
    }
    return NULL;
}
void showFriendState(Account *acc)
{
    cout<<acc->getNickname()<<" 's friends"<<endl;
    cout<<"Friend's Nick Name        State"<<endl;
    for(int i=0;i<acc->getFriendnum();i++)
    {
        Account *friends=getAccountInfoByID(acc->getFriendID(i));
        cout<<friends->getNickname()<<"                     ";
        if(friends->showState()==0)
            cout<<"offline"<<endl;
        else
            cout<<"online"<<endl;
    }
}

int initial()
{
    ifstream infile("datafile.data",ifstream::binary|ifstream::in);
    if(!infile)
    {
        cout<<"fail to open datafile.data"<<endl;
        return 1;
    }

    infile.seekg(0,ios::beg);
    infile.read(reinterpret_cast<char*>(&accountnum),sizeof(int));
    for(int i=0;i<accountnum;i++)
    {
        infile.seekg(sizeof(int)+BlockSize*i,ios::beg);
        unsigned int id=0,psw=0,friendnum=0;
        char nick[20]={0};
        infile.read(reinterpret_cast<char*>(&id),sizeof(unsigned int));
        infile.read(reinterpret_cast<char*>(&psw),sizeof(unsigned int));
        infile.read(nick,sizeof(char[20]));
        Account acc(id,psw,nick);
        infile.read(reinterpret_cast<char*>(&friendnum),sizeof(unsigned int));
        for(size_t j=0;j<friendnum;j++)
        {
            int friendid=0;
            infile.read(reinterpret_cast<char*>(&friendid),sizeof(unsigned int));
            acc.loadFriendList(friendid);
        }
        accounts.push_back(AccountReg(acc));
    }
    cout<<"initialize over"<<endl;
    infile.close();
    return 0;
}

int newAccount(unsigned int id,unsigned int psw,char* nickname)
{
    Account acc(id,psw,nickname);
    accounts.push_back(AccountReg(acc));
    accountnum++;

    fstream file("datafile.data",fstream::binary|fstream::out|fstream::in);
    if(!file)
    {
        cout<<"fail to open datafile.data"<<endl;
        return 1;
    }    
    
    char buffer[BlockSize]={0};
    memcpy(buffer,&accountnum,sizeof(int));
    file.seekp(0,ios::beg);
    file.write(buffer,sizeof(int));

    char *ptr=buffer;
    memcpy(ptr,&id,sizeof(unsigned int));
    memcpy(ptr+=sizeof(unsigned int),&psw,sizeof(unsigned int));
    memcpy(ptr+=sizeof(unsigned int),nickname,strlen(nickname));
    ptr+=sizeof(char [20]);
    unsigned int temp=0;
    for(int i=0;i<11;i++)
    {
        memcpy(ptr,&temp,sizeof(unsigned int));
        ptr+=sizeof(unsigned int);
    }
    
    file.seekp(0,ios::end);
    file.write(buffer,BlockSize);
    file.close();
    return 0;
}
int newFriend(unsigned int myid,unsigned int friendid)
{
    Account *acc=getAccountInfoByID(myid);
    if(acc==NULL)
    {
        cout<<"myid does not exit"<<endl;
        return 3;
    }
    if(acc->getFriendnum()>=10)
    {
        cout<<"number of friends should not be more than 10"<<endl;
        return 2;
    }
    acc->loadFriendList(friendid);

    fstream file("datafile.data",fstream::binary|fstream::in|fstream::out);
    if(!file)
    {
        cout<<"fail to open datafile.data"<<endl;
        return 1;
    }

    for(int i=0;i<accountnum;i++)
    {
        file.seekg(sizeof(int)+BlockSize*i,ios::beg);
        unsigned int id=0;
        file.read(reinterpret_cast<char*>(&id),sizeof(unsigned int));
        if(id==myid) 
        {
            file.seekg(sizeof(unsigned int)*2,ios::cur);
            int friendnum=0;
            file.read(reinterpret_cast<char*>(&friendnum),sizeof(unsigned int));
            file.seekp(sizeof(unsigned int)*friendnum,ios::cur);
            file.write(reinterpret_cast<char*>(&friendid),sizeof(unsigned int));
            file.close();
            return 0;
        }
    }
    return 3;
}

void showAllAccount()
{
    cout<<"all accounts"<<endl;
    cout<<"--------------------------------------------------"<<endl;
    cout<<"ID      Nick Name"<<endl;
    for(int i=0;i<accountnum;i++)
    {
        cout<<accounts[i].account.getID()<<"   "<<accounts[i].account.getNickname()<<endl;
    }
    cout<<"--------------------------------------------------"<<endl;
}


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
        char szBuffer[MAXBYTE] = { 0 };
        int ret = recv(client, szBuffer, MAXBYTE, NULL);
        if (ret == SOCKET_ERROR)
            return 0;
        cout<<"---------------------client"<<endl;
        cout<<"                     "<<szBuffer<<endl;
    }
    return 0;
}

int main()
{
    initial();
    // newAccount(1,123456,"Ming");
    // newAccount(2,123456,"jackeylove");
    // newAccount(3,123456,"Xiaohu");
    newFriend(1,3);
    newFriend(1,2);
    Account *acc=getAccountInfoByID(1);
    getAccountInfoByID(2)->changeState();
    showFriendState(acc);
    showAllAccount();

	// WSADATA wsaData;
    // int s = WSAStartup(MAKEWORD(2, 0), &wsaData);
	
    // int error_rec=0;
    // char* ip_addr="127.0.0.1";
	// SOCKET server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	// sockaddr_in sock_addr;
    // memset(&sock_addr, 0, sizeof(sock_addr));
    // sock_addr.sin_family = AF_INET;
    // error_rec=inet_pton(AF_INET, ip_addr,&sock_addr.sin_addr.s_addr);
    // sock_addr.sin_port=htons(20);
    // bind(server, (SOCKADDR*)&sock_addr, sizeof(SOCKADDR));
    // listen(server, 20);

	// SOCKADDR clientAddr;
    // int nSize = sizeof(SOCKADDR);
    // client = accept(server, (SOCKADDR*)&clientAddr, &nSize);
    // if (client == SOCKET_ERROR)
    // {
    //     cout << "connect error" << endl;
    //     WSACleanup();
    //     return 0;
    // }
	// cout << "connect with client successfully" << endl;
	
    // HANDLE handle = CreateThread(NULL, 0, receiveMessage, NULL, 0, NULL);
    // while(1)
    // {
    //     char send_str[100];
    //     cin.getline(send_str,100);
    //     sendMessage(send_str,strlen(send_str));
    // }

    // CloseHandle(handle);
    // closesocket(client);
    // closesocket(server);
    // WSACleanup();
    return 0;
}