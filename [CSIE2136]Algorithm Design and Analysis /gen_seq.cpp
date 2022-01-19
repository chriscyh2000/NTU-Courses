#include <iostream>
using namespace std;

int main(){
	srand(time(NULL));
	int N = rand() % 1000 + 1;
	int M = rand() % 500 + 1;
	cout << N << ' ' << M << '\n';
	for(int i = 0;i < N;i++) cout << rand() % 1000 << ' ';
}