#include <iostream>
#include <vector>

#define ll long long
const ll  n_inf = -1e18;
using namespace std;
int main(){
    int k;
    cin >> k;
    vector<int> idx(k+1);
    int z = k-1, rec = 0;
    while(z > 3){
        idx[z] = rec++;
        printf("idx[%d]=%d\n", z, idx[z]);
        z = z - z/3;
    }
    idx[2] = rec++;
    idx[1] = rec++;
    idx[0] = rec++;
    printf("idx[2]=%d\n", idx[2]);
    printf("idx[1]=%d\n", idx[1]);
    printf("idx[0]=%d\n", idx[0]);
}