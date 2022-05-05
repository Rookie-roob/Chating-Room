#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <iostream>
#include <vector>
#include <string>

class CRP;

struct friendinfo {
    unsigned int friendid;
    char name[20];
    bool state;

    friendinfo(unsigned int id, char name[20], bool state) :
        friendid(id), state(state)
    {
        strcpy(this->name, name);
    }
};

class Account
{
private:
    unsigned int id;
    unsigned int password;
    char nick[20];
    bool online;
public:
    Account(unsigned int id,unsigned int psw,char *nick=NULL,bool online=false);
    ~Account();

    std::vector<unsigned int> friendList;
    std::vector<friendinfo> friendinfoList;

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