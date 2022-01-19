#include <iostream>
#include <vector>
#include <array>
const long long n_inf = (long long)-1e16;
using namespace std;
void toAlt(vector<long long> &a, vector<long long> &b, int N){
    int i = 0;
    long long cur;
    bool pn = (a[0] > 0);
    while(i < N){
        cur = n_inf;
        if(pn){
            while(i < N && a[i] > 0){
                if(cur < a[i]){
                    cur = a[i];
                }
                ++i;
            }
            b.push_back(cur);
        }
        else{
            while(i < N && a[i] < 0){
                if(cur < a[i]){
                    cur = a[i];
                }
                ++i;
            }
            b.push_back(cur);
        }
        pn = !pn;
    }
}
int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int T, flag;
    cin >> T >> flag;
    while(T--){
        int count, N;
        cin >> N;
        count = N;
        vector<long long> a(N);
        vector<long long> b;
        for(int i = 0; i < N; ++i){
            cin >> a[i];
        }
        toAlt(a, b, N);

        long long pmax[2][b.size()+1], nmax[2][b.size()+1];
        long long p = n_inf, n = n_inf;
        int stop, start;

        if(flag){
            stop = max(0, (N/2) - N + (int)b.size());
        }
        else{
            stop = b.size();
        } 
        start = stop + 1;

        if(stop){
            long long ans = n_inf;
            for(int i = 0; i < b.size(); ++i){
                if(b[i] > 0){
                    pmax[1][i] = max(p, b[i]);
                    p = pmax[1][i];
                    ans = max(ans, p);
                }
                else{
                    nmax[1][i] = max(n, b[i]);
                    n = nmax[1][i];
                    ans = max(ans, n);
                }
            }
            
            cout << ans;
            --count;
            if(count) cout << ' ';
            else cout << endl;

            for(int j = 2; j <= stop; ++j){
                ans = n_inf;
                for(int i = j - 1; i < b.size(); ++i){
                    if(b[i] > 0){
                        if(i < j+1) pmax[j%2][i] = nmax[(j-1)%2][i-1] + b[i];
                        else pmax[j%2][i] = max(pmax[j%2][i-2], nmax[(j-1)%2][i-1] + b[i]);
                        ans = max(ans, pmax[j%2][i]);
                    }
                    else{
                        if(i < j+1) nmax[j%2][i] = pmax[(j-1)%2][i-1] + b[i];
                        else nmax[j%2][i] = max(nmax[j%2][i-2], pmax[(j-1)%2][i-1] + b[i]);
                        ans = max(ans, nmax[j%2][i]);
                    }
                }
                cout << ans;
                --count;
                if(count) cout << ' ';
                else cout << endl;
            }
        }
        for(int i = start; i <= N; ++i){
            cout << 0;
            --count;
            if(count) cout << ' ';
            else cout << endl;
        }
    }
    return 0;
}