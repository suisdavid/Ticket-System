#include "bpt.hpp"
#include "main.hpp"
#include "module_train.hpp"
#include "module_user.hpp"
#include "module_request.hpp"
#include "vector.hpp"
#include <iostream>
#include <fstream>
using std::cin;
using std::cout;
using std::endl;
using std::string;
using sjtu::vector;
int timestamp;
string op;
bool CanCin()
{
    while (cin.peek() == ' ')
    {
        getchar();
    }
    return (cin.peek()!='[' && cin.peek() != '\n' && cin.peek() != '\r' && cin.peek() != -1);
}

bool GetEOF()
{
    while (cin.peek() == ' ')
    {
        getchar();
    }
    if (cin.peek() == -1)
    {
        return 1;
    }
    return 0;
}
int get_timestamp()
{
    int res=0;
    char c=getchar();
    while (c<'0'||c>'9'){c=getchar();}
    while (c>='0'&&c<='9')
    {
        res=res*10+(c-'0');
        c=getchar();
    }
    return res;
}
void FilterLine()
{
    while (cin.peek() != '\n' && cin.peek() != '\r' && cin.peek() != -1)
    {
        getchar();
    }
    if (cin.peek() == '\r')
    {
        getchar();
    }
    if (cin.peek() == '\n')
    {
        getchar();
    }
}


void read_separate_string(vector<string>& v)
{
    string s="";char c=getchar();
    while (c==' '){c=getchar();}
    while (c!=' '&&c!='\n'&&c!='\r')
    {
        if (c=='|')
        {
            v.push_back(s);s="";
        }
        else
        {
            s+=c;
        }
        c=getchar();
    }
    v.push_back(s);
}
void read_separate_int(vector<int>& v)
{
    int res=0;char c=getchar();
    while (c==' '){c=getchar();}
    while (c!=' '&&c!='\n'&&c!='\r')
    {
        if (c=='|')
        {
            v.push_back(res);res=0;
        }
        else if (c=='_')
        {
            return;
        }
        else
        {
            res=res*10+c-'0';
        }
        c=getchar();
    }
    v.push_back(res);
}

date read_date()
{
    int hour=0,minute=0;char c=getchar();
    while (c==' '){c=getchar();}
    while (c!=':')
    {
        hour=hour*10+c-'0';
        c=getchar();
    }
    c=getchar();
    while (c>='0'&&c<='9'){
        minute=minute*10+c-'0';
        c=getchar();
    }
    return date(hour,minute);
}

int read_day()
{
    int month=0,day=0;char c=getchar();
    while (c<'0'||c>'9'){c=getchar();}
    while (c>='0'&&c<='9')
    {
        month=month*10+c-'0';c=getchar();
    }
    c=getchar();
    while (c>='0'&&c<='9')
    {
        day=day*10+c-'0';c=getchar();
    }
    if (month==6){day+=0;}
    else if (month==7){day+=30;}
    else if (month==8){day+=61;}
    else if (month==9){day+=92;}
    else{day=-1;}
    return day;
}


void response(int sig=2)
{
    if (sig==2)
    {
         cout<<"["<<timestamp<<"] ";return;
    }
    cout<<"["<<timestamp<<"] "<<sig<<endl;
}

namespace user_op
{
    void add_user()
    {
        string param,value,cur,username,password,name,mail;
        int privilege;
        for (int i=1;i<=6;i++)
        {
            cin>>param;
            if (param=="-c")
            {
                cin>>cur;
            }
            else if (param=="-u")
            {
                cin>>username;
            }
            else if (param=="-p")
            {
                cin>>password;
            }
            else if (param=="-n")
            {
                cin>>name;
            }
            else if (param=="-m")
            {
                cin>>mail;
            }
            else if (param=="-g")
            {
                cin>>privilege;
            }
        }
        if (user_database::empty())
        {
            user_database::add(user(username,password,name,mail,10));
            response(0);
        }
        else
        {
            if (user_database::login(cur)==0){response(-1);return;}
            pair<bool,user>current_user=user_database::query(cur);
            if (current_user.first==0||current_user.second.privilege<=privilege||user_database::query(username).first==1)
            {
                response(-1);return;
            }
            user_database::add(user(username,password,name,mail,privilege));
            response(0);
        }
    }

    void login()
    {
        string param,username,password;
        for (int i=1;i<=2;i++)
        {
            cin>>param;
            if (param=="-u"){cin>>username;}
            else if (param=="-p"){cin>>password;}
        }
        pair<bool,user>_user=user_database::query(username);
        if (_user.first==0||string(_user.second.password)!=password||user_database::login(username)){response(-1);return;}
        user_database::add_login(username);
        user_database::modify(username,_user.second);
        response(0);
    }

    void logout()
    {
        string param,username;
        cin>>param>>username;
        pair<bool,user>_user=user_database::query(username);
        if (_user.first==0||user_database::login(username)==0){response(-1);return;}
        user_database::logout(username);
        user_database::modify(username,_user.second);
        response(0);
    }

    void query_profile()
    {
        string param,cur,username;
        for (int i=1;i<=2;i++)
        {
            cin>>param;
            if (param=="-c"){cin>>cur;}
            else if (param=="-u"){cin>>username;}
        }
        if (user_database::login(cur)==0){response(-1);return;}
        pair<bool,user>current_user=user_database::query(cur);
        if (current_user.first==0){response(-1);return;}
        pair<bool,user>queried_user=user_database::query(username);
        if (queried_user.first==0||!(current_user.second.privilege>queried_user.second.privilege||current_user.second.uid==queried_user.second.uid)){response(-1);return;}
        response(); queried_user.second.print();
    }

    void modify_profile()
    {
        string param,cur,username,password="",name="",mail="";
        int privilege=-1;
        while (CanCin())
        {
            cin>>param;
            if (param=="-c"){cin>>cur;}
            else if (param=="-u"){cin>>username;}
            else if (param=="-p"){cin>>password;}
            else if (param=="-n"){cin>>name;}
            else if (param=="-m"){cin>>mail;}
            else if (param=="-g"){cin>>privilege;}
        }
        if (user_database::login(cur)==0){response(-1);return;}
        pair<bool,user>current_user=user_database::query(cur);
        if (current_user.first==0){response(-1);return;}
        pair<bool,user>queried_user=user_database::query(username);
        if (queried_user.first==0||!(current_user.second.privilege>queried_user.second.privilege||current_user.second.uid==queried_user.second.uid)){response(-1);return;}
        if (privilege>=current_user.second.privilege){response(-1);return;}
        if (password!=""){charmake(queried_user.second.password,password);}
        if (name!=""){charmake(queried_user.second.name,name);}
        if (mail!=""){charmake(queried_user.second.mail,mail);}
        if (privilege!=-1){queried_user.second.privilege=privilege;}
        user_database::modify(username,queried_user.second);
        response();queried_user.second.print();
    }
}

namespace train_op
{
    void add_train()
    {
        string param,train_id;
        vector<string>stations;vector<int>travelTimes,stopoverTimes,prices;
        char type;date startTime;
        int seatNum,stationNum;
        pair<int,int>saleDate;
        for (int i=1;i<=10;i++)
        {
            cin>>param;
            if (param=="-i"){cin>>train_id;}
            else if (param=="-n"){cin>>stationNum;}
            else if (param=="-m"){cin>>seatNum;}
            else if (param=="-s")
            {
                read_separate_string(stations);
            }
            else if (param=="-p")
            {
                read_separate_int(prices);
            }
            else if (param=="-x"){startTime=read_date();}
            else if (param=="-t"){read_separate_int(travelTimes);}
            else if (param=="-o"){read_separate_int(stopoverTimes);}
            else if (param=="-d"){
                saleDate.first=read_day();
                saleDate.second=read_day();
            }
            else if (param=="-y"){cin>>type;}
        }
        if (train_database::query(train_id).first){response(-1);return;}
        int n=stationNum,len=saleDate.second-saleDate.first;
        train _train(train_id,stationNum,seatNum,stations,saleDate.first,len,type);
        train_database::add(_train);
        pair<int,date>min_departure=std::make_pair(saleDate.first,startTime);
        pair<int,date>min_arrival=min_departure;
        for (int i=0;i<n;i++)
        {
            train_database::add_station(stations[i]);
            train_station_map _train_station_map(min_departure,min_arrival,i,len);
            train_database::add_train_station_map(_train.uid,_train.station_uid[i],_train_station_map);
            if (i!=n-1)
            {
                min_arrival=date_add(min_departure,travelTimes[i]);
                if (i!=n-2)
                {
                    min_departure=date_add(min_arrival,stopoverTimes[i]);
                }
                else
                {
                    min_departure=min_arrival;
                }
            }
        }
        train_database::add_price(_train.uid,prices);
        response(0);
    }

    void delete_train()
    {
        string param,train_id;
        cin>>param>>train_id;
        response(train_database::del(train_id));
    }

    void release_train()
    {
        string param,train_id;
        cin>>param>>train_id;
        pair<bool,train>res=train_database::query(train_id);
        if (res.first==1&&train_database::released(train_id)==0)
        {
            train_database::release(res.second);
            response(0);
        }
        else{response(-1);}
    }

    void query_train()
    {
        string param,train_id;int day;
        for (int i=1;i<=2;i++)
        {
            cin>>param;
            if (param=="-i"){cin>>train_id;}
            else if (param=="-d")
            {
                day=read_day();
            }
        }
        pair<bool,train>res=train_database::query(train_id);
        if (res.first==0||res.second.start>day||res.second.start+res.second.len<day)
        {
            response(-1);return;
        }
        train &_train=res.second;int addition=day-_train.start;
        response();cout<<train_id<<" "<<_train.type<<endl;int n=_train.station_num;
        train_seat _train_price=train_database::query_all_prices(_train.uid);
        train_seat _train_seat;
        bool released=train_database::released(train_id);
        if (released)
        {
            _train_seat=train_database::query_all_seats(_train.uid,addition);
        }
        for (int i=0,price=0;i<n;i++)
        {
            train_station_map v=train_database::query_train_station_map(_train.uid,_train.station_uid[i]);
            cout<<train_database::get_station_name(_train.station_uid[i])<<" ";
            if (i==0){cout<<"xx-xx xx:xx -> ";}
            else{
                outputdate(make_pair(v.min_arrival.first+addition,v.min_arrival.second));
                cout<<" -> ";
            }
            if (i==n-1){cout<<"xx-xx xx:xx ";}
            else{
                outputdate(make_pair(v.min_departure.first+addition,v.min_departure.second));
                cout<<" ";
            }
            cout<<price<<" ";
            price+=_train_price.a[i];
            if (i==n-1){cout<<'x';}
            else if (!released){cout<<_train.seats;}
            else
            {
                cout<<_train_seat.a[i];
            }
            cout<<endl;
        }
    }

    void query_ticket()
    {
        string param,s,t,flag="time";int day;
        while (CanCin())
        {
            cin>>param;
            if (param=="-s"){cin>>s;}
            else if (param=="-t"){cin>>t;}
            else if (param=="-d"){day=read_day();}
            else if (param=="-p")
            {
                cin>>flag;
            }
        }
        long long sid=_hash(s),tid=_hash(t);
        vector<train_da>trains=train_database::find_all(sid,tid);int sz=trains.size();
        vector<train_da>new_trains;
        for (int i=0;i<sz;i++)
        {
            if (trains[i].departure.first>day||trains[i].departure.first+trains[i].len<day){continue;}
            new_trains.push_back(trains[i]);
        }
        sort_trains(new_trains,0,new_trains.size()-1,(flag=="time"?1:2));      
        sz=new_trains.size();
        response();cout<<sz<<endl;
        for (int i=0;i<sz;i++)
        {
            int addition=day-new_trains[i].departure.first;
            cout<<new_trains[i].trainid<<" "<<s<<" ";
            outputdate(make_pair(new_trains[i].departure.first+addition,new_trains[i].departure.second));
            cout<<" -> "<<t<<" ";
            outputdate(make_pair(new_trains[i].arrival.first+addition,new_trains[i].arrival.second));
            cout<<" "<<new_trains[i].price<<" "<<train_database::query_seat(new_trains[i].uid,addition,new_trains[i].l,new_trains[i].r)<<endl;
        }
    }

    struct transfer_ans
    {
        pair<int,date> departure1,arrival1,departure2,arrival2;
        long long midid;
        string trainid1,trainid2;
        int price,l1,r1,l2,r2,time,day1,day2;
        transfer_ans(pair<int,date> _departure1=pair<int,date>(),pair<int,date> _arrival1=pair<int,date>(),pair<int,date> _departure2=pair<int,date>(),pair<int,date> _arrival2=pair<int,date>(),int _price=0,string _id1="",string _id2="",int _l1=0,int _r1=0,int _l2=0,int _r2=0,long long _midid=0,int _day1=0,int _day2=0)
        {
            departure1=_departure1;arrival1=_arrival1;
            departure2=_departure2;arrival2=_arrival2;
            price=_price;trainid1=_id1;trainid2=_id2;
            l1=_l1;r1=_r1;l2=_l2;r2=_r2;midid=_midid;day1=_day1;day2=_day2;
            time=date_diff(arrival2,departure1);
        }
        void checkmin(transfer_ans &rhs,int seed)
        {
            if (seed==1)//time
            {
                if (time<rhs.time){return;}
                if (time>rhs.time){*this=rhs;return;}
                if (price<rhs.price){return;}
                if (price>rhs.price){*this=rhs;return;}
            }
            else//cost
            {
                if (price<rhs.price){return;}
                if (price>rhs.price){*this=rhs;return;}
                if (time<rhs.time){return;}
                if (time>rhs.time){*this=rhs;return;}
            }
            if (trainid1<rhs.trainid1){return;}
            else if (trainid1>rhs.trainid1){*this=rhs;return;}
            if (trainid2<rhs.trainid2){return;}
            else if (trainid2>rhs.trainid2){*this=rhs;return;}
        }
    };

    void output_transfer(string trainid,int l,int r,long long sid,long long tid,pair<int,date>departure,pair<int,date>arrival,int day)
    {
        long long uid=_hash(trainid);
        cout<<trainid<<" "<<train_database::get_station_name(sid)<<" ";
        outputdate(departure);cout<<" -> ";
        cout<<train_database::get_station_name(tid)<<" ";
        outputdate(arrival);
        int price=train_database::query_price(uid,l,r),seat=train_database::query_seat(uid,day,l,r);
        cout<<" "<<price<<" "<<seat<<endl;
    }
    void query_transfer()
    {
        string param,s,t,flag="time";int day,seed=1;//time
        while (CanCin())
        {
            cin>>param;
            if (param=="-s"){cin>>s;}
            else if (param=="-t"){cin>>t;}
            else if (param=="-d"){day=read_day();}
            else if (param=="-p")
            {
                cin>>flag;
                if (flag=="cost"){seed=2;}
            }
        }
        long long sid=_hash(s),tid=_hash(t);
        vector<long long>trains1=train_database::get_all_trains(sid);int sz=trains1.size();
        transfer_ans ans=transfer_ans();bool found=0;
        //cout<<"TRANSFER"<<" "<<sz<<endl;
        for (int i=0;i<sz;i++)
        {
            long long trid1=trains1[i];train _train=train_database::get_train(trid1);
            int n=_train.station_num,len=_train.len,price=0,j=0;
            while (_train.station_uid[j]!=sid){j++;}
            if (j==n-1){continue;}
            train_station_map m1=train_database::query_train_station_map(trid1,sid);
            if (m1.min_departure.first>day||m1.min_departure.first+len<day){continue;}
            int addition1=day-m1.min_departure.first;
            pair<int,date>departure1=make_pair(day,m1.min_departure.second);
            train_price _train_price=train_database::query_all_prices(trid1);
            int startid=j;j++;
            for (;j<n;j++)
            {
                price+=_train_price.a[j-1];
                long long midid=_train.station_uid[j];
                vector<train_da>trains2=train_database::find_all(midid,tid);
                int sz2=trains2.size();
                if (!sz2){continue;}
                train_station_map m_mid=train_database::query_train_station_map(trid1,midid);
                pair<int,date>arrival1=make_pair(m_mid.min_arrival.first+addition1,m_mid.min_arrival.second);
                for (int k=0;k<sz2;k++)
                {
                    train_da &train2=trains2[k];
                    if (train2.uid==_train.uid||make_pair(train2.departure.first+train2.len,train2.departure.second)<arrival1)
                    {
                        continue;
                    }
                    int addition2=(train2.departure>=arrival1?0:(train2.departure.second>=arrival1.second?arrival1.first-train2.departure.first:arrival1.first-train2.departure.first+1));
                    pair<int,date>departure2=make_pair(train2.departure.first+addition2,train2.departure.second);
                    pair<int,date>arrival2=make_pair(train2.arrival.first+addition2,train2.arrival.second);
                    
                    transfer_ans newans(departure1,arrival1,departure2,arrival2,price+train2.price,_train.trainid,train2.trainid,startid,j,train2.l,train2.r,midid,addition1,addition2);
                    if (!found){ans=newans;found=1;}
                    else{ans.checkmin(newans,seed);}
                }
            }
        }
        if (!found){response(0);return;}
        response();
        output_transfer(ans.trainid1,ans.l1,ans.r1,sid,ans.midid,ans.departure1,ans.arrival1,ans.day1);
        output_transfer(ans.trainid2,ans.l2,ans.r2,ans.midid,tid,ans.departure2,ans.arrival2,ans.day2);
    }
}

namespace request_op
{
    void buy_ticket()
    {
        string param,username,trainid,s,t,flag="false";int num,day;
        while (CanCin())
        {
            cin>>param;
            if (param=="-u"){cin>>username;}
            else if (param=="-i"){cin>>trainid;}
            else if (param=="-d"){day=read_day();}
            else if (param=="-n"){cin>>num;}
            else if (param=="-f"){cin>>s;}
            else if (param=="-t"){cin>>t;}
            else if (param=="-q"){cin>>flag;}
        }
        if (user_database::login(username)==0||train_database::released(trainid)==0){response(-1);return;}
        long long sid=_hash(s),tid=_hash(t),trid=_hash(trainid);
        int l=train_database::get_station_id(trid,sid),r=train_database::get_station_id(trid,tid);
        train_station_map _train_station_map=train_database::query_train_station_map(trid,sid);
        pair<int,date>departure=_train_station_map.min_departure;
        int addition=day-departure.first;
        if (addition<0||addition>_train_station_map.len){response(-1);return;}
        
        pair<int,date>arrival=train_database::query_train_station_map(trid,tid).min_arrival;
        departure.first+=addition;arrival.first+=addition;
        int seat=train_database::query_seat(trid,addition,l,r),price=train_database::query_price(trid,l,r);
        request _request(trainid,s,t,departure,arrival,num,price,l,r,addition);
        if (seat>=num)
        {
            _request.status=1;
            train_database::change_seat(trid,addition,l,r,-num);
            request_database::add(username,_request);
            response();cout<<(long long)price*num<<endl;
        }
        else if (flag=="true")
        {
            _request.status=0;
            request_database::add_pending(username,timestamp,_request);
            request_database::add(username,_request);
            response();cout<<"queue"<<endl;
        }
        else
        {
            response(-1);
        }
    }

    void query_order()
    {
        string param,username;
        cin>>param>>username;
        if (user_database::login(username)==0){response(-1);return;}
        request_database::print_all(timestamp,username);
    }

    void refund_ticket()
    {
        string param,username;int n=1;
        while (CanCin())
        {
            cin>>param;
            if (param=="-u"){cin>>username;}
            else if (param=="-n"){cin>>n;}
        }
        if (user_database::login(username)==0){response(-1);return;}
        int cnt=request_database::query_user_request_cnt(username)-n+1;
        pair<bool,request>rq=request_database::refund(cnt,username);
        if (rq.first==0){response(-1);return;}
        request &_request=rq.second;
        int l=_request.l,r=_request.r,num=_request.num,day=_request.day;long long trid=_hash(_request.trainid);
        train_seat v=train_database::query_all_seats(trid,day);
        for (int i=l;i<r;i++){v.a[i]+=num;}
        vector<pending_request> pending_list=request_database::get_pendings(trid,day);
        int sz=pending_list.size();
        for (int i=0;i<sz;i++)
        {
            int l=pending_list[i].l,r=pending_list[i].r,num=pending_list[i].num,res=100000;
            for (int j=l;j<r;j++){res=min(res,v.a[j]);}
            if (res>=num)
            {
                for (int j=l;j<r;j++){v.a[j]-=num;}
                request_database::succeed(pending_list[i].order,pending_list[i].uid);
                request_database::del_pending(trid,day,pending_list[i]);
            }
        }
        train_database::change_seat(trid,day,v);
        response(0);
    }
}
void clean()
{
    user_database::clear();
    train_database::clear();
    request_database::clear();
}
int main()
{
    while (1)
    {
        if (GetEOF())
        {
            break;
        }
        timestamp=get_timestamp();
        cin >> op;
        if (op=="add_user")
        {
            user_op::add_user();
        }
        else if (op=="login")
        {
            user_op::login();
        }
        else if (op=="logout")
        {
            user_op::logout();
        }
        else if (op=="query_profile")
        {
            user_op::query_profile();
        }
        else if (op=="modify_profile")
        {
            user_op::modify_profile();
        }
        else if (op=="add_train")
        {
            train_op::add_train();
        }
        else if (op=="delete_train")
        {
            train_op::delete_train();
        }
        else if (op=="release_train")
        {
            train_op::release_train();
        }
        else if (op=="query_train")
        {
            train_op::query_train();
        }
        else if (op=="query_ticket")
        {
            train_op::query_ticket();
        }
        else if (op=="query_transfer")
        {
            train_op::query_transfer();
        }
        else if (op=="buy_ticket")
        {
            request_op::buy_ticket();
        }
        else if (op=="query_order")
        {
            request_op::query_order();
        }
        else if (op=="refund_ticket")
        {
            request_op::refund_ticket();
        }
        else if (op=="exit")
        {
            user_database::exit();
            response();cout<<"bye"<<endl;
            break;
        }
        else if (op=="clean")
        {
            clean();
        }
    }
}