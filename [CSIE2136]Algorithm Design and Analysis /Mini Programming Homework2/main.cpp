//I didn't discuss this problem with anyone.
#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include <algorithm>
using namespace std;
int N, M;
void bfs(vector<int> &v, vector<vector<int>> &edge, vector<bool> &b){
    queue<pair<int, int>> q;
    q.push(make_pair(1, 1));
    v.push_back(1);
    b[1] = 1;
    int curD = 1, count = 0, front = 0;
    while(!q.empty()){
        pair<int, int> cur = q.front();
        q.pop();
        if(cur.second > curD){
            curD = cur.second;
            sort(v.begin()+front, v.begin()+front+count);
            front += count;
            count = 1;
        }
        else ++count;
        for(auto it = edge[cur.first].cbegin(); it < edge[cur.first].cend(); ++it){
            if(b[*it]) continue;
            q.push(make_pair(*it, cur.second+1));
            v.push_back(*it);
            b[*it] = 1;
        }
    }
    sort(v.begin()+front, v.begin()+front+count);
}
void dfs_visit(vector<int> &v, vector<vector<int>> &edge, vector<bool> &b, int u){
    v.push_back(u);
    b[u] = 1;
    sort(edge[u].begin(), edge[u].end());
    for(auto it = edge[u].cbegin(); it < edge[u].cend(); ++it){
        if(b[*it]) continue;
        dfs_visit(v, edge, b, *it);
    }
}
void dfs(vector<int> &v, vector<vector<int>> &edge, vector<bool> &b){
    dfs_visit(v, edge, b, 1);
}
int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    cin >> N >> M;
    vector<vector<int>> edge(N+4);
    vector<bool> b1(N+4, {0});
    vector<bool> b2(N+4, {0});
    while(M--){
        int u, v;
        cin >> u >> v;
        edge[u].push_back(v);
        edge[v].push_back(u);
    }
    //dfs
    vector<int> dfs_vec;
    dfs(dfs_vec, edge, b1);
    for(auto it = dfs_vec.cbegin(); it < dfs_vec.cend()-1; ++it){
        cout << *it << ' ';
    }
    cout << dfs_vec[N-1] << '\n';
    //bfs
    vector<int> bfs_vec;
    bfs(bfs_vec, edge, b2);
    for(auto it = bfs_vec.cbegin(); it < bfs_vec.cend()-1; ++it){
        cout << *it << ' ';
    }
    cout << bfs_vec[N-1] << '\n';
    return 0;
}