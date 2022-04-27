#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <iostream>
#include <vector>
#include <string>

class CRP;

class Account
{
private:
    unsigned int id;
    unsigned int password;
    char nick[20];
    bool online;
    std::vector<unsigned int> friendList;
public:
    Account(unsigned int id,unsigned int psw,char *nick=NULL,bool online=false);
    ~Account();
    unsigned int getID();
    char* getNickname();
    bool checkPassword(unsigned int psw);
    void setNickname(char *nick);
    bool changeState();
    bool showState();
    unsigned int getFriendnum();
    void loadFriendList(unsigned int friendid); //use this function to load the information of friend list from data file
    unsigned int getFriendID(int key);

    friend class CRP;
};


#endif