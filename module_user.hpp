#pragma once
#include "main.hpp"
#include "bpt.hpp"
using std::pair;
using std::string;
using namespace ::database;
namespace user_database
{
    database::BPT<user, 109> db("database_user.out");
    database::BPT<int, 201> dbl("database_login.out");
    void add(user _user)
    {
        db.insert(_user.uid, _user);
    }
    pair<bool, user> query(string userid)
    {
        return db.findone(_hash(userid));
    }
    void modify(string userid, user &new_user)
    {
        db.update(_hash(userid), new_user);
    }
    void del(string userid)
    {
        pair<bool, user> u = db.findone(_hash(userid));
        if (u.first)
        {
            db.erase(_hash(userid), u.second);
        }
    }
    bool empty()
    {
        return db.empty();
    }
    bool login(string username)
    {
        return dbl.findone(_hash(username)).first;
    }
    void add_login(string username)
    {
        int value = 1;
        dbl.insert(_hash(username), value);
    }

    void logout(string username)
    {
        int value = 1;
        dbl.erase(_hash(username), value);
    }
    void clear()
    {
        db.clear("database_user.out");
        dbl.clear("database_login.out");
    }
    void exit()
    {
        dbl.clear("database_login.out");
    }
}