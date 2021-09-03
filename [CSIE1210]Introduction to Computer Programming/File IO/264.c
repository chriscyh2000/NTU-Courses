#include <stdio.h>
#include <assert.h>
#define MAXFILENAME 201
#define MAXN 65536
int main(void)
{
    char filename[MAXFILENAME];
    scanf("%s", filename);

    FILE *fp = fopen(filename, "rb");
    assert(fp != NULL);

    int N, time[MAXN] = {0};
    int maxtime = -1, maxtimenum;
    short number;
    
    fread(&N, sizeof(int), 1, fp);
    for(int i = 0; i < N; i++)
    {
        fread(&number, sizeof(short), 1, fp);
        time[number + 32768]++;
        if(time[number + 32768] > maxtime)
        {
            maxtime = time[number + 32768];
            maxtimenum = number;
        }
        else if(maxtime == time[number + 32768] && number > maxtimenum)
            maxtimenum = number;
    }
    fclose(fp);
    printf("%d\n%d\n", maxtimenum, maxtime);
    return 0;
}