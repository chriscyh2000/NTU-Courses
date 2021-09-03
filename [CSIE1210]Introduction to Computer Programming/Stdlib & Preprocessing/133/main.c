#include <stdio.h>
#include <string.h>
#include <stdlib.h>
struct string_count {
        char seen[80];
        int count;
};
int compare_and_add(char *string, struct string_count strings[]) 
{
    int i = 0;
    while(strings[i].count != 0 && strcmp(strings[i].seen, string) != 0)
        i++;
    if(strings[i].count == 0)
    {
        strcpy(strings[i].seen, string);
        strings[i].count++;
        return 0;
    }
    strings[i].count++;
    return 1;
}
int compare(const void *data1, const void *data2)
{
    struct string_count *ptr1 = (struct string_count *)data1;
    struct string_count *ptr2 = (struct string_count *)data2;
    if(ptr1->count < ptr2->count) return -1;
    if(ptr1->count > ptr2->count) return 1;
    if(ptr1->count == ptr2->count)
    {
        if(strcmp(ptr1->seen, ptr2->seen) < 0) return -1;
        if(strcmp(ptr1->seen, ptr2->seen) > 0) return 1;
    }
    return 0;
}
void sort(struct string_count strings[]) 
{
    int i = 0;
    while(strings[i].count > 0) i++;
    qsort(strings, i, sizeof(struct string_count), compare);
    return;
}
void print(struct string_count strings[]) 
{
    int i = 0;
    while(strings[i].count > 0)
    {
        printf("%d %s\n", strings[i].count, strings[i].seen);
        i++;
    }
    return;
}
/* your uploaded code will be up there ! */
int main() {
        struct string_count strings[20];
        int i;
        for( i=0 ; i<20 ; i++ )
                strings[i].count = 0;
        compare_and_add( "This", strings );
        compare_and_add( "is", strings );
        compare_and_add( "an", strings );
        compare_and_add( "apple,", strings );
        compare_and_add( "and", strings );
        compare_and_add( "this", strings );
        compare_and_add( "is", strings );
        compare_and_add( "a", strings );
        compare_and_add( "book.", strings );
        sort( strings );
        print( strings );
        return 0;
}