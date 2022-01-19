#include "ypglpk.hpp"
#include <bitset>
#include <vector>
#include <iostream>
#define MAXN 80

using namespace std;
void BronKerbosch(vector<bitset<MAXN>> &neb, vector<vector<int>> &adj, vector<bitset<MAXN>> &cliques, bitset<MAXN> P, bitset<MAXN> R, bitset<MAXN> X){
    if((P | X).none()){
        cliques.push_back(R);
        return;
    }

    bitset<MAXN> PX = P | X; 
    int count = 0, pivot = 0;
    while(PX.any()){
        int u = PX._Find_first();
        if(count < adj[u].size()){
            count = adj[u].size();
            pivot = u;
        }
        PX[u] = 0;
    }
    bitset<MAXN> newP =  P ^ (P & neb[pivot]);
    
    while(newP.any()){
        int first = newP._Find_first();
        bitset<MAXN> tmp = 1;
        BronKerbosch(neb, adj, cliques, P & neb[first], R | (tmp << first), X & neb[first]);
        newP[first] = 0;
        P[first] = 0;
        X[first] = 1;
    }
}
int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int T, N, M;
    cin >> T;
    while(T--){
        cin >> N >> M;

        if(!M){
            cout << N << '\n';
            for(int i = 0; i < N; ++i){
                cout << "1 " << i << '\n';
            }
            continue;
        }

        vector<vector<int>> adj(N);
        vector<bitset<MAXN>> cliques;
        vector<bitset<MAXN>> neb(N);
        bitset<MAXN> P = 0, R = 0, X = 0;

        int u, v;
        for(int i = 0; i < N; ++i) P[i] = 1;
        for(int i = 0; i < M; ++i){
            cin >> u >> v;
            adj[u].push_back(v);
            adj[v].push_back(u);
            neb[u][v] = 1;
            neb[v][u] = 1;
        }
        BronKerbosch(neb, adj, cliques, P, R, X);

        vector<vector<double>> A(cliques.size() * 2 + N, vector<double>(cliques.size(), 0));
        vector<double> b(cliques.size() * 2 + N, 0), c(cliques.size(), -1);
        vector<bool> isint(cliques.size() * 2 + N, 1);

        for(int i = 0; i < cliques.size(); ++i){
            A[i][i] = 1;
            A[i+cliques.size()][i] = -1;
            b[i] = 1;
        }
        for(int i = 0; i < cliques.size(); ++i){
            for(int j = 0; j < N; ++j){
                A[j+2*cliques.size()][i] = (cliques[i][j]) ? -1 : 0;
            }
        }
        for(int i = 0; i < N; ++i){
            b[2*cliques.size()+i] = -1;
        }

        pair<double, vector<double>> ans = ypglpk::mixed_integer_linear_programming(A, b, c, isint);
        vector<bool> clicked(N, 0);

        cout << (int)(ans.first*(-1)) << '\n';
        for(int i = 0; i < ans.second.size(); ++i){
            if((int)ans.second[i]){
                vector<int> clique;
                while(cliques[i].any()){
                    int first = cliques[i]._Find_first();
                    cliques[i][first] = 0;
                    if(!clicked[first]){
                        clicked[first] = 1;
                        clique.push_back(first);
                    }
                }
                cout << clique.size() << ' ';
                for(int j = 0; j < clique.size(); ++j){
                    cout << clique[j] << ((j < clique.size()-1) ? ' ' : '\n');
                }
            }
        }
    }
    return 0;
}