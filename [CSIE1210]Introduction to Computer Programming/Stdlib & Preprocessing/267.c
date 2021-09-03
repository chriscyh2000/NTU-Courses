#include <stdio.h>
#include <stdlib.h>
#define MAXPOINT 100000
#define ORIGIN Point origin;\
origin.x = 0; origin.y = 0

typedef struct point{
    int x;
    int y;
} Point;
int distance(Point p1, Point p2)
{
    return 
        ((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
}
int compare(const void *data1, const void *data2)
{
    Point *ptr1 = (Point *)data1;
    Point *ptr2 = (Point *)data2;
    ORIGIN;
    if(distance(origin, *ptr1) < distance(origin, *ptr2)) return -1;
    if(distance(origin, *ptr1) > distance(origin, *ptr2)) return 1;
    if(distance(origin, *ptr1) == distance(origin, *ptr2))
    {
        if(ptr1->x < ptr2->x) return -1;
        if(ptr1->x > ptr2->x) return 1;
        if(ptr1->y < ptr2->y) return -1;
        if(ptr1->y > ptr2->y) return 1;
    }
    return 0;
}
int totalDistance(Point city[], int i)
{
    ORIGIN;
    int total_distance = distance(origin, city[0]);
    for(int j = 1; j < i; j++)
        total_distance += distance(city[j-1], city[j]);
    return total_distance;
}
int main(void)
{
    Point city[100000];
    int i = 0, j = 0;
    while(scanf("%d%d", &(city[i].x), &(city[i].y)) != EOF) i++;
    qsort(city, i, sizeof(Point), compare);
    printf("%d\n", totalDistance(city, i));
    return 0;
}