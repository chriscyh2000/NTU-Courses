/*
討論對象：
b06303131 沈家睿
b05504066 李旻翰
*/
#include <iostream>
#include <vector>
#include <queue>
using namespace std;
int N, M;
void reset(vector<int> &toReset, vector<bool> &b){
    while(!toReset.empty()){
        int tmp = toReset.back();
        b[tmp] = 0;
        toReset.pop_back();
    }
}
pair<int, int> bfs(int u, vector<vector<int>> &edge, vector<int> &parent, vector<int> &toReset, vector<bool> &b){
    queue<pair<int, int>> q;
    q.push(make_pair(u, 0));
    toReset.push_back(u);
    b[u] = 1;
    parent[u] = u;
    int curD = 0, count = 0;
    pair<int, int> cur;
    while(!q.empty()){
        cur = q.front();
        q.pop();
        if(cur.second > curD){
            curD = cur.second;
        }
        for(auto it = edge[cur.first].cbegin(); it < edge[cur.first].cend(); ++it){
            if(b[*it]) continue;
            parent[*it] = cur.first;
            q.push(make_pair(*it, cur.second+1));
            toReset.push_back(*it);
            b[*it] = 1;
        }
    }
    return cur;
}
pair<int, int> findD(int u, vector<vector<int>> &edge, vector<int> &parent, vector<int> &toReset, vector<bool> &b){
    pair<int, int> p = bfs(u, edge, parent, toReset, b);
    reset(toReset, b);
    return bfs(p.first, edge, parent, toReset, b);
}
pair<int, int> bfs2(int u, vector<vector<int>> &edge, int block, vector<int> &toReset, vector<bool> &b){
    b[block] = 1;
    queue<pair<int, int>> q;
    q.push(make_pair(u, 0));
    toReset.push_back(u);
    b[u] = 1;
    int curD = 0, count = 0;
    pair<int, int> cur;
    while(!q.empty()){
        cur = q.front();
        q.pop();
        if(cur.second > curD){
            curD = cur.second;
        }
        for(auto it = edge[cur.first].cbegin(); it < edge[cur.first].cend(); ++it){
            if(b[*it]) continue;
            q.push(make_pair(*it, cur.second+1));
            toReset.push_back(*it);
            b[*it] = 1;
        }
    }
    return cur;
}
int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    cin >> N;
    M = N - 1;
    vector<vector<int>> edge(N+4);
    while(M--){
        int u, v;
        cin >> u >> v;
        edge[u].push_back(v);
        edge[v].push_back(u);
    }
    vector<int> parent(N+4);
    vector<int> toReset;
    vector<bool> b(N+4, 0);
    pair<int, int> d1 = findD(1, edge, parent, toReset, b);
    reset(toReset, b);
    vector<int> l(N+4, 0);
    vector<int> r(N+4, 0);
    vector<int> child(N+4);
    int e_ = d1.first, s_ = d1.first;
    int prev = s_;
    child[s_] = s_;
    while(1){
        bool check = 0;
        if(s_ == parent[s_]) check = 1;
        for(auto it = edge[s_].cbegin(); it < edge[s_].cend(); ++it){
            if(*it != prev && *it != parent[s_]){
                int tmp = bfs2(*it, edge, s_, toReset, b).second;
                l[*it] = tmp;
            }
        }
        if(check) break;
        prev = s_;
        s_ = parent[s_];
        child[s_] = prev;
    }
    prev = e_;
    int count = 0;
    while(1){
        bool check = 0;
        if(e_ == parent[e_]) check = 1;
        int maxL = -1;
        for(auto it = edge[e_].cbegin(); it < edge[e_].cend(); ++it){
            if(*it != prev && *it != parent[e_]){
                maxL = max(maxL, l[*it]);
            }
        }
        r[e_] = count;
        if(maxL != -1){
            r[e_] += 1;  
            r[e_] += maxL;
        } 
        if(r[prev] > r[e_]) r[e_] = r[prev];
        if(check) break;
        ++count;
        prev = e_;
        e_ = parent[e_];
    }
    prev = s_;
    count = 0;
    while(1){
        bool check = 0;
        if(s_ == child[s_]) check = 1;
        int maxL = -1;
        for(auto it = edge[s_].cbegin(); it < edge[s_].cend(); ++it){
            if(*it != prev && *it != child[s_]){
                maxL = max(maxL, l[*it]);
            }
        }
        l[s_] = count;
        if(maxL != -1){
            l[s_] += 1;  
            l[s_] += maxL;
        } 
        if(l[prev] > l[s_]) l[s_] = l[prev];
        if(check) break;
        ++count;
        prev = s_;
        s_ = child[s_];
    }
    int minD = 1e9;
    while(child[e_] != e_){
        if(e_ == s_) break;
        int i1, i2;
        if(l[e_] % 2) i1 = l[e_] / 2 + 1;
        else i1 = l[e_] / 2;
        if(r[child[e_]] % 2) i2 = r[child[e_]] / 2 + 1;
        else i2 = r[child[e_]] / 2;
        int tmp = i1 + i2 + 1;
        tmp = max(tmp, l[e_]);
        tmp = max(tmp, r[child[e_]]);
        minD = min(tmp, minD);
        e_ = child[e_];
    }
    cout << minD << '\n';
    return 0;
}