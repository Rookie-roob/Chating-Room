//this header is mainly used to manage all accounts on server

#ifndef ACCOUNTREG_H
#define ACCOUNTREG_H

#include <iostream>
#include <fstream>
#include <WinSock2.h>
#include "Account.h"
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

class AccountTable {
public:
    vector<AccountReg> accounts;
    int accountnum;

    Account* getAccountInfoByID(unsigned int id);

    int findAccount(unsigned int id); //find account by id and check the password meanwhile,if found return the index,else return -1

    void recordIPAddress(int index); //record the ip address of client: accounts[index]

    void showFriendState(Account* acc);

    int initial();

    int newAccount(unsigned int id, unsigned int psw, char* nickname);

    int newFriend(unsigned int myid, unsigned int friendid);

    void showAllAccount();
};

#endif