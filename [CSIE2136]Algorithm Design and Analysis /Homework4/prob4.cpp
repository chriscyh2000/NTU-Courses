#include <iostream>
#include <vector>
#include <random>
#include <cstdlib>

using namespace std;
const int64_t n_inf = -1e18;
const int64_t inf = 1e18;

int N, M;
int64_t calc(vector<int64_t> &b){
    int64_t value = 0;
    for (size_t j = 0; j < b.size(); ++j) value += b[j];
    for (size_t j = 1; j < b.size(); ++j) value -= abs(b[j] - b[j-1]);
    for (size_t j = 2; j < b.size(); ++j){
        value += b[j-1] * ((b[j-2] & b[j-1]) ^ (b[j-1] | b[j]) ^ (b[j-2] + b[j]));
    }
    return value;
}

int64_t findMin(int i, int j, vector<vector<int64_t>> &dp, vector<int64_t> &b, long long total, vector<vector<int>> &last_cut){
    if(i < j || j < 0) return inf;

    if(dp[i][j] > n_inf) return dp[i][j];

    if(!i && !j){
        last_cut[i][j] = 0;
        return dp[i][j] = b[0];
    } 

    if(!j){
        last_cut[i][j] = 0;
        return dp[i][j] = total;    
    } 

    if(i == 1 && j == 1){
        last_cut[i][j] = 1;
        return dp[i][j] = b[0] + b[1];
    } 

    int p;
    total -= (b[i] - abs(b[i] - b[i-1]) + b[i-1] * ((b[i-2] & b[i-1]) ^ (b[i-1] | b[i]) ^ (b[i-2] + b[i])));
 
    int64_t tmp2 = findMin(i-1, j, dp, b, total, last_cut);
    if(tmp2 != inf){
        tmp2 += (b[i] - abs(b[i-1] - b[i]));
        if(i - last_cut[i-1][j] > 1){
            tmp2 += b[i-1] * ((b[i-2] & b[i-1]) ^ (b[i-1] | b[i]) ^ (b[i-2] + b[i]));
        }
    }
    int64_t tmp3 = findMin(i-2, j-1, dp, b, total, last_cut);
    if(tmp3 != inf){
        tmp3 += (b[i-1] + b[i] - abs(b[i-1] - b[i]));
    }
    int64_t tmp = findMin(i-1, j-1, dp, b, total, last_cut) + b[i];

    if(tmp < tmp2){
        last_cut[i][j] = i;
    }
    else{
        last_cut[i][j] = last_cut[i-1][j];
        tmp = tmp2;
    }
    if(tmp3 < tmp){
        last_cut[i][j] = i-1;
        tmp = tmp3;
    }
    return dp[i][j] = tmp;
}

int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);

    cin >> N >> M;
    vector<int64_t> a(N+1);
    for(int i = 0; i < N; ++i){
        cin >> a[i];
    }
    int k = (5 * N / M) + 9;

    random_device rd;   // non-deterministic generator
    mt19937 gen(rd());  // to seed mersenne twister.
    uniform_int_distribution<> dist(0, N-1); 

    int64_t ans = inf;
    vector<int64_t> b(N);
    vector<bool> cut(N, 0);
    while(k--){
        int cut_point = dist(gen);
        if(cut[cut_point]){
            continue;
        }
        cut[cut_point] = 1;

        int i = 0;
        if(cut_point){
            for(; i < N-cut_point; ++i){
                b[i] = a[cut_point+i];
            }
            for(int j = 0; i < N; ++i, ++j){
                b[i] = a[j];
            }
        }
        
        vector<vector<int64_t>> dp(N, vector<int64_t>(M, n_inf)); 
        vector<vector<int>> last_cut(N, vector<int>(M));

        if(cut_point){
            ans = min(ans, findMin(N-1, M-1, dp, b, calc(b), last_cut));
        }
        else{
            ans = min(ans, findMin(N-1, M-1, dp, a, calc(a), last_cut));
        } 
    }
    cout << ans << '\n';
    return 0;
}