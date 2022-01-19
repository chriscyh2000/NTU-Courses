// I finished the problem all by myself.
#include <iostream>
#include <vector>
#define n_inf -1e18
using namespace std;

int N; 
long long x, y, z;
long long maxSub_cross(vector<int> &avec, int l, int r, int mid){
    long long l_ans = avec[mid] * x, r_ans = avec[mid+1] * z;
    long long record = 0;
    for(int i = mid; i > l; --i){
        record += avec[i];
        l_ans = max(l_ans, record * y + avec[i-1] * x);
    }
    record = 0;
    for(int i = mid + 1; i < r; ++i){
        record += avec[i];
        r_ans = max(r_ans, record * y + avec[i+1] * z);
    }
    return l_ans + r_ans;
}
long long maxSub(vector<int> &avec, int l, int r){
    if(l == r) return n_inf;
    if(r == l + 1) return (x * avec[l] + z * avec[r]);
    long long a, b, c;
    int mid = (l + r) / 2;
    a = maxSub(avec, l, mid);
    b = maxSub(avec, mid+1, r);
    c = maxSub_cross(avec, l, r, mid);
    return max(max(a, b), c);
}
int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    cin >> N >> x >> y >> z;
    vector<int> avec(N);
    for(int i = 0; i < N; ++i){
        cin >> avec[i];
    }
    cout << maxSub(avec, 0, N-1) << endl;
    return 0;
}