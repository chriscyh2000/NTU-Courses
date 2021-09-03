#include <stdio.h>
struct point
{
    int x; int y;
};
typedef struct point Point;
int computeWays(Point p, Point destination)
{
    if(p.x == destination.x && p.y == destination.y)
        return 1;
    int countedWays = 0;
    if(p.x + 1 <= destination.x)
    {
        Point tempPoint;
        tempPoint.x = p.x + 1; tempPoint.y = p.y;
        countedWays += computeWays(p, p) *\
                       computeWays(tempPoint, destination);
    }
    if(p.y + 1 <= destination.y)
    {
        Point tempPoint;
        tempPoint.x = p.x; tempPoint.y = p.y + 1;
        countedWays += computeWays(p, p) *\
                       computeWays(tempPoint, destination);
    }
    return countedWays;
}
int main(void)
{
    int row, col;
    scanf("%d%d", &row, &col);
    Point inition, destination;
    inition.x = 1; inition.y = 1;
    destination.x = col; destination.y = row;
    printf("%d\n", computeWays(inition, destination));
}