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
    Account(unsigned int id,unsigned int psw,char *nick,bool online=false):
            id(id),password(psw),online(online)
    {
        strcpy(this->nick,nick);
    }
    ~Account();
    inline unsigned int getID(){return this->id;}
    inline char* getNickname(){return this->nick;}
    inline bool checkPassword(unsigned int psw){return (psw==this->password);}
    inline bool changeState(){online= !online; return online;}
    inline bool showState(){return online;}
    inline unsigned int getFriendnum(){return (unsigned int)friendList.size();}
    void loadFriendList(unsigned int friendid); //use this function to load the information of friend list from data file
    inline unsigned int getFriendID(int key){return friendList[key];}

    friend class CRP;
};

Account::~Account()
{
}

void Account::loadFriendList(unsigned int friendid)
{
    friendList.push_back(friendid);
}


#endif