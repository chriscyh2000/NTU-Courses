#include <stdio.h>
#include <assert.h>
#define MAXN 65536
int main(void)
{
    FILE *fin = fopen("test", "rb");
    FILE *fout = fopen("test.enc", "wb");
    assert(fin != NULL);
    assert(fout != NULL);
    int key, size;
    char buff[MAXN];
    scanf("%d", &key);
    while((size = fread(buff, sizeof(char), MAXN, fin)) != 0)
    {
        for(int i = 0; i < size; i++)
            buff[i] ^= key;
        fwrite(buff, sizeof(char), size, fout);
    }
    fclose(fin);
    fclose(fout);
    return 0;
}