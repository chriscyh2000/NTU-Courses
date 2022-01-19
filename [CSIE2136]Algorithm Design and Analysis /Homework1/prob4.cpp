//I discussed the problem with 楊偉倫
#include <iostream>
#include <vector>
#include <stack>
#define n_inf -1e10
using namespace std;
int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int T, flag;
    cin >> T >> flag;
    while(T--){
        int N;
        cin >> N;
        stack<int> index;
        vector<int> avec(N);
        vector<pair<long long, int>> alt;
        long long cur = n_inf, ans = 0;
        int i = 0, cur_idx;
        for(; i < N; ++i) cin >> avec[i];
        i = 0;
        while(i < N && avec[i] <= 0){
            if(cur < avec[i]){
                cur = avec[i];
                cur_idx = i;
            }
            ++i;
        }
        if(i == N){
            ans = cur;
            cout << ans << endl;
            if(flag){
                cout << 1 << ' ' << cur_idx + 1 << endl;
            }
            continue;
        }
        bool pn = true;
        while(i < N){
            cur = n_inf;
            if(pn){
                while(i < N && avec[i] > 0){
                    if(cur < avec[i]){
                        cur = avec[i];
                        cur_idx = i;
                    }
                    ++i;
                }
                alt.push_back(make_pair(cur, cur_idx));
            }
            else{
                while(i < N && avec[i] <= 0){
                    if(cur < avec[i]){
                        cur = avec[i];
                        cur_idx = i;
                    }
                    ++i;
                }
                alt.push_back(make_pair(cur, cur_idx));
            }
            pn = !pn;
        }
        vector<long long> dp(alt.size());
        long long pmax = n_inf, nmax = n_inf;
        dp[0] = alt[0].first;
        pmax = dp[0];
        for(int j = 1; j < alt.size(); ++j){
            if(j % 2){
                dp[j] = pmax + alt[j].first;
                nmax = max(nmax, dp[j]);
            }
            else{
                dp[j] = max(nmax + alt[j].first, alt[j].first);
                pmax = max(pmax, dp[j]);
            }
        }
        ans = n_inf;
        int idx;
        for(int j = 0; j < alt.size(); ++j){
            if(ans < dp[j]){
                ans = dp[j];
                idx = j;
            }
        }
        cout << ans << endl;
        if(flag){
            long long record = ans;
            while(idx >= 0){
                if(dp[idx] == record){
                    index.push(alt[idx].second);
                    record -= alt[idx].first;
                }
                --idx;
            }
            cout << index.size() << ' ';
            while(index.size() > 1){
                cout << index.top() + 1 << ' ';
                index.pop();
            }
            cout << index.top() + 1 << endl;
        }
    }
    return 0;
}