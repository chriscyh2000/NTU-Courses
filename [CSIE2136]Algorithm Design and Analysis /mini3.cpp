#include <iostream>
#include <vector>
using namespace std;
vector<int> dp(11, {0});
int count(vector<int> &dp, int i){
    if(dp[i] > 0) return dp[i];
    int sum = 0;
    for(int j = 0; j < i; ++j){
        sum += count(dp, j) * count(dp, i - j - 1);
    }
    dp[i] = sum;
    return sum;
}
int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    dp[0] = dp[1] = 1;
    int N;
    cin >> N;
    cout << count(dp, N) << endl;
    return 0;
}   