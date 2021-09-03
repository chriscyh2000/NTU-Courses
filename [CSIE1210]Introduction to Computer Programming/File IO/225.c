#include <stdio.h>
#include <assert.h>
#include <string.h>
struct person {
    unsigned int id;
    char name[32];
};
struct friends {
    unsigned id1;
    unsigned id2;
};
int main(void)
{
    FILE *fin = fopen("friends", "rb");
    assert(fin != NULL);
    int np, nf;

    fread(&np, sizeof(int), 1, fin);
    struct person p[np];
    fread(p, sizeof(struct person), np, fin);

    fread(&nf, sizeof(int), 1, fin);
    struct friends f[nf];
    fread(f, sizeof(struct friends), nf, fin);
    fclose(fin);
    char name[2][32];
    unsigned int id1, id2;

    int i;
    scanf("%s", name[0]);
    while(strcmp(name[0], "end") != 0)
    {
        scanf("%s", name[1]);
        int checkid1 = 0, checkid2 = 0;
        for(i = 0; i < np; i++)
        {
            if(checkid1 == 1 && checkid2 == 1)
                break;
            if(checkid1 == 0)
            {
                if(strcmp(name[0], p[i].name) == 0)
                {
                    id1 = p[i].id;
                    checkid1 = 1;
                    continue;
                }
            }
            if(checkid2 == 0)
            {
                if(strcmp(name[1], p[i].name) == 0)
                {
                    id2 = p[i].id;
                    checkid2 = 1;
                    continue;
                }
            }
        }
        for(i = 0; i < nf; i++)
        {
            if((f[i].id1 == id1 && f[i].id2 == id2) ||\
               (f[i].id2 == id1 && f[i].id1 == id2))
            {
                printf("yes\n");
                break;
            }
        }
        if(i == nf) printf("no\n");
        scanf("%s", name[0]);
    }
    return 0;
}