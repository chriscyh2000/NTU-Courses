/*
討論對象：
b06303131 沈家睿
b08501098 柯晨緯
b09902040 洪郁凱
*/
#include <iostream>
#include <vector>
#include <stack>
using namespace std;
void dfs1(int u, stack<int> &stk, vector<bool> &b, vector<vector<int>> &adj){
    b[u] = 1;
    for(int i = 0; i < adj[u].size(); ++i){
        if(b[adj[u][i]]) continue;
        dfs1(adj[u][i], stk, b, adj);
    }
    stk.push(u);
}
void dfs2(int u, vector<bool> &b, vector<vector<int>> &rev_adj, vector<int> &place, vector<vector<int>> &components, int numComp){
    b[u] = 1;
    place[u] = numComp;
    components[numComp].push_back(u);
    for(int i = 0; i < rev_adj[u].size(); ++i){
        if(b[rev_adj[u][i]]) continue;
        dfs2(rev_adj[u][i], b, rev_adj, place, components, numComp);
    }
}
pair<bool, int> checkSrc(int src, vector<vector<int>> &adj, vector<bool> &b){
    b[src] = 1;
    if(adj[src].empty()) return make_pair(1, src);
    pair<bool, int> check;
    for(int i = 0; i < adj[src].size(); ++i){
        if(!b[adj[src][i]]){
            check = checkSrc(adj[src][i], adj, b);
            if(check.first) return check;
        }
    }
    return make_pair(0, -1);
}
int countMinEdges(int N, vector<vector<int>> &adj, vector<vector<int>> &rev_adj, vector<pair<int, int>> &evec){
    vector<bool> b1(N+1, 0);
    vector<bool> b2(N+1, 0);
    vector<vector<int>> components(N+1);
    vector<int> place(N+1);
    stack<int> stk;
    int numComp = 0;
    for(int i = 1; i <= N; ++i){
        if(!b1[i]) dfs1(i, stk, b1, adj);
    }
    while(!stk.empty()){
        int v = stk.top();
        stk.pop();
        if(!b2[v]){
            dfs2(v, b2, rev_adj, place, components, numComp);
            ++numComp;
        }
    }
    if(numComp == 1) return 0;
    vector<int> inD(numComp+1, 0);
    vector<int> outD(numComp+1, 0);
    vector<vector<int>> new_adj(numComp+1);
    for(int i = 0; i < numComp; ++i){
        for(int j = 0; j < components[i].size(); ++j){
            int u = components[i][j];
            for(int k = 0; k < adj[u].size(); ++k){
                if(place[u] != place[adj[u][k]]){
                    ++inD[place[adj[u][k]]];
                    ++outD[place[u]];
                    new_adj[place[u]].push_back(place[adj[u][k]]);
                }
            }
        }
    }
    int count1 = 0, count2 = 0;
    vector<int> isolated, src, sink;
    for(int i = 0; i < numComp; ++i){
        if(!inD[i] && !outD[i]) isolated.push_back(i);
        else if(!inD[i]) src.push_back(i);
        else if(!outD[i]) sink.push_back(i);
    }
    vector<bool> b3(numComp+1, 0);
    vector<int> sink_r, src_r;
    int prevSink = -1, head = -1, tail = -1;
    pair<bool, int> check;
    for(int i = 0; i < src.size(); ++i){
        check = checkSrc(src[i], new_adj, b3);
        if(check.first){
            if(head < 0) head = src[i];
            tail = check.second;
            if(prevSink >= 0) evec.push_back(make_pair(components[prevSink][0], components[src[i]][0]));
            prevSink = tail;
        }
        else src_r.push_back(src[i]);
    }
    if(prevSink < 0) head = isolated[0];
    for(int i = 0; i < isolated.size(); ++i){
        if(prevSink >= 0){
            evec.push_back(make_pair(components[prevSink][0], components[isolated[i]][0]));
        }
        tail = prevSink = isolated[i];
    }
    for(int i = 0; i < sink.size(); ++i){
        if(!b3[sink[i]]) sink_r.push_back(sink[i]);
    }
    evec.push_back(make_pair(components[tail][0], components[head][0]));
    if(sink_r.size() > src_r.size()){
        int i = 0;
        for(; i < src_r.size(); ++i){
            evec.push_back(make_pair(components[sink_r[i]][0], components[src_r[i]][0]));
        }
        for(; i < sink_r.size(); ++i){
            evec.push_back(make_pair(components[sink_r[i]][0], components[head][0]));
        }
    }
    else{
        int i = 0;
        for(; i < sink_r.size(); ++i){
            evec.push_back(make_pair(components[sink_r[i]][0], components[src_r[i]][0]));
        }
        for(; i < src_r.size(); ++i){
            evec.push_back(make_pair(components[head][0], components[src_r[i]][0]));
        }
    }
    return evec.size();
}
int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int T, flag;
    cin >> T >> flag;
    while(T--){
        int N, M;
        cin >> N >> M;
        vector<vector<int>> adj(N+1);
        vector<vector<int>> rev_adj(N+1);
        int u, v;
        for(int i = 0; i < M; ++i){
            cin >> u >> v;
            adj[u].push_back(v);
            rev_adj[v].push_back(u);
        }
        vector<pair<int, int>> evec;
        cout << countMinEdges(N, adj, rev_adj, evec) << '\n';
        if(evec.size() && flag){
            for(int i = 0; i < evec.size(); ++i){
                cout << evec[i].first << ' ' << evec[i].second << '\n'; 
            }
        }
    }
    return 0;
}