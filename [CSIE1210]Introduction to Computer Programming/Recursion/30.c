#include <stdio.h>
struct point
{
    int x; int y;
};
typedef struct point Point;
int computeWays(int row, int col, int map[col][row], 
                Point p, Point destination)
{
    if(p.x == destination.x && p.y == destination.y)
        return 1;
    int countedWays = 0;
    if((p.x + 1 <= destination.x) && (map[p.x + 1][p.y] == 1))
    {
        Point tempPoint;
        tempPoint.x = p.x + 1; tempPoint.y = p.y;
        countedWays += computeWays(row, col, map, p, p) *\
                       computeWays(row, col, map, tempPoint, destination);
    }
    if(p.y + 1 <= destination.y && (map[p.x][p.y + 1] == 1))
    {
        Point tempPoint;
        tempPoint.x = p.x; tempPoint.y = p.y + 1;
        countedWays += computeWays(row, col, map, p, p) *\
                       computeWays(row, col, map, tempPoint, destination);
    }
    return countedWays;
}
int main(void)
{
    int row, col;
    scanf("%d%d", &row, &col);
    int map[col][row];
    for(int i = row - 1; i >= 0; i--)
    {
        for(int j = 0; j < col; j++)
            scanf("%d", &(map[j][i]));
    }
    Point inition, destination;
    inition.x = 0; inition.y = 0;
    destination.x = col - 1; destination.y = row - 1;
    printf("%d\n", computeWays(row, col, map, inition, destination));
}