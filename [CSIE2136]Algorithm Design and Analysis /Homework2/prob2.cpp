// I discussed this problem with b06303131 沈家睿
#include <iostream>
#include <vector>
using namespace std;
long long GCD[505][505] = {0};
int num[505];
long long dp[505][505] = {0};
int N;
const long long n_inf = -1e16;
int gcd(int a, int b){
    if(b > a) swap(a, b);
    if(!b) return a;
    return gcd(b, a%b);
}
long long getGcd(int i, int j){
    if(GCD[i][j]) return GCD[i][j];
    return GCD[i][j] = (long long)gcd(num[i], num[j]);
}
long long f(int i, int j){
    // cout << i << ' ' << j << endl;
    if(dp[i][j] != 0) return dp[i][j];
    if(j < i) return 0;
    if(i == j) return n_inf;
    if(i == j-1){
        if(getGcd(i, j) == 1) return dp[i][j] = n_inf;
        return dp[i][j] = getGcd(i, j);
    }
    if(i == j-2){
        if(getGcd(i, i+1) == 1 || getGcd(i, j) == 1 || getGcd(i+1, j) == 1)
            return dp[i][j] = n_inf;
        return dp[i][j] = getGcd(i, i+1) + getGcd(i+1, j);
    }
    long long ans = n_inf;
    for(int n = i; n <= j-2; ++n){
        ans = max(ans, f(i, n) + f(n+1, j));
    }
    if(getGcd(i, j) != 1){
        ans = max(ans, getGcd(i, j) + f(i+1, j-1));
    }
    else return dp[i][j] = ans;
    for(int n = i+1; n <= j-1; ++n){
        if(getGcd(i, n) == 1 || getGcd(n, j) == 1) continue;
        ans = max(ans, f(i+1, n-1) + getGcd(i, n) + f(n+1, j-1) + getGcd(n, j));
    }
    return dp[i][j] = ans;
}
int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    cin >> N;
    for(int i = 0; i < N; ++i){
        cin >> num[i];
    }
    if(f(0, N-1) <= 0) cout << -1 << endl;
    else cout << f(0, N-1) << endl;
    return 0;
}