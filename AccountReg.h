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

Account* getAccountInfoByID(unsigned int id);

void showFriendState(Account *acc);

int initial();

int newAccount(unsigned int id,unsigned int psw,char* nickname);

int newFriend(unsigned int myid,unsigned int friendid);

void showAllAccount();

#endif