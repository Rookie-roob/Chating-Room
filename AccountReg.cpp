#include "AccountReg.h"


int AccountTable::findAccount(unsigned int id)
{

    return true;
}

void AccountTable::recordIPAddress(int index)
{

}

Account* AccountTable::getAccountInfoByID(unsigned int id)
{
    for (int i = 0;i < accountnum;i++)
    {
        if (accounts[i].account.getID() == id)
        {
            return &accounts[i].account;
        }
    }
    return NULL;
}

void AccountTable::showFriendState(Account* acc)
{
    cout << acc->getNickname() << " 's friends" << endl;
    cout << "Friend's Nick Name        State" << endl;
    for (int i = 0;i < acc->getFriendnum();i++)
    {
        Account* friends = getAccountInfoByID(acc->getFriendID(i));
        cout << friends->getNickname() << "                     ";
        if (friends->showState() == 0)
            cout << "offline" << endl;
        else
            cout << "online" << endl;
    }
}

int AccountTable::initial()
{
    ifstream infile("datafile.data", ifstream::binary | ifstream::in);
    if (!infile)
    {
        cout << "fail to open datafile.data" << endl;
        return 1;
    }

    infile.seekg(0, ios::beg);
    infile.read(reinterpret_cast<char*>(&accountnum), sizeof(int));
    for (int i = 0;i < accountnum;i++)
    {
        infile.seekg(sizeof(int) + BlockSize * i, ios::beg);
        unsigned int id = 0, psw = 0, friendnum = 0;
        char nick[20] = { 0 };
        infile.read(reinterpret_cast<char*>(&id), sizeof(unsigned int));
        infile.read(reinterpret_cast<char*>(&psw), sizeof(unsigned int));
        infile.read(nick, sizeof(char[20]));
        Account acc(id, psw, nick);
        infile.read(reinterpret_cast<char*>(&friendnum), sizeof(unsigned int));
        for (size_t j = 0;j < friendnum;j++)
        {
            int friendid = 0;
            infile.read(reinterpret_cast<char*>(&friendid), sizeof(unsigned int));
            acc.loadFriendList(friendid);
        }
        accounts.push_back(AccountReg(acc));
    }
    cout << "initialize over" << endl;
    infile.close();
    return 0;
}

int AccountTable::newAccount(unsigned int id, unsigned int psw, char* nickname)
{
    Account acc(id, psw, nickname);
    accounts.push_back(AccountReg(acc));
    accountnum++;

    fstream file("datafile.data", fstream::binary | fstream::out | fstream::in);
    if (!file)
    {
        cout << "fail to open datafile.data" << endl;
        return 1;
    }

    char buffer[BlockSize] = { 0 };
    memcpy(buffer, &accountnum, sizeof(int));
    file.seekp(0, ios::beg);
    file.write(buffer, sizeof(int));

    char* ptr = buffer;
    memcpy(ptr, &id, sizeof(unsigned int));
    memcpy(ptr += sizeof(unsigned int), &psw, sizeof(unsigned int));
    memcpy(ptr += sizeof(unsigned int), nickname, strlen(nickname));
    ptr += sizeof(char[20]);
    unsigned int temp = 0;
    for (int i = 0;i < 11;i++)
    {
        memcpy(ptr, &temp, sizeof(unsigned int));
        ptr += sizeof(unsigned int);
    }

    file.seekp(0, ios::end);
    file.write(buffer, BlockSize);
    file.close();
    return 0;
}
int AccountTable::newFriend(unsigned int myid, unsigned int friendid)
{
    Account* acc = getAccountInfoByID(myid);
    if (acc == NULL)
    {
        cout << "myid does not exit" << endl;
        return 3;
    }
    if (acc->getFriendnum() >= 10)
    {
        cout << "number of friends should not be more than 10" << endl;
        return 2;
    }
    acc->loadFriendList(friendid);

    fstream file("datafile.data", fstream::binary | fstream::in | fstream::out);
    if (!file)
    {
        cout << "fail to open datafile.data" << endl;
        return 1;
    }

    for (int i = 0;i < accountnum;i++)
    {
        file.seekg(sizeof(int) + BlockSize * i, ios::beg);
        unsigned int id = 0;
        file.read(reinterpret_cast<char*>(&id), sizeof(unsigned int));
        if (id == myid)
        {
            file.seekg(sizeof(unsigned int) * 2, ios::cur);
            int friendnum = 0;
            file.read(reinterpret_cast<char*>(&friendnum), sizeof(unsigned int));
            file.seekp(sizeof(unsigned int) * friendnum, ios::cur);
            file.write(reinterpret_cast<char*>(&friendid), sizeof(unsigned int));
            file.close();
            return 0;
        }
    }
    return 3;
}

void AccountTable::showAllAccount()
{
    cout << "all accounts" << endl;
    cout << "--------------------------------------------------" << endl;
    cout << "ID      Nick Name" << endl;
    for (int i = 0;i < accountnum;i++)
    {
        cout << accounts[i].account.getID() << "   " << accounts[i].account.getNickname() << endl;
    }
    cout << "--------------------------------------------------" << endl;
}