/*
討論對象：
b06303131 沈家睿
b08501098 柯晨緯
*/
#include <iostream>
#include <vector>
#include <queue>
using namespace std;
const long long inf = 1e16;
typedef pair<long long, long long> lPair;
void addEdge(vector<vector<pair<lPair, long long>>> &adj, long long u, long long v, long long w, long long i){
    adj[u].push_back(make_pair(make_pair(v, w), i));
    adj[v].push_back(make_pair(make_pair(u, w), i));
}
long long findMST(vector<vector<pair<lPair, long long>>> &adj, int v_num){
    priority_queue<lPair, vector<lPair>, greater<lPair>> pq;
    int src = 1;
    vector<long long> key(v_num+4, inf);
    vector<bool> inMST(v_num+4, 0);
    key[src] = 0;
    pq.push(make_pair(0, src));
    long long cost = 0;
    while(!pq.empty()){
        int u = pq.top().second;
        pq.pop();
        if(inMST[u]) continue;
        inMST[u] = 1;
        cost += key[u];
        for(auto it = adj[u].cbegin(); it < adj[u].cend(); ++it){
            if(!inMST[(*it).first.first] && key[(*it).first.first] > (*it).first.second){
                key[(*it).first.first] = (*it).first.second;
                pq.push(make_pair((*it).first.second, (*it).first.first));
            }
        }
    }
    return cost;
}
long long findSPT(vector<vector<pair<lPair, long long>>> &adj, int v_num, vector<int> &edge_idx, vector<long long> &costs){
    priority_queue<lPair, vector<lPair>, greater<lPair>> pq;
    int src = 1;
    vector<long long> key(v_num+4, inf);
    vector<int> edgeUsed(v_num+4, 0);
    vector<int> edgeCost(v_num+4, 0);
	vector<bool> isSPT(v_num+4, false);
    key[src] = 0;
    pq.push(make_pair(0, src));
    long long cost = 0;
    while(!pq.empty()){
        int u = pq.top().second;
        pq.pop();
		if(isSPT[u]) continue;
		isSPT[u] = 1;
        edge_idx.push_back(edgeUsed[u]);
        for(auto it = adj[u].cbegin(); it < adj[u].cend(); ++it){
            if(key[(*it).first.first] > key[u] + (*it).first.second || (key[(*it).first.first] == key[u] + (*it).first.second && edgeCost[(*it).first.first] > (*it).first.second)){
                key[(*it).first.first] = key[u] + (*it).first.second;
                pq.push(make_pair(key[(*it).first.first], (*it).first.first));
                edgeUsed[(*it).first.first] = (*it).second;
                edgeCost[(*it).first.first] = (*it).first.second;
            }
        }
    }
    for(auto it = edge_idx.cbegin() + 1; it < edge_idx.cend(); ++it){
        cost += costs[*it];
    }
    return cost;
}
int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int N, M;
    cin >> N >> M;
    vector<vector<pair<lPair, long long>>> adj(N+1);
    vector<long long> costs;
    int u, v, w;
    costs.push_back(0);
    for(int i = 1; i <= M; ++i){
        cin >> u >> v >> w;
        addEdge(adj, u, v, w, i);
        costs.push_back(w);
    }
    vector<int> edge_idx;
    
    if(findSPT(adj, M, edge_idx, costs) == findMST(adj, M)){
        cout << "Yes\n";
        for(auto it = edge_idx.cbegin() + 1; it < edge_idx.cend() - 1; ++it){
            cout << *it << ' ';
        }
        if(N > 1) cout << edge_idx.back() << '\n';
    }
    else cout << "No\n";
    return 0;
}