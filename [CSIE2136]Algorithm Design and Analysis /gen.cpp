#include <iostream>
#include <vector>
#include <random>
#include <cstdlib>
#include <algorithm>
using namespace std;
int main(){
    int T, flag;
    // cin >> T >> flag;
    // vector<unsigned long long> ivec(T);
    random_device rd;   // non-deterministic generator
    mt19937 gen(rd());  // to seed mersenne twister.
    uniform_int_distribution<> distN(500,500); // distribute results between 1 and 6 inclusive.
    uniform_int_distribution<> distNum(1e9, 1e9);
    uniform_int_distribution<> dist3(1,1e6);
    // cout << T << ' ' << flag << endl;
    // for (int i = 0; i < T; ++i) {
        int N = 500;
        cout << N << endl;
        int num;
        for(int j = 0; j < N; ++j){
            num = distNum(gen);
            while(num == 0) num = distNum(gen);
            cout << num << ' ';
        }
        cout << endl;
    // }
    // cout << endl << endl;
    // unsigned long long ans = 0;
    // for(int i = 0; i < N; ++i){
    //     for(int j = i + 1; j < N; ++j){
    //         if(ivec[i] > ivec[j]){
    //             ans += ((ivec[i] % 10000019) + (ivec[j] % 10000019)) % 10000019;
    //             ans %= 10000019;
    //         }
    //     }
    // }
    // sort(ivec.begin(), ivec.end());
    // for(int i = 0; i < N; ++i) cout << ivec[i] << " ";
    // cout << endl;
    // cout << endl << ans << endl;
}