// I finished the problem all by myself. No discussion with anyone else.
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
double a, b, c;
double cal(int p){
    double pp = p;
    return (pp * (b / a) + (c / a));
}
bool cmp(const pair<int, int> &i1, const pair<int, int> &i2){
    return (i1.second < i2.second);
}
void merge_p(vector<int> &pvec, vector<int> &l_part, vector<int> &r_part, int l){
    int i = 0, j = 0;
    while(i < l_part.size() && j < r_part.size()){
        if(l_part[i] <= r_part[j]){
            pvec[l] = l_part[i];
            ++l;
            ++i;
        }
        else{
            pvec[l] = r_part[j];
            ++l;
            ++j;
        }
    }
    if(i < l_part.size()){
        while(i < l_part.size()){
            pvec[l] = l_part[i];
            ++l;
            ++i;
        }
    }
    else{
        while(j < r_part.size()){
            pvec[l] = r_part[j];
            ++l;
            ++j;
        }
    }
}
void mergesort_p(vector<int> &pvec, int l, int r, long long *count){
    if(l == r) return;
    if(l == r - 1){
        if(cal(pvec[l]) < (double)pvec[r]){
            ++(*count);
        }
        if(pvec[l] > pvec[r]){
            swap(pvec[l], pvec[r]);
        }
        return;
    }
    int mid = (l + r) / 2;
    vector<int> l_part(pvec.begin() + l, pvec.begin() + mid + 1);
    vector<int> r_part(pvec.begin() + mid + 1, pvec.end());
    mergesort_p(l_part, 0, l_part.size() - 1, count);
    mergesort_p(r_part, 0, r_part.size() - 1, count);
    int i = 0, j = 0;
    while(i < l_part.size() && j < r_part.size()){
        while(cal(l_part[i]) >= (double)(r_part[j])){
            ++j;
            if(j == r_part.size()) break;
        } 
        (*count) += (r_part.size() - j);
        ++i;
    }
    merge_p(pvec, l_part, r_part, l);
}
int main(){
    int N;
    int pi, zi;
    cin >> N >> a >> b >> c;
    vector<pair<int, int>> arr(N);
    for(int i = 0; i < N; ++i){
        cin >> pi >> zi;
        arr[i] = make_pair(pi, zi);
    }
    stable_sort(arr.begin(), arr.end(), cmp);
    long long count = 0;
    vector<int> pvec(N);
    int i = 0, cur = arr[0].second;
    long long count2 = 0;
    while(i < N){
        vector<int> vec;
        while(arr[i].second == cur && i < N){
            vec.push_back(arr[i].first);
            ++i;
        }
        if(vec.size() > 1){
            mergesort_p(vec, 0, vec.size()-1, &count2);
        }
        if(i < N) cur = arr[i].second;
        // cout << i << endl;
    }
    for(int i = 0; i < N; ++i){
        pvec[i] = arr[i].first;
    }
    mergesort_p(pvec, 0, pvec.size()-1, &count);
    cout << count - count2 << endl;
    return 0;
}