//I discussed the problem with b05504066 李旻翰/ b06303131 沈家睿/ p10922001 黃佳琪/ b08501098 柯晨緯
#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <array>
#define ll long long
const ll  n_inf = -1e18;
using namespace std;
int tsize;
ll prevMax(int x, int y, int z, vector<vector<vector<ll>>> &dq, vector<vector<ll>> &dp, vector<int> &idx, int i, int j){
    if(x == 0 && y == 0) return 0;
    if(x < 0 || y < 0 || z < 0) return n_inf;
    if(dq[x][y][idx[z]] >= n_inf) return dq[x][y][idx[z]];
    if(z == 0) return dp[x][y];
    if(z == 1){
        ll M = n_inf-1;
        if(x > 0) M = dp[x-1][y];
        if(y > 0) M = max(M, dp[x][y-1]);
        if(x == i && y == j) return dq[x][y][idx[z]] = M;
        M = max(dp[x][y], M);
        return dq[x][y][idx[z]] = M;
    }
    if(z == 2){
        ll M = max(prevMax(x-1, y, z-1, dq, dp, idx, i, j), prevMax(x, y-1, z-1, dq, dp, idx, i, j));
        if(x == i && y == j) return dq[x][y][idx[z]] = M;
        return dq[x][y][idx[z]] = max(M, dp[x][y]);
    }
    int size = z-z/3;
    ll MAX = max(prevMax(x, y-z/3, size, dq, dp, idx, i, j), prevMax(x-z/3, y, size, dq, dp, idx, i, j));
    MAX = max(prevMax(x, y, size, dq, dp, idx, i, j), MAX);
    if(x == i && y == j) return dq[x][y][idx[z]] = MAX;
    return dq[x][y][idx[z]] = max(MAX, dp[x][y]);
}

int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int T;
    cin >> T;
    while(T--){
        int n, m, k;
        cin >> n >> m >> k;
        ll map[n][m];
        vector<vector<ll>> dp(n);
        string str;
        size_t pos;
        getline(cin, str);
        vector<vector<vector<ll>>> dq(n);
        vector<int> idx(k+1);
        int z = k, rec = 0;
        while(z >= 3){
            idx[z] = rec++;
            z = z - z/3;
        }
        idx[2] = rec++;
        idx[1] = rec++;
        
        for(int i = 0; i < n; ++i){
            dq[i].resize(m);
            dp[i].resize(m);
            int cur = 0, next;
            for(int j = 0; j < m; ++j){
                cin >> str;
                if(str[0] == 'X') map[i][j] = n_inf;
                else map[i][j] = stoll(str, nullptr, 10);
                dq[i][j].resize(rec+1, n_inf-1);
            }
        }
        dp[0][0] = 0;
        for(int i = 0; i < n; ++i){
            for(int j = 0; j < m; ++j){
                if(i == 0 && j == 0) continue;
                if(map[i][j] == n_inf){
                    dp[i][j] = n_inf;
                    continue;
                }
                if(prevMax(i, j, k, dq, dp, idx, i, j) == n_inf){
                    dp[i][j] = n_inf;
                    continue;
                }
                dp[i][j] = prevMax(i, j, k, dq, dp, idx, i, j) + map[i][j];
            }
        }
        if(dp[n-1][m-1] == n_inf){
            cout << "Impassable" << endl;
        }
        else{
            cout << "Passable" << endl;
            cout << dp[n-1][m-1] << endl;
            int cur_j = m-1;
            ll temp = dp[n-1][m-1];
            stack<pair<int, int>> st;
            int count = k, c = k+1;
            for(int i = n-1; i >= 0; --i){
                for(int j = cur_j; j >= 0; --j){
                    if(dp[i][j] == temp){
                        st.push(make_pair(i, j));
                        temp -= map[i][j];
                        cur_j = j;
                        count = k+1;
                        c = k+1;
                    }
                    --count;
                    if(!count || !j){
                        count = (--c);
                        break;
                    }
                }
            }
            cout << st.size() << endl;
            while(!st.empty()){
                cout << st.top().first+1 << ' ' << st.top().second+1 << endl;
                st.pop();
            }
        }
    }
    return 0;
}