/*
討論對象：
b06303131 沈家睿
b05504066 李旻翰
b08501098 柯晨緯
*/
#include <iostream>
#include <vector>
#include <map>
using namespace std;
typedef struct node{
    long long size;
    int pos;
    struct node* parent;
}Node;
long long calc(long long num){
    if(num < 2) return 0;
    else return (num * (num-1)) / 2;
}
Node* findParent(Node *cur){
    --cur->size;
    if(cur->parent == cur) return cur;
    else return cur->parent = findParent(cur->parent);
}
int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int N, Q;
    cin >> N >> Q;
    vector<Node *> cat(N+1);
    map<int, Node *> mp;
    int tmp;
    for(int i = 1; i <= N; ++i){
        cin >> tmp;
        cat[i] = (Node *)malloc(sizeof(Node));
        cat[i]->parent = cat[i];
        cat[i]->pos = tmp;
        cat[i]->size = 1;
        auto it = mp.insert(make_pair(tmp, cat[i]));
        if(!it.second){
            cat[i]->parent = it.first->second;
            ++it.first->second->size;
        }
    }
    long long ans = 0;
    for(auto it = mp.cbegin(); it != mp.cend(); ++it){
        ans += calc(it->second->size);
    }
    int flag, a, b;
    for(int i = 0; i < Q; ++i){
        cin >> flag >> a >> b;
        if(flag == 1){
            auto s = mp.lower_bound(a-b);
            auto e = mp.upper_bound(a+b);
            if(s == mp.end() || s == e){
                cout << ans << '\n';
                continue;
            } 
            Node* tmp_parent = s->second;
            ans -= calc(tmp_parent->size);
            s = mp.erase(s);
            while(s != e){
                ans -= calc(s->second->size);
                if(tmp_parent->size < s->second->size){
                    tmp_parent->parent = s->second;
                    s->second->size += tmp_parent->size;
                    tmp_parent = s->second;
                }
                else{
                    s->second->parent = tmp_parent;
                    tmp_parent->size += s->second->size;
                }
                s = mp.erase(s);
            }
            tmp_parent->pos = a;
            mp.insert(make_pair(a, tmp_parent));
            ans += calc(tmp_parent->size);
        }
        else{
            Node *root = findParent(cat[a]);
            ans -= calc(root->size+1);
            ans += calc(root->size);
            if(!root->size) mp.erase(root->pos);
            cat[a] = (Node *)malloc(sizeof(Node));
            cat[a]->parent = cat[a];
            cat[a]->pos = b;
            cat[a]->size = 1;
            auto it = mp.insert(make_pair(b, cat[a]));
            if(!it.second){
                cat[a]->parent= it.first->second;
                ans -= calc(it.first->second->size);
                ++it.first->second->size;
            }
            ans += calc(it.first->second->size);
        }
        cout << ans << '\n';
    }
}