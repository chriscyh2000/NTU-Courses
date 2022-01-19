// I only discussed this problem with TA
#include <iostream>
#include <vector>
#include <queue>
using namespace std;
typedef pair<long long, long long>  ll_pair;
typedef pair<int, int>  ii_pair;
typedef struct info{
    ii_pair idx;
    ll_pair val;
}Info;
typedef struct lf{
    int l;
    int r;
}LF;
typedef priority_queue<Info, vector<Info>, greater<Info>> pq;
bool operator<(const Info &p1, const Info &p2){
    return (p1.val.first + p1.val.second) < (p2.val.first + p2.val.second); 
}
bool operator>(const Info &p1, const Info &p2){
    return (p1.val.first + p1.val.second) > (p2.val.first + p2.val.second); 
}
const long long n_inf = -1e15;
void toAlt(vector<long long> &a, vector<long long> &b, int N, pq &d1, pq &d2, long long *sum){
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
        if(b.size() > 1){
            Info toPush;
            toPush.idx = make_pair(b.size()-2, b.size()-1);
            toPush.val = make_pair(b[b.size()-2], b[b.size()-1]);
            d1.push(toPush);
            d2.push(toPush);
        }
        *sum += cur;
        pn = !pn;
    }
}
int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int T, flag;
    cin >> T >> flag;
    while(T--){
        int N;
        cin >> N;
        vector<long long> a(N);
        vector<long long> b;
        vector<long long> ans(N+1, {0});
        pq d[2];
        long long sum = 0;
        for(int i = 0; i < N; ++i){
            cin >> a[i];
        }
        toAlt(a, b, N, d[0], d[1], &sum);
        int head[2], tail[2];
        head[0] = head[1] = 0; 
        tail[0] = tail[1] = b.size()-1;
        bool check[2][(int)b.size()];
        vector<LF> rec[2];
        for(int i = 0; i < b.size(); ++i){
            check[0][i] = check[1][i] = 0;
            LF lf1, lf2;
            lf1.l = lf2.l = i-1;
            lf1.r = lf2.r = i+1;
            rec[0].push_back(lf1);
            rec[1].push_back(lf2);
        }
        ans[b.size()] = sum;
        if(b[0] < b.back()){
            ans[b.size()-1] = sum - b[0];
            head[1] = 1;
            check[1][0] = 1;
        }
        else{
            ans[b.size()-1] = sum - b.back();
            tail[1] = b.size()-2;
            check[1][b.size()-1] = 1;
        }
        int cur = b.size()-2, r = b.size()-2;
        if(flag) r = max(0, N/2-N+(int)b.size());
        int i = 0;
        while(r > 0){
            while(!d[i%2].empty() && (check[i%2][d[i%2].top().idx.first] || check[i%2][d[i%2].top().idx.second])){
                d[i%2].pop();
            }
            if(b[head[i%2]] + b[tail[i%2]] <= d[i%2].top().val.first + d[i%2].top().val.second){
                ans[cur] = ans[cur+2] - (b[head[i%2]] + b[tail[i%2]]);
                check[i%2][head[i%2]] = 1;
                check[i%2][tail[i%2]] = 1;
                head[i%2] = rec[i%2][head[i%2]].r;
                tail[i%2] = rec[i%2][tail[i%2]].l;
            }
            else{
                ans[cur] = ans[cur+2] - (d[i%2].top().val.first + d[i%2].top().val.second);
                check[i%2][d[i%2].top().idx.first] = check[i%2][d[i%2].top().idx.second] = 1;
                if(head[i%2] == d[i%2].top().idx.first){
                    head[i%2] = rec[i%2][d[i%2].top().idx.second].r;
                    d[i%2].pop();
                }
                else if(tail[i%2] == d[i%2].top().idx.second){
                    tail[i%2] = rec[i%2][d[i%2].top().idx.first].l;
                    d[i%2].pop();
                }
                else{
                    Info toPush;
                    int idx_l = d[i%2].top().idx.first, idx_r = d[i%2].top().idx.second;
                    toPush.idx = make_pair(rec[i%2][idx_l].l, rec[i%2][idx_r].r);
                    toPush.val = make_pair(b[rec[i%2][idx_l].l], b[rec[i%2][idx_r].r]);
                    rec[i%2][rec[i%2][idx_l].l].r = rec[i%2][idx_r].r;
                    rec[i%2][rec[i%2][idx_r].r].l = rec[i%2][idx_l].l;
                    d[i%2].pop();
                    d[i%2].push(toPush);
                }
            }
            ++i;
            --cur;
            --r;
        }
        for(int i = 1; i <= N; ++i){
            cout << ans[i];
            if(i < N) cout << ' ';
            else cout << endl;
        }
    }
    return 0;
}