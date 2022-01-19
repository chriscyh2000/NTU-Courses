// I finished the problem all by myself. No discussion with anyone else.
#include <iostream>
#include <vector>
#include <array>
using namespace std;
const long long mod = 998244353;
int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int n;
    cin >> n;
    vector<long long> record(n+1);
    array<vector<int>, 3> peg;
    record[0] = 0;
    int k = 1;
    for(int i = 0; i < 3; ++i){
        int ki, input;
        cin >> ki;
        for(int j = 0; j < ki; ++j, ++k){
            record[k] = ((record[k-1] + 1) * 2 - 1) % mod;
            cin >> input;
            peg[i].push_back(input);
        }
    }
    long long ans = 0;
    int src = 0, buf = 1, des = 2, b = n;
    int start[3] = {0};
    bool check = true;
    //check = 1 -> +; check = 0 -> -;
    while(true){
        if(check){
            while(start[des] < peg[des].size() && peg[des][start[des]] == b){
                --b;
                ++start[des];
                swap(src, buf);
            }
            ans += record[b];
            ans %= mod;
            if(ans < 0) ans += mod;
        }
        else{
            if(start[src] >= peg[src].size() || peg[src][start[src]] != b){
                cout << -1 << endl;
                return 0;
            }
            --b;
            ++start[src];
            swap(des, buf);
            while(start[src] < peg[src].size() && peg[src][start[src]] == b){
                --b;
                ++start[src];
                swap(des, buf);
            }
            ans -= record[b];
            ans %= mod;
            if(ans < 0) ans += mod;
        }
        check = !check;
        if(!b) break;
    }
    cout << ans << endl;
    return 0;
}