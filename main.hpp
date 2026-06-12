#pragma once
#include <string.h>
#include <iostream>
#include "vector.hpp"
using sjtu::vector;
using std::cout;
using std::endl;
using std::pair;
using std::string;
const int Base = 257;
const long long Mod = 1000000000000091;
const long long PW = 552877488653549; // pow(base,50,mod)
const int maxday = 200;
long long _hash(string s)
{
    int len = s.length();
    long long v = 0;
    for (int i = 0; i < len; i++)
    {
        v = (v * Base + (s[i] + 256) % 256) % Mod;
    }
    return v;
}
void charmake(char *c, string s)
{
    int len = s.length();
    for (int i = 0; i < len; i++)
    {
        c[i] = s[i];
    }
    c[len] = 0;
}
struct user // 116
{
    char username[21], password[31], name[21], mail[31];
    int privilege;
    long long uid;
    user(string _username = "", string _password = "", string _name = "", string _mail = "", int _privilege = -1)
    {
        charmake(username, _username);
        charmake(password, _password);
        charmake(name, _name);
        charmake(mail, _mail);
        privilege = _privilege;
        uid = _hash(_username);
    }
    bool const operator<(const user &rhs) const
    {
        return uid < rhs.uid;
    }
    bool const operator==(const user &rhs) const
    {
        return uid == rhs.uid;
    }
    bool const operator!=(const user &rhs) const
    {
        return uid != rhs.uid;
    }
    bool const operator>(const user &rhs) const
    {
        return uid > rhs.uid;
    }
    bool const operator>=(const user &rhs) const
    {
        return uid >= rhs.uid;
    }
    bool const operator<=(const user &rhs) const
    {
        return uid <= rhs.uid;
    }

    void print()
    {
        cout << username << " " << name << " " << mail << " " << privilege << endl;
    }
};

struct station // 41
{
    char name[41];
    station(string _name = "")
    {
        charmake(name, _name);
    }
    bool const operator<(const station &rhs) const
    {
        return 0;
    }
    bool const operator==(const station &rhs) const
    {
        return 1;
    }
    bool const operator!=(const station &rhs) const
    {
        return 0;
    }
    bool const operator>(const station &rhs) const
    {
        return 0;
    }
    bool const operator>=(const station &rhs) const
    {
        return 1;
    }
    bool const operator<=(const station &rhs) const
    {
        return 1;
    }
};
struct date // 8
{
    int hour, minute;
    date(int h = 0, int m = 0)
    {
        hour = h;
        minute = m;
    }
    bool const operator<(const date &rhs) const
    {
        return hour != rhs.hour ? hour < rhs.hour : minute < rhs.minute;
    }
    bool const operator==(const date &rhs) const
    {
        return hour == rhs.hour && minute == rhs.minute;
    }
    bool const operator!=(const date &rhs) const
    {
        return hour != rhs.hour || minute != rhs.minute;
    }
    bool const operator>(const date &rhs) const
    {
        return hour != rhs.hour ? hour > rhs.hour : minute > rhs.minute;
    }
    bool const operator>=(const date &rhs) const
    {
        return hour > rhs.hour || (hour == rhs.hour && minute >= rhs.minute);
    }
    bool const operator<=(const date &rhs) const
    {
        return hour < rhs.hour || (hour == rhs.hour && minute <= rhs.minute);
    }
};

pair<int, date> date_add(pair<int, date> d, int x)
{
    d.second.minute += x;
    d.second.hour += d.second.minute / 60;
    d.second.minute %= 60;
    d.first += d.second.hour / 24;
    d.second.hour %= 24;
    return d;
}

void outputdate(pair<int, date> d)
{
    if (d.first <= 30)
    {
        cout << "06-";
    }
    else if (d.first <= 61)
    {
        d.first -= 30;
        cout << "07-";
    }
    else if (d.first <= 92)
    {
        d.first -= 61;
        cout << "08-";
    }
    else
    {
        d.first -= 92;
        cout << "09-";
    }
    if (d.first < 10)
    {
        cout << 0;
    }
    cout << d.first << " ";
    if (d.second.hour < 10)
    {
        cout << 0;
    }
    cout << d.second.hour << ":";
    if (d.second.minute < 10)
    {
        cout << 0;
    }
    cout << d.second.minute;
}

struct train // 854
{
    char trainid[21], type;
    int station_num, seats, start, len;
    long long uid, station_uid[101];
    train(string _trainid = "", int _station_num = 0, int _seat = 0, vector<string> _station_name = vector<string>(), int _start = 0, int _len = 0, char _type = 'G')
    {
        charmake(trainid, _trainid);
        uid = _hash(_trainid);
        station_num = _station_num;
        for (int i = 0; i < station_num; i++)
        {
            station_uid[i] = _hash(_station_name[i]);
        }
        seats = _seat;
        start = _start;
        len = _len;
        type = _type;
    }

    bool const operator<(const train &rhs) const
    {
        return uid < rhs.uid;
    }
    bool const operator==(const train &rhs) const
    {
        return uid == rhs.uid;
    }
    bool const operator!=(const train &rhs) const
    {
        return uid != rhs.uid;
    }
    bool const operator>(const train &rhs) const
    {
        return uid > rhs.uid;
    }
    bool const operator>=(const train &rhs) const
    {
        return uid >= rhs.uid;
    }
    bool const operator<=(const train &rhs) const
    {
        return uid <= rhs.uid;
    }
};

struct train_station_map // 32
{
    pair<int, date> min_arrival, min_departure;
    int id, len;
    train_station_map(pair<int, date> _min_departure = pair<int, date>(), pair<int, date> _min_arrival = pair<int, date>(), int _id = 0, int _len = 0)
    {
        min_arrival = _min_arrival;
        min_departure = _min_departure;
        id = _id;
        len = _len;
    }
    bool const operator<(const train_station_map &rhs) const
    {
        return 0;
    }
    bool const operator==(const train_station_map &rhs) const
    {
        return 1;
    }
    bool const operator!=(const train_station_map &rhs) const
    {
        return 0;
    }
    bool const operator>(const train_station_map &rhs) const
    {
        return 0;
    }
    bool const operator>=(const train_station_map &rhs) const
    {
        return 1;
    }
    bool const operator<=(const train_station_map &rhs) const
    {
        return 1;
    }
};
struct train_da // 69
{
    char trainid[21];
    long long uid;
    int price, len, l, r;
    pair<int, date> departure, arrival;
    train_da(string _trainid = "", int _price = 0, int _len = 0, pair<int, date> _departure = pair<int, date>(), pair<int, date> _arrival = pair<int, date>(), int i = 0, int j = 0)
    {
        charmake(trainid, _trainid);
        price = _price;
        len = _len;
        departure = _departure;
        arrival = _arrival;
        l = i;
        r = j;
        uid = _hash(_trainid);
    }
    bool const operator<(const train_da &rhs) const
    {
        return uid < rhs.uid;
    }
    bool const operator==(const train_da &rhs) const
    {
        return uid == rhs.uid;
    }
    bool const operator!=(const train_da &rhs) const
    {
        return uid != rhs.uid;
    }
    bool const operator>(const train_da &rhs) const
    {
        return uid > rhs.uid;
    }
    bool const operator>=(const train_da &rhs) const
    {
        return uid >= rhs.uid;
    }
    bool const operator<=(const train_da &rhs) const
    {
        return uid <= rhs.uid;
    }
};

int date_diff(pair<int, date> a, pair<int, date> b)
{
    return (a.first - b.first) * 1440 + (a.second.hour - b.second.hour) * 60 + (a.second.minute - b.second.minute);
}

bool cmp(train_da &a, train_da &b, int seed)
{
    if (seed == 1)
    {
        int time1 = date_diff(a.arrival, a.departure), time2 = date_diff(b.arrival, b.departure);
        if (time1 < time2)
        {
            return 1;
        }
        if (time1 > time2)
        {
            return 0;
        }
    }
    else
    {
        if (a.price < b.price)
        {
            return 1;
        }
        if (a.price > b.price)
        {
            return 0;
        }
    }
    return string(a.trainid) <= string(b.trainid);
}

void sort_trains(vector<train_da> &a, int l, int r, int seed)
{
    if (l >= r)
        return;
    int i = l, j = r;
    train_da key(a[l]);
    while (i < j)
    {
        while (i < j && cmp(key, a[j], seed))
        {
            j--;
        }
        if (i < j)
        {
            std::swap(a[i++], a[j]);
        }
        while (i < j && cmp(a[i], key, seed))
        {
            i++;
        }
        if (i < j)
        {
            std::swap(a[i], a[j--]);
        }
    }
    sort_trains(a, l, i - 1, seed);
    sort_trains(a, i + 1, r, seed);
}

struct train_seat // 400
{
    int a[100];
    train_seat(vector<int> _seats = vector<int>())
    {
        int sz = _seats.size();
        for (int i = 0; i < sz; i++)
        {
            a[i] = _seats[i];
        }
    }
    bool const operator<(const train_seat &rhs) const
    {
        return 0;
    }
    bool const operator==(const train_seat &rhs) const
    {
        return 1;
    }
    bool const operator!=(const train_seat &rhs) const
    {
        return 0;
    }
    bool const operator>(const train_seat &rhs) const
    {
        return 0;
    }
    bool const operator>=(const train_seat &rhs) const
    {
        return 1;
    }
    bool const operator<=(const train_seat &rhs) const
    {
        return 1;
    }
};

typedef train_seat train_price;
struct request // 159
{
    char trainid[21], s[41], t[41];
    pair<int, date> departure, arrival;
    int num, order;   // 该用户的第几个request
    int l, r, status; // 0 pending ,1 success ,2 refunded
    int price, day;
    int timestamp;
    request(string _trainid = "", string _s = "", string _t = "", pair<int, date> _departure = pair<int, date>(), pair<int, date> _arrival = pair<int, date>(), int _num = 0, int _price = 0, int _l = 0, int _r = 0, int _day = 0, int _timestamp = 0)
    {
        charmake(trainid, _trainid);
        charmake(s, _s);
        charmake(t, _t);
        departure = _departure;
        arrival = _arrival;
        num = _num;
        price = _price;
        l = _l;
        r = _r;
        day = _day;
        timestamp = _timestamp;
    }
    bool const operator<(const request &rhs) const
    {
        return order < rhs.order;
    }
    bool const operator==(const request &rhs) const
    {
        return order == rhs.order;
    }
    bool const operator!=(const request &rhs) const
    {
        return order != rhs.order;
    }
    bool const operator>(const request &rhs) const
    {
        return order > rhs.order;
    }
    bool const operator>=(const request &rhs) const
    {
        return order >= rhs.order;
    }
    bool const operator<=(const request &rhs) const
    {
        return order <= rhs.order;
    }
};

struct pending_request // 32
{
    long long uid;
    int timestamp, order, l, r, num;
    pending_request(long long _uid = 0, int _timestamp = 0, int _order = 0, int _l = 0, int _r = 0, int _num = 0)
    {
        uid = _uid;
        timestamp = _timestamp;
        order = _order;
        l = _l;
        r = _r;
        num = _num;
    }
    bool const operator<(const pending_request &rhs) const
    {
        return timestamp < rhs.timestamp;
    }
    bool const operator==(const pending_request &rhs) const
    {
        return timestamp == rhs.timestamp;
    }
    bool const operator!=(const pending_request &rhs) const
    {
        return timestamp != rhs.timestamp;
    }
    bool const operator>(const pending_request &rhs) const
    {
        return timestamp > rhs.timestamp;
    }
    bool const operator>=(const pending_request &rhs) const
    {
        return timestamp >= rhs.timestamp;
    }
    bool const operator<=(const pending_request &rhs) const
    {
        return timestamp <= rhs.timestamp;
    }
};
