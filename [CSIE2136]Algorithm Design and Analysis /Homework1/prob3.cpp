//I discussed the problem with 林胤辰 on Gather Town.
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
#define x first
#define y second
typedef pair<int, int> pos;
bool cmp(const pos &p1, const pos &p2){
    return p1.x < p2.x;
}
bool cmp2(const pos &p1, const pos &p2){
    return p1.y < p2.y;
}
void toCount(vector<pos> &point, int l, int r, long long *count){
    if(l == r) return;
    if(l == r - 1){
        ++(*count);
        return;
    }

    int mid = (l + r) / 2;
    toCount(point, l, mid, count);
    toCount(point, mid+1, r, count);

    vector<pos> l_side(point.cbegin() + l, point.cbegin() + (mid+1)); //will be sorted by y
    vector<pos> r_side(point.cbegin() + (mid+1), point.cbegin() + (r+1));
    sort(l_side.begin(), l_side.end(), cmp2);
    sort(r_side.begin(), r_side.end(), cmp2);

    vector<pos> l_stk1;
    vector<pos> r_stk1;
    int i = 0, j = 0;
    while(i < l_side.size()){
        while(!l_stk1.empty() && l_stk1.back().x < l_side[i].x){
            l_stk1.pop_back();
        }    
        while(j < r_side.size() && r_side[j].y < l_side[i].y){
            while(!r_stk1.empty() && r_stk1.back().x > r_side[j].x){
                r_stk1.pop_back();
            }
            r_stk1.push_back(r_side[j]);
            ++j;
        }
        if(l_stk1.size()){
            auto it = lower_bound(r_stk1.cbegin(), r_stk1.cend(), l_stk1.back(), cmp2);
            (*count) += (r_stk1.cend() - it);
        }
        else (*count) += r_stk1.size();
        l_stk1.push_back(l_side[i]);
        ++i;
    }
    
    vector<pos> l_stk2;
    vector<pos> r_stk2;
    i = 0; j = 0;
    while(i < r_side.size()){
        while(!r_stk2.empty() && r_stk2.back().x > r_side[i].x){
            r_stk2.pop_back();
        }    
        while(j < l_side.size() && l_side[j].y < r_side[i].y){
            while(!l_stk2.empty() && l_stk2.back().x < l_side[j].x){
                l_stk2.pop_back();
            }
            l_stk2.push_back(l_side[j]);
            ++j;
        }
        if(r_stk2.size()){
            auto it = lower_bound(l_stk2.cbegin(), l_stk2.cend(), r_stk2.back(), cmp2);
            (*count) += (l_stk2.cend() - it);
        }
        else (*count) += l_stk2.size();
        r_stk2.push_back(r_side[i]);
        ++i;
    }
}
int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int N;
    cin >> N;
    vector<pos> point(N);
    for(int i = 0; i < N; ++i){
        cin >> point[i].x >> point[i].y;
    }
    sort(point.begin(), point.end(), cmp);
    long long count = 0;
    toCount(point, 0, N-1, &count);
    cout << count << endl;
    return 0;
}