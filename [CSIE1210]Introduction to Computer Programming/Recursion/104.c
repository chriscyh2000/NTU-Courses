#include <stdio.h>
#include <stdbool.h>
#include <string.h>
 
#define KINDS 3
bool allMaterialsUsed(int need[KINDS][KINDS], int materials[KINDS])
{
    if(materials[0] == 0 &&\
       materials[1] == 0 &&\
       materials[2] == 0)
        return true;
    if(materials[0] - need[0][0] >= 0 &&\
       materials[1] - need[0][1] >= 0 &&\
       materials[2] - need[0][2] >= 0)
    {
        materials[0] -= need[0][0];
        materials[1] -= need[0][1];
        materials[2] -= need[0][2];
        if(allMaterialsUsed(need, materials))
            return true;
        materials[0] += need[0][0];
        materials[1] += need[0][1];
        materials[2] += need[0][2];
    }
    if(materials[0] - need[1][0] >= 0 &&\
       materials[1] - need[1][1] >= 0 &&\
       materials[2] - need[1][2] >= 0)
    {
        materials[0] -= need[1][0];
        materials[1] -= need[1][1];
        materials[2] -= need[1][2];
        if(allMaterialsUsed(need, materials))
            return true;
        materials[0] += need[1][0];
        materials[1] += need[1][1];
        materials[2] += need[1][2];
    }
    if(materials[0] - need[2][0] >= 0 &&\
       materials[1] - need[2][1] >= 0 &&\
       materials[2] - need[2][2] >= 0)
    {
        materials[0] -= need[2][0];
        materials[1] -= need[2][1];
        materials[2] -= need[2][2];
        if(allMaterialsUsed(need, materials))
            return true;
        materials[0] += need[2][0];
        materials[1] += need[2][1];
        materials[2] += need[2][2];
    }   
    return false;
}
void read(int testTimes, int need[KINDS][KINDS])
{
    if(testTimes <= 0)
        return;
    int materials[KINDS];
    scanf("%d%d%d", &materials[0], &materials[1], &materials[2]);
    if(allMaterialsUsed(need, materials)) printf("yes\n");
    else printf("no\n");
    testTimes--;
    read(testTimes, need);
}
int main(void)
{
    int need[KINDS][KINDS], materials[KINDS];
    int i, j;
    scanf("%d%d%d", &need[0][0], &need[0][1], &need[0][2]);
    scanf("%d%d%d", &need[1][0], &need[1][1], &need[1][2]);
    scanf("%d%d%d", &need[2][0], &need[2][1], &need[2][2]);
    int testTimes;
    scanf("%d", &testTimes);
    read(testTimes, need);
    return 0;
}