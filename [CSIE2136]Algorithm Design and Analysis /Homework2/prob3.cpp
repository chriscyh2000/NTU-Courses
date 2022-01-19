// I discussed this problem with b08501098 柯晨緯
#include <iostream>
#include <vector>
#include <array>
#include <queue>
#include <algorithm>
using namespace std;
typedef struct infos{
    long long s;
    long long e;
    long long x;
    long long p;
}Info;
bool cmp(const Info &a1, const Info &a2){
    return (a1.p > a2.p);
}
bool cmp2(const Info &a1, const Info &a2){
    return (a1.s < a2.s);
}
bool operator<(const Info &a1, const Info &a2){
    return a1.e < a2.e;
}
bool operator>(const Info &a1, const Info &a2){
    return a1.e > a2.e;
}
int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int N;
    cin >> N;
    vector<Info> info(N);
    long long s, e, x, p;
    cin >> s >> e >> x >> p;
    info[0].s = s;
    info[0].e = e;
    info[0].x = x;
    info[0].p = p;
    bool check = true;
    for(int i = 1; i < N; ++i){
        cin >> s >> e >> x >> p;
        info[i].s = s;
        info[i].e = e;
        info[i].x = x;
        info[i].p = p;
        if(p != info[i-1].p) check = false;
    }
    sort(info.begin(), info.end(), cmp);
    long long totalP = 0, curMaxTime = 0;
    vector<Info> ivec;
    if(!check){
        for(int k = 0; k < N; ++k){
            ivec.push_back(info[k]);
            sort(ivec.begin(), ivec.end(), cmp2);
            int i = 0;
            long long current, temp = 0;
            priority_queue<Info, vector<Info>, greater<Info>> pq;
            while(i < ivec.size() || !pq.empty()){
                if(pq.empty() && i < ivec.size()){
                    int s = ivec[i].s;
                    while(i < ivec.size() && s == ivec[i].s)
                        pq.push(ivec[i++]);
                    current = s;
                }
                else{
                    while(i < ivec.size() && current == ivec[i].s){
                        pq.push(ivec[i++]);
                    }
                }
                if(i < ivec.size()){
                    if(pq.top().e >= ivec[i].s){
                        if(current + pq.top().x > ivec[i].s){
                            temp += (ivec[i].s - current);
                            Info toPush = pq.top();
                            pq.pop();
                            toPush.x -= (ivec[i].s - current);
                            current = ivec[i].s;
                            pq.push(toPush);
                        }
                        else{
                            temp += pq.top().x;
                            current += pq.top().x;
                            pq.pop();
                        }
                    }
                    else{
                        if(pq.top().e - current + 1 < pq.top().x){
                            temp += (pq.top().e - current + 1);
                            current = pq.top().e + 1;
                            pq.pop();
                        }
                        else{
                            temp += pq.top().x;
                            current += pq.top().x; 
                            pq.pop();
                        }
                    }
                }
                else if(!pq.empty()){
                    if(pq.top().e - current + 1 < pq.top().x){
                        temp += (pq.top().e - current + 1);
                        current = pq.top().e + 1;
                        pq.pop();
                    }
                    else{
                        temp += pq.top().x;
                        current += pq.top().x; 
                        pq.pop();
                    }
                }
            }
            if(curMaxTime < temp){
                totalP += (temp - curMaxTime) * (info[k].p);
                curMaxTime = temp;
            }
            else{
                for(auto it = ivec.begin(); it < ivec.end(); ++it){
                    if((*it).s == info[k].s && (*it).e == info[k].e && (*it).p == info[k].p && (*it).x == info[k].x){
                        ivec.erase(it, it+1);
                        break;
                    }
                }
            }
        }
    }
    else{
        int i = 0;
        long long current;
        priority_queue<Info, vector<Info>, greater<Info>> pq;
        sort(info.begin(), info.end(), cmp2);
        while(i < N || !pq.empty()){
            if(pq.empty() && i < N){
                int s = info[i].s;
                while(i < N && s == info[i].s)
                    pq.push(info[i++]);
                current = s;
            }
            else{
                while(i < N && current == info[i].s){
                    pq.push(info[i++]);
                }
            }
            if(i < N){
                if(pq.top().x + current > info[i].s){
                    totalP += pq.top().p * (info[i].s - current);
                    Info temp = pq.top();
                    pq.pop();
                    temp.x -= (info[i].s - current);
                    current = info[i].s;
                    pq.push(temp);
                }
                else{
                    totalP += pq.top().p * pq.top().x;
                    current += pq.top().x;
                    pq.pop();
                }
            }
            else if(!pq.empty()){
                if(pq.top().e - current + 1 < pq.top().x){
                    totalP += pq.top().p * (pq.top().e - current + 1);
                    current = pq.top().e + 1;
                    pq.pop();
                }
                else{
                    totalP += pq.top().p * pq.top().x;
                    current += pq.top().x; 
                    pq.pop();
                }
            }
        }
    }
    cout << totalP << endl;
    return 0;
}