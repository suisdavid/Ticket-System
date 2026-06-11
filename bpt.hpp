#pragma once
#include <iostream>
#include <fstream>
#include <filesystem>
#include "vector.hpp"
using namespace std;
namespace database
{
bool not_exist(string filename){return !filesystem::exists(filename);}//return 1;}
template <class T, int info_len = 2>
class MemoryRiver//位置0存储root下标,1存储结点下标使用量
{
    private:
        /* your code here */
        fstream file;
        string file_name;
        int sizeofT = sizeof(T);

    public:
        MemoryRiver() = default;
        ~MemoryRiver()
        {
            if (file.is_open())
            {
                file.close();
            }
        }
        void initialise(string FN)
        {
            file_name = FN;
            file.open(file_name, std::ios::out);
            int tmp = 0;
            for (int i = 0; i <= info_len; ++i)
            {
                file.write(reinterpret_cast<char *>(&tmp), sizeof(int));
            }
            file.close();
            file.open(file_name, std::ios::out|std::ios::in);
        }
        MemoryRiver(string filename)
        {
            file_name = filename;
            if (not_exist(filename))
            {
                initialise(filename);
            }
        }
        void close()
        {
            file.close();
        }
        void name(string FN)
        {
            file_name = FN;
            file.open(file_name, std::ios::out|std::ios::in);
        }
        // 读出第n个int的值赋给tmp，0_base
        void GetInfo(int &tmp, int n)
        {
            if (n >= info_len)
                return;
            /* your code here */
            file.seekp(n * sizeof(int));
            file.read(reinterpret_cast<char *>(&tmp), sizeof(int));
        }
        // 将tmp写入第n个int的位置，0_base
        void WriteInfo(int tmp, int n)
        {
            if (n >= info_len)
                return;
            /* your code here */
            file.seekp(n * sizeof(int));
            file.write(reinterpret_cast<char *>(&tmp), sizeof(int));
        }
        // 在文件合适位置写入类对象t，并返回写入的位置索引index
        // 位置索引意味着当输入正确的位置索引index，在以下三个函数中都能顺利的找到目标对象进行操作
        // 位置索引index可以取为对象写入的起始位置
        void write(T &t,int cnt)
        {
            /* your code here */
            file.seekp(info_len * sizeof(int) + (cnt-1) * sizeofT);
            file.write(reinterpret_cast<char *>(&t), sizeofT);
        }
        // 用t的值更新位置索引index对应的对象，保证调用的index都是由write函数产生
        void update(T &t, const int index)
        {
            /* your code here */
            file.seekp(info_len * sizeof(int) + (index-1) * sizeofT);
            file.write(reinterpret_cast<char *>(&t), sizeofT);
        }
      
        // 读出位置索引index对应的T对象的值并赋值给t，保证调用的index都是由write函数产生
        void read(T &t, const int index)
        {
            /* your code here */
            file.seekp(info_len * sizeof(int) + (index-1) * sizeofT);
            file.read(reinterpret_cast<char *>(&t), sizeofT);
        }
};
const int base = 257;
const long long mod=10000000000000061;
void print(long long val)
{
    if (val<0){putchar('-');print(-val);return;}
    if (val<10){putchar(val+'0');return;}
    print(val/10);putchar(val%10+'0');
}
        
long long get_hash(string s)
{
    int len = s.length();
    long long v = 0;
    for (int i = 0; i < len; i++)
    {
        v = (v * base + (s[i]+256)%256) % mod;
    }
    return v;
 }

template <class T, int M = 251>
class BPT
{
    private:
        struct node
        {
            int is_data;//若is_data!=0,则表示下一条记录位置
            int cnt;
            pair<long long,T>val[M+1];
            int child[M+1];//cnt表示child/data个数
            node(){is_data=cnt=0;}
        };
        MemoryRiver<node,3>f;
        int new_node(node &p)
        {
            int cnt=0;f.GetInfo(cnt,1);
            f.WriteInfo(++cnt,1);
            f.write(p,cnt);
            return cnt;
        }
        void node_add(node &p,pair<long long, T>v,int c)
        {
            int id=0;
            while (id<p.cnt&&p.val[id]<v){id++;}
            if (id<p.cnt&&p.val[id]==v){return;}
            for (int i=p.cnt;i>id;i--)
            {
                p.val[i]=p.val[i-1];
                p.child[i]=p.child[i-1];
            }
            p.cnt++;p.val[id]=v;
            p.child[id]=c;
        }
        pair<node,node>node_split(node &p)
        {
            node a=node(),b=node();
            for (int i=0;i<((M+1)>>1);i++)
            {
                a.val[i]=p.val[i];a.child[i]=p.child[i];
                b.val[i]=p.val[i+((M+1)>>1)];b.child[i]=p.child[i+((M+1)>>1)];
            }
            a.cnt=b.cnt=((M+1)>>1);
            return make_pair(a,b);
        }
        pair<int,pair<long long,T> > ins(int u,pair<long long,T> v,int rt=0)
        {
            node p;f.read(p,u);
            if (p.is_data)
            {
                node_add(p,v,0);
                if (p.cnt<=M)
                {
                    f.update(p,u);
                    return make_pair(0,make_pair(0,T()));
                }
                else
                {
                    pair<node,node>new_nodes=node_split(p);
                    node nd=node();int id=new_node(nd);
                    new_nodes.first.is_data=id;new_nodes.second.is_data=p.is_data;
                    f.update(new_nodes.first,u);
                    f.update(new_nodes.second,id);
                    return make_pair(id,new_nodes.second.val[0]);
                }
            }
            else
            {
                int id=0;
                for (int i=1;i<p.cnt;i++)
                {
                    if (p.val[i]<v){id=i;}
                    else if (p.val[i]==v){return make_pair(0,make_pair(0,T()));}
                }
                pair<int,pair<long long,T> >res=ins(p.child[id],v);
                if (!id)//这是唯一可能需要更新p.val[id]的时候
                {
                    node tp;f.read(tp,p.child[0]);
                    p.val[0]=tp.val[0];
                    f.update(p,u);
                }
                if (res.first)
                {
                    node_add(p,res.second,res.first);
                    if (p.cnt<=M)
                    {
                        f.update(p,u);
                        return make_pair(0,make_pair(0,T()));
                    }
                    else
                    {
                        pair<node,node>new_nodes=node_split(p);
                        node nd=node();int id=new_node(nd);
                        f.update(new_nodes.first,u);
                        f.update(new_nodes.second,id);
                        if (u==rt)
                        {
                            node np=node();np.cnt=2;
                            np.child[0]=rt;np.child[1]=id;
                            np.val[0]=new_nodes.first.val[0];np.val[1]=new_nodes.second.val[0];
                            int newrt=new_node(np);f.WriteInfo(newrt,0);
                            return make_pair(0,make_pair(0,T()));
                        }
                        return make_pair(id,new_nodes.second.val[0]);
                    }
                }
                else{return make_pair(0,make_pair(0,T()));}
            }
        }
        int node_del(node &p,pair<long long,T>v)
        {
            int id=0;
            if (v.first<=-2){id=-(v.first+2);}//通过下标删除
            else
            {
                while (id<p.cnt&&p.val[id]<v){id++;}
                if (id==p.cnt||p.val[id]!=v){return 0;}
            }
            for (int i=id;i<=p.cnt-2;i++)
            {
                p.val[i]=p.val[i+1];
                p.child[i]=p.child[i+1];
            }
            p.cnt--;return 1;
        }
        void node_merge(node &p,node &q)
        {
            for (int i=p.cnt;i<p.cnt+q.cnt;i++)
            {
               p.val[i]=q.val[i-p.cnt];p.child[i]=q.child[i-p.cnt];
            }
            p.cnt+=q.cnt;
        }
        int try_del(int u,pair<long long,T>v,int fa,int id,node &p,node &father)
        {
            if (!node_del(p,v)){return -1;}
            if (p.cnt>=(M+1)/2)
            {
                f.update(p,u);father.val[id]=p.val[0];
                return -1;
            }
            else
            {
                if (id+1<father.cnt)
                {
                    node nex;f.read(nex,father.child[id+1]);
                    if (nex.cnt>(M+1)/2)
                    {
                        node_add(p,nex.val[0],nex.child[0]);f.update(p,u);
                        node_del(nex,make_pair(-2,T()));f.update(nex,father.child[id+1]);
                        father.val[id+1]=nex.val[0];father.val[id]=p.val[0];
                        return -1;
                    }
                    else if (!id)
                    {
                        node_merge(p,nex);
                        p.is_data=nex.is_data;
                        f.update(p,u);
                        father.val[id]=p.val[0];
                        return id+1;
                    }
                }
                if (id)
                {
                    node pre;f.read(pre,father.child[id-1]);
                    if (pre.cnt>(M+1)/2)
                    {
                        node_add(p,pre.val[pre.cnt-1],pre.child[pre.cnt-1]);f.update(p,u);
                        node_del(pre,make_pair(-2-pre.cnt+1,T()));f.update(pre,father.child[id-1]);
                        father.val[id]=p.val[0];
                        return -1;
                    }
                    else
                    {
                        node_merge(pre,p);pre.is_data=p.is_data;
                        if (p.is_data&&id>1)
                        {
                            node prepre;f.read(prepre,father.child[id-2]);prepre.is_data=u;f.update(prepre,father.child[id-2]);
                        }
                        f.update(pre,u);
                        father.val[id]=pre.val[0];
                        return id-1;
                    }
                }
                else{return -1;}
            }
        }

        int del(int u,pair<long long,T>v,int fa,node &father,int child_id,int rt=0)
        {
            node p;f.read(p,u);
            if (p.is_data)
            {
                return try_del(u,v,fa,child_id,p,father);
            }
            else
            {
                int id=0;
                for (int i=1;i<p.cnt;i++)
                {
                    if (p.val[i]<=v){id=i;}
                }
                pair<long long,T>temp1=p.val[id],temp2=p.val[id+1];
                int res=del(p.child[id],v,u,p,id);
                if (res!=-1)
                {
                    if (u==rt)
                    {
                        if (p.cnt>2)
                        {
                            node_del(p,make_pair(-2-res,T()));f.update(p,rt);
                        }
                        else//默认p.cnt>=2
                        {
                            f.WriteInfo(p.child[1-res],0);
                        }
                        return -1;
                    }
                    return try_del(u,make_pair(-2-res,T()),fa,child_id,p,father);
                }
                else if (p.val[id]!=temp1||p.val[id+1]!=temp2){f.update(p,u);father.val[child_id]=p.val[0];}
                return -1;
            }
        }
        void output(int u)
        {
            node p;f.read(p,u);
            cout<<u<<" "<<p.cnt<<": ";
            for (int i=0;i<p.cnt;i++)
            {
                cout<<p.child[i]<<" "<<p.val[i].first<<" ";
            }
            cout<<endl;
            if (!p.is_data)
            {
                for (int i=0;i<p.cnt;i++)
                {
                    output(p.child[i]);
                }
            }
        }
    public:
        BPT(string filename="a.out")
        {
            if (not_exist(filename))
            {
                f.initialise(filename);
                node root=node(),p=node(),q=node();root.cnt=2;root.val[0]=make_pair(-(M+1)/2,T());root.val[1]=make_pair(mod,T());
                for (int i=0;i<(M+1)/2;i++)
                {
                    p.val[i]=make_pair(i-(M+1)/2,T());q.val[i]=make_pair(mod+i,T());
                }
                p.cnt=q.cnt=(M+1)/2;p.is_data=p.child[0]=2;q.is_data=q.child[0]=1;
                root.child[0]=new_node(p);root.child[1]=new_node(q);
                f.WriteInfo(new_node(root),0);
            }
            else
            {
                f.name(filename);
            }
        }
        void clear(string filename="a.out")
        {
            f.close();f.initialise(filename);
            node root=node(),p=node(),q=node();root.cnt=2;root.val[0]=make_pair(-(M+1)/2,T());root.val[1]=make_pair(mod,T());
            for (int i=0;i<(M+1)/2;i++)
            {
                p.val[i]=make_pair(i-(M+1)/2,T());q.val[i]=make_pair(mod+i,T());
            }
            p.cnt=q.cnt=(M+1)/2;p.is_data=p.child[0]=2;q.is_data=q.child[0]=1;
            root.child[0]=new_node(p);root.child[1]=new_node(q);
            f.WriteInfo(new_node(root),0);
        }
        pair<bool,T> findone(long long index)
        {
            long long hashval=index;
            int u=0;
            f.GetInfo(u,0);
            node p;
            while (1)
            {
                f.read(p,u);
                if (p.is_data){break;}
                u=p.child[0];
                for (int i=1;i<p.cnt;i++)
                {
                    if (p.val[i].first<=hashval)
                    {
                        u=p.child[i];
                    }
                    else{break;}
                }
            }
            while (1)
            {
                f.read(p,u);
                if (p.val[0].first>hashval){break;}
                if (p.val[p.cnt-1].first>=hashval)
                {
                    for (int i=0;i<p.cnt;i++)
                    {
                        if (p.val[i].first==hashval)
                        {
                           return make_pair(1,p.val[i].second);
                        }
                        else if (p.val[i].first>hashval){break;}
                    }
                }
                u=p.is_data;//l=nex[l]
            }
            return make_pair(0,T());
        }
        T find_value(long long index,T &value)
        {
            pair<long long,T>key=make_pair(index,value);
            int u=0;
            f.GetInfo(u,0);
            node p;
            while (1)
            {
                f.read(p,u);
                if (p.is_data){break;}
                u=p.child[0];
                for (int i=1;i<p.cnt;i++)
                {
                    if (p.val[i]<=key)
                    {
                        u=p.child[i];
                    }
                    else{break;}
                }
            }
            while (1)
            {
                f.read(p,u);
                if (p.val[0]>key){break;}
                if (p.val[p.cnt-1]>=key)
                {
                    for (int i=0;i<p.cnt;i++)
                    {
                        if (p.val[i]==key)
                        {
                            return p.val[i].second;
                        }
                        else if (p.val[i]>key){break;}
                    }
                }
                u=p.is_data;//l=nex[l]
            }
            return T();
        }
        sjtu::vector<T> find_all(long long index)
        {
            long long hashval=index;int u=0;
            f.GetInfo(u,0);
            node p;sjtu::vector<T> res;
            while (1)
            {
                f.read(p,u);
                if (p.is_data){break;}
                u=p.child[0];
                for (int i=1;i<p.cnt;i++)
                {
                    if (p.val[i].first<hashval)
                    {
                        u=p.child[i];
                    }
                    else{break;}
                }
            }
            while (1)
            {
                f.read(p,u);
                if (p.val[0].first>hashval){break;}
                if (p.val[p.cnt-1].first>=hashval)
                {
                    for (int i=0;i<p.cnt;i++)
                    {
                        if (p.val[i].first==hashval)
                        {
                           res.push_back(p.val[i].second);
                        }
                        else if (p.val[i].first>hashval){break;}
                    }
                    if (p.val[p.cnt-1].first>hashval){break;}
                }
                u=p.is_data;//l=nex[l]
            }
            return res;
        }
        void insert(long long index,T &value)
        {
            pair<long long,T>v=make_pair(index,value);
            int rt=0;f.GetInfo(rt,0);
            ins(rt,v,rt);
            f.WriteInfo(1,2);
        }
    
        void erase(long long index,T &value)
        {
            pair<long long,T>v=make_pair(index,value);
            int rt=0;f.GetInfo(rt,0);
            node p;
            del(rt,v,0,p,0,rt);
        }
        void update(long long index,T &value)
        {
            pair<bool,T>res=findone(index);
            if (res.first){erase(index,res.second);}
            insert(index,value);
        }

        bool empty()
        {
            int tmp=0;f.GetInfo(tmp,2);
            return tmp==0;
        }
    };
};
/*
8
insert FlowersForAlgernon 1966
insert CppPrimer 2012
insert Dune 2021
insert CppPrimer 2001
find CppPrimer
find Java
delete Dune 2021
find Dune
*/