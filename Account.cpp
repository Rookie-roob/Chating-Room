#include "Account.h"

Account::Account(unsigned int id,unsigned int psw,char *nick,bool online):
            id(id),password(psw),online(online)
    {
        if(nick)
            strcpy(this->nick,nick);
    }

Account::~Account(){}

unsigned int Account::getID(){return this->id;}

char* Account::getNickname(){return this->nick;}

bool Account::checkPassword(unsigned int psw){return (psw==this->password);}

void Account::setNickname(char *nick){if(nick) strcpy(this->nick,nick);}

bool Account::changeState(){online= !online; return online;}

bool Account::showState(){return online;}

unsigned int Account::getFriendnum(){return (unsigned int)friendList.size();}

void Account::loadFriendList(unsigned int friendid){friendList.push_back(friendid);}

unsigned int Account::getFriendID(int key){return friendList[key];}