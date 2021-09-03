#include <stdio.h>
#include <stdlib.h>
int hasEOF = 0;
int readchar() {
    static int N = 1<<20;
    static char buf[1<<20];
    static char *p = buf, *end = buf;
    if(p == end) {
        if((end = buf + fread(buf, 1, N, stdin)) == buf) {
            hasEOF = 1;
            return EOF;    
        }
        p = buf;
    }
    return *p++;
}
int ReadInt(int *x) {
    char c, neg;
    while((c = readchar()) < '-')    {if(c == EOF) return 0;}
    neg = (c == '-') ? -1 : 1;
    *x = (neg == 1) ? c-'0' : 0;
    while((c = readchar()) >= '0')
        *x = (*x << 3) + (*x << 1) + c-'0';
    *x *= neg;
    return 1;
}
int main() {
    long long int shelf = 0;
    int book;
    while (ReadInt(&book)) {
        // add your code
        int i;
        for(i = 0; i <= 56; i += 8)
        {
            if(((shelf >> i) & 255) == book)
            {
                shelf ^= ((long long int)book << i);
                long long int temp1;
                long long int temp2;
                temp1 = (shelf >> i) << i;
                temp2 = shelf ^ temp1;
                shelf = (temp1 >> 8) + temp2;
                shelf = (shelf << 8) + book;
                break;
            }
        }
        if(i == 64)
        {
            shelf <<= 8;
            shelf += book; 
        }
    }    
    // output your answer
    for(int i = 56; i >= 0; i -= 8){
		printf("%lld", ((shelf >> i) & 255));
		if(i != 0) 
            printf(" ");
	}
	printf("\n");
    return 0;
}