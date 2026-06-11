#pragma once
#include "main.hpp"
#include "bpt.hpp"
#include "vector.hpp"
using std::string;
using std::pair;
using sjtu::vector;
using namespace database;
namespace train_database
{
    database::BPT<train,5>db("database_train.out");
    database::BPT<long long,201>dbp("database_train_path.out");
    database::BPT<train_seat,7>dbs("database_train_seats.out");
    database::BPT<train_price,7>dbpr("database_train_prices.out");
    database::BPT<station,75>dbst("database_stations.out");
    database::BPT<train_da,47>dbda("database_train_departure_arrival.out");
    database::BPT<train_station_map,91>dbm("database_train_station_map.out");
    void add(train &_train)
    {
        db.insert(_train.uid, _train);
    }

    void add_price(long long index,vector<int>&prices)
    {
       train_price v(prices);
       dbpr.insert(index,v);
    }

    void add_station(string station_name)
    {
        long long sid=_hash(station_name);
        pair<bool,station>res=dbst.findone(sid);
        if (res.first==0)
        {
            station v(station_name);
            dbst.insert(sid,v);
        }
    }

    void add_train_station_map(long long trainid,long long station_id,train_station_map &v)
    {
        long long uid=((__int128)station_id*PW+trainid)%mod;
        dbm.insert(uid,v);
    }

    pair<bool,train> query(string train_id)
    {
        return db.findone(_hash(train_id));
    }

    vector<train_da> find_all(long long sid,long long tid)
    {
        long long index=((__int128)tid*PW+sid)%mod;
        return dbda.find_all(index);
    }
    
    train get_train(long long index)
    {
        return db.findone(index).second;
    }

    vector<long long> get_all_trains(long long sid)
    {
        return dbp.find_all(sid);
    }

    train_seat query_all_seats(__int128 index,long long day)
    {
        pair<bool,train_seat>res=dbs.findone((index*PW+day)%mod);
        if (res.first==0)
        {
            return dbs.findone((index*PW+maxday)%mod).second;
        }
        else
        {
            return res.second;
        }
    }
    train_price query_all_prices(long long index)
    {
        return dbpr.findone(index).second;
    }

    int query_seat(long long index,int day,int l,int r)
    {
        train_seat v=query_all_seats(index,day);int res=100000;
        for (int i=l;i<r;i++)
        {
            res=min(res,v.a[i]);
        }
        return res;
    }

    int query_price(long long index,int l,int r)
    {
        train_price v=dbpr.findone(index).second;int res=0;
        for (int i=l;i<r;i++)
        {
            res+=v.a[i];
        }
        return res;
    }

    train_station_map query_train_station_map(long long trainid,__int128 sid)
    {
        long long uid=(sid*PW+trainid)%mod;
        return dbm.findone(uid).second;
    }

    int get_station_id(long long trainid,long long station_id)
    {
        long long uid=((__int128)station_id*PW+trainid)%mod;
        pair<bool,train_station_map>res=dbm.findone(uid);
        if (res.first==0){return -1;}
        return res.second.id;
    }

    string get_station_name(long long sid)
    {
        pair<bool,station>res=dbst.findone(sid);
        return string(res.second.name);
    }
     
    void change_seat(__int128 index,long long day,train_seat &v)
    {
        dbs.update((index*PW+day)%mod,v);
    }

    void change_seat(__int128 index,long long day,int l,int r,int v)
    {
       train_seat _train_seat=query_all_seats(index,day);
       for (int i=l;i<r;i++){_train_seat.a[i]+=v;}
       change_seat(index,day,_train_seat);
    }
    void release(train &_train)
    {
        long long id=_train.uid;int n=_train.station_num;
        train_price _train_price=dbpr.findone(id).second;
        vector<pair<int,date> >min_departures,min_arrivals;
        for (int i=0;i<n;i++)
        {
            dbp.insert(_train.station_uid[i],id);
            train_station_map v=query_train_station_map(id,_train.station_uid[i]);
            min_departures.push_back(v.min_departure);
            min_arrivals.push_back(v.min_arrival);
        }
        for (int i=0;i<n;i++)
        {
            for (int j=i+1,price=0;j<n;j++)
            {
                price+=_train_price.a[j-1];
                train_da v(_train.trainid,price,_train.len,min_departures[i],min_arrivals[j],i,j);
                dbda.insert((_train.station_uid[i]+(__int128)_train.station_uid[j]*PW%mod)%mod,v);
            }
        }
        vector<int>v;
        for (int i=0;i<n-1;i++){v.push_back(_train.seats);}
        train_seat _train_seat(v);
        dbs.insert(((__int128)id*PW+maxday)%mod,_train_seat);
    }

    bool released(string trainid)
    {
        __int128 tid=_hash(trainid);
        return dbs.findone((tid*PW+maxday)%mod).first;
    }

    int del(string train_id)
    {
        pair<bool,train>u=db.findone(_hash(train_id));
        if (u.first&&!released(train_id))
        {
            db.erase(_hash(train_id),u.second);return 0;
        }
        return -1;
    }

    void clear()
    {
        db.clear("database_train.out");
        dbp.clear("database_train_path.out");
        dbs.clear("database_train_seats.out");
        dbpr.clear("database_train_prices.out");
        dbst.clear("database_stations.out");
        dbda.clear("database_train_departure_arrival.out");
        dbm.clear("database_train_station_map.out");
    }
}