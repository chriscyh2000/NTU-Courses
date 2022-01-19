#include "ada-hw4-p3.h"

// feel free to use global variables
std::vector<int> contest;
std::vector<int> id_vec;
std::vector<bool> concealed(3004);
std::vector<int> pair(3004);

std::vector<int> init(int N) {
    int l = 0, r = 1;
    for(int i = 0; i < N; ++i){
        id_vec.push_back(i);
    }
    while(r < N){
        while(l >= 0 && concealed[l]){
            --l;
        }
        if(l == -1){
            l = r;
            ++r;
            if(r >= N) break;
        }
        if(!compare(l, r)){
            contest.push_back(l);
            contest.push_back(r);
            concealed[l] = concealed[r] = 1;
            pair[l] = r;
            pair[r] = l;
        }
        else{
            l = r;
        }
        ++r;
    }
    return contest;
}

std::vector<int> insert(int p, int id) {
    id_vec.insert(id_vec.begin() + p, id);
    int l = p - 1, r = p + 1;
    while(l >= 0 && concealed[id_vec[l]]){
        --l;
    }
    if(l >= 0 && !compare(id_vec[l], id)){
        contest.push_back(id);
        contest.push_back(id_vec[l]);
        concealed[id_vec[l]] = concealed[id] = 1;
        pair[id] = id_vec[l];
        pair[id_vec[l]] = id;
        return contest;
    }
    while(r < id_vec.size() && concealed[id_vec[r]]){
        ++r;
    }
    if(r < id_vec.size() && !compare(id, id_vec[r])){
        contest.push_back(id);
        contest.push_back(id_vec[r]);
        concealed[id_vec[r]] = concealed[id] = 1;
        pair[id] = id_vec[r];
        pair[id_vec[r]] = id;
    }
    return contest;
}

std::vector<int> remove(int p) {
    if(concealed[id_vec[p]]){
        concealed[pair[id_vec[p]]] = 0;

        for(auto it = contest.begin(); it < contest.end(); ++it){
            if(pair[id_vec[p]] == *it){
                contest.erase(it);
                break;
            }
        }
        for(auto it = contest.begin(); it < contest.end(); ++it){
            if(id_vec[p] == *it){
                contest.erase(it);
                break;
            }
        }

        int tmp = id_vec[p];
        id_vec.erase(id_vec.begin() + p);

        if(id_vec.empty()) return contest;

        int i = 0;
        while(id_vec[i] != pair[tmp]) ++i;
        int l = i - 1, r = i + 1;
        while(l >= 0 && concealed[id_vec[l]]){
            --l;
        }
        if(l >= 0 && !compare(id_vec[l], id_vec[i])){
            contest.push_back(id_vec[i]);
            contest.push_back(id_vec[l]);
            concealed[id_vec[l]] = concealed[id_vec[i]] = 1;
            pair[id_vec[i]] = id_vec[l];
            pair[id_vec[l]] = id_vec[i];
            return contest;
        }
        while(r < id_vec.size() && concealed[id_vec[r]]){
            ++r;
        }
        if(r < id_vec.size() && !compare(id_vec[i], id_vec[r])){
            contest.push_back(id_vec[i]);
            contest.push_back(id_vec[r]);
            concealed[id_vec[r]] = concealed[id_vec[i]] = 1;
            pair[id_vec[i]] = id_vec[r];
            pair[id_vec[r]] = id_vec[i];
        }
    }
    else id_vec.erase(id_vec.begin() + p);
    
    return contest;
}
