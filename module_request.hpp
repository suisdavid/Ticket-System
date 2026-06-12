#pragma once
#include "main.hpp"
#include "bpt.hpp"
using std::string;
using std::pair;

namespace request_database
{
    database::BPT<request,21>dbr("database_request.out");
    database::BPT<int,251>dbu("database_user_request_cnt.out");
    database::BPT<pending_request,101>db_pending("database_pending_list.out");
    
    int query_user_request_cnt(string username)
    {
        pair<int,int>res=dbu.findone(_hash(username));
        if (res.first==0){return 0;}
        return res.second;
    }

    void add(string username,request& _request)
    {
        long long uid=_hash(username);
        int cnt=query_user_request_cnt(username);
        _request.order=cnt+1;
        if (cnt){dbu.erase(uid,cnt);}
        cnt++;dbu.insert(uid,cnt);
        dbr.insert(uid,_request);
    }
    void add_pending(string username,request &_request)
    {
        __int128 trid=_hash(string(_request.trainid));
        pending_request v(_hash(username),_request.timestamp,_request.order,_request.l,_request.r,_request.num);
        db_pending.insert((trid*PW+_request.day)%mod,v);
    }
    void del_pending(__int128 trid,long long day,pending_request &v)
    {
        db_pending.erase((trid*PW+day)%mod,v);
    }
    vector<pending_request> get_pendings(__int128 trid,long long day)
    {
        return db_pending.find_all((trid*PW+day)%mod);
    }

    pair<int,request> refund(int order,string username)
    {
        long long uid=_hash(username);
        request _temp=request();_temp.order=order;
        request _request=dbr.find_value(uid,_temp);
        if (_request.status==2){return make_pair(-1,_request);}
        if (_request.status==0)
        {
            __int128 trid=_hash(string(_request.trainid));
            pending_request v(uid,_request.timestamp,_request.order,_request.l,_request.r,_request.num);
            del_pending(trid,_request.day,v);
        }
        int temp=_request.status;
        _request.status=2;
        dbr.erase(uid,_temp);dbr.insert(uid,_request);
        return make_pair(temp,_request);
    }
    void succeed(int order,long long uid)
    {
        request _temp=request();_temp.order=order;
        request _request=dbr.find_value(uid,_temp);
        _request.status=1;
        dbr.erase(uid,_temp);dbr.insert(uid,_request);
    }
    void print_all(int timestamp,string username)
    {
        
        long long uid=_hash(username);
        vector<request>v=dbr.find_all(uid);
        int n=v.size();
        cout<<"["<<timestamp<<"] "<<n<<endl;
        for (int i=n-1;i>=0;i--)
        {
            request &r=v[i];
            cout<<"["<<(r.status==0?"pending":(r.status==1?"success":"refunded"));
            cout<<"] "<<r.trainid<<" "<<r.s<<" ";
            outputdate(r.departure);
            cout<<" -> "<<r.t<<" ";
            outputdate(r.arrival);
            cout<<" "<<r.price<<" "<<r.num<<endl;
        }
    }


    void clear()
    {
        dbr.clear("database_request.out");
        dbu.clear("database_user_request_cnt.out");
        db_pending.clear("database_pending_list.out");
    }
}