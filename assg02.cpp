#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <queue>
#include <unordered_map>

using namespace std;

struct assg {
    int id, prompts;
    vector<int> deps;
};

vector<assg> A;
unordered_map<int,int> mp;
vector<vector<int>> adj;
vector<int> org;
int nA;

bool canFinish(int N,int K,int maxDays,bool exc){
    vector<int> temp=org;
    queue<int> q,nextQ;

    for(int i=0;i<nA;i++){
        if(temp[i]==0) q.push(i);
        if(A[i].prompts>K) return false;
    }

    int done=0;

    for(int day=0;day<maxDays;day++){
        if(q.empty()) break;

        vector<int> cur;
        while(!q.empty()){
            cur.push_back(q.front());
            q.pop();
        }

        vector<int> cap(N,K);
        vector<int> today,left;

        for(int x:cur){
            bool ok=false;
            for(int s=0;s<N;s++){
                if(cap[s]>=A[x].prompts){
                    cap[s]-=A[x].prompts;
                    today.push_back(x);
                    ok=true;
                    break;
                }
            }
            if(!ok) left.push_back(x);
        }

        for(int x:today){
            done++;
            for(int v:adj[x]){
                temp[v]--;
                if(temp[v]==0){
                    if(exc) nextQ.push(v);
                    else q.push(v);
                }
            }
        }

        for(int x:left) q.push(x);

        if(exc){
            while(!nextQ.empty()){
                q.push(nextQ.front());
                nextQ.pop();
            }
        }

        if(done==nA) return true;
    }
    return done==nA;
}

int main(int argc,char* argv[]){
    if(argc<4) return 1;

    string file=argv[1];
    int N=stoi(argv[2]);
    int X=stoi(argv[3]);

    bool findK=false,exc=false;
    for(int i=4;i<argc;i++){
        if(string(argv[i])=="--findK") findK=true;
        if(string(argv[i])=="--exc") exc=true;
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
            while(ss>>d&&d!=0) a.deps.push_back(d);
            mp[a.id]=A.size();
            A.push_back(a);
        }
    }

    nA=A.size();
    adj.assign(nA,vector<int>());
    org.assign(nA,0);

    for(int i=0;i<nA;i++){
        for(int d:A[i].deps){
            int u=mp[d];
            adj[u].push_back(i);
            org[i]++;
        }
    }

    int lo=1,hi=2000,ans=-1;

    if(!findK){
        int K=X;
        while(lo<=hi){
            int mid=(lo+hi)/2;
            if(canFinish(N,K,mid,exc)){
                ans=mid; hi=mid-1;
            }else lo=mid+1;
        }
        cout<<"Earliest completion time: "<<ans<<" days\n";
    }else{
        int D=X;
        while(lo<=hi){
            int mid=(lo+hi)/2;
            if(canFinish(N,mid,D,exc)){
                ans=mid; hi=mid-1;
            }else lo=mid+1;
        }
        cout<<"Minimum prompts per student per day: "<<ans<<"\n";
    }
    return 0;
}
