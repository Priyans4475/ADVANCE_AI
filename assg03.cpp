#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <queue>
#include <stack>
#include <unordered_map>
#include <algorithm>

using namespace std;

struct assg{
    int id,prompts;
    vector<int> deps;
};

struct State{
    vector<int> done;
    int day;
};

vector<assg> A;
unordered_map<int,int> mp;
int nA;

int CGPT,GEM,c1,c2;
bool caseA=false;

int nodesDFS=0,nodesDFBB=0,nodesA=0;

//--------------------------------------------------
bool goal(const State &s){
    for(int x:s.done) if(!x) return false;
    return true;
}

//--------------------------------------------------
bool feasible(int idx,const State &s){
    for(int d:A[idx].deps){
        if(!s.done[mp[d]]) return false;
    }
    return true;
}

//--------------------------------------------------
// SIMPLE HEURISTIC
//--------------------------------------------------
int heuristic(const State &s,int N){

    int rem=0,cg=0,gm=0;

    for(int i=0;i<nA;i++){
        if(!s.done[i]){
            rem++;
            if(A[i].id%2==0) cg+=A[i].prompts;
            else gm+=A[i].prompts;
        }
    }

    int h1=(rem+N-1)/N;
    int h2=(cg+CGPT-1)/CGPT;
    int h3=(gm+GEM-1)/GEM;

    return max({h1,h2,h3});
}

//--------------------------------------------------
vector<State> expand(const State &cur,int N){

    vector<State> nxt;

    vector<int> avail;
    for(int i=0;i<nA;i++){
        if(!cur.done[i] && feasible(i,cur))
            avail.push_back(i);
    }

    int m=avail.size();

    for(int mask=1;mask<(1<<m);mask++){

        int cg=0,gm=0,cnt=0;
        State ns=cur;
        ns.day++;

        bool ok=true;

        for(int j=0;j<m;j++){
            if(mask&(1<<j)){
                int idx=avail[j];
                cnt++;
                if(caseA && cnt>N){ ok=false; break;}

                if(A[idx].id%2==0) cg+=A[idx].prompts;
                else gm+=A[idx].prompts;

                ns.done[idx]=1;
            }
        }

        if(!ok) continue;
        if(cg<=CGPT && gm<=GEM)
            nxt.push_back(ns);
    }

    return nxt;
}

//--------------------------------------------------
int DFS(int N){

    stack<State> st;
    State start;
    start.done.assign(nA,0);
    start.day=0;

    st.push(start);
    int best=1e9;

    while(!st.empty()){
        State cur=st.top(); st.pop();
        nodesDFS++;

        if(goal(cur)){
            best=min(best,cur.day);
            continue;
        }

        if(cur.day>=best) continue;

        vector<State> ch=expand(cur,N);
        for(auto &nx:ch) st.push(nx);
    }

    return best==1e9?-1:best;
}

//--------------------------------------------------
int DFBB(int N){

    stack<State> st;
    State start;
    start.done.assign(nA,0);
    start.day=0;

    st.push(start);
    int bound=1e9;

    while(!st.empty()){
        State cur=st.top(); st.pop();
        nodesDFBB++;

        if(goal(cur)){
            bound=min(bound,cur.day);
            continue;
        }

        if(cur.day>=bound) continue;

        vector<State> ch=expand(cur,N);
        for(auto &nx:ch){
            if(nx.day<bound) st.push(nx);
        }
    }

    return bound==1e9?-1:bound;
}

//--------------------------------------------------
struct cmp{
    bool operator()(pair<State,int> a,pair<State,int> b){
        return a.second>b.second;
    }
};

int Astar(int N){

    priority_queue<pair<State,int>,vector<pair<State,int>>,cmp> pq;

    State start;
    start.done.assign(nA,0);
    start.day=0;

    pq.push({start,heuristic(start,N)});

    int best=-1;

    while(!pq.empty()){
        auto curP=pq.top(); pq.pop();
        State cur=curP.first;
        nodesA++;

        if(goal(cur)){
            best=cur.day;
            break;
        }

        vector<State> ch=expand(cur,N);
        for(auto &nx:ch){
            int f=nx.day+heuristic(nx,N);
            pq.push({nx,f});
        }
    }

    return best;
}

//--------------------------------------------------
int main(int argc,char* argv[]){

    if(argc<7) return 0;

    string file=argv[1];
    int N=stoi(argv[2]);
    CGPT=stoi(argv[3]);
    GEM =stoi(argv[4]);
    c1  =stoi(argv[5]);
    c2  =stoi(argv[6]);

    for(int i=7;i<argc;i++){
        if(string(argv[i])=="--caseA") caseA=true;
    }

    ifstream in(file);
    string line;

    while(getline(in,line)){
        if(line.empty()||line[0]=='%') continue;

        stringstream ss(line);
        char c; ss>>c;

        if(c=='A'){
            assg a;
            ss>>a.id>>a.prompts;
            int d;
            while(ss>>d && d!=0) a.deps.push_back(d);
            mp[a.id]=A.size();
            A.push_back(a);
        }
    }

    nA=A.size();

    cout<<"DFS Days: "<<DFS(N)<<"  Nodes: "<<nodesDFS<<endl;
    cout<<"DFBB Days: "<<DFBB(N)<<"  Nodes: "<<nodesDFBB<<endl;
    cout<<"A* Days: "<<Astar(N)<<"  Nodes: "<<nodesA<<endl;

    return 0;
}
