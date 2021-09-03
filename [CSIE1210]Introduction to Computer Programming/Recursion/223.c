#include <stdio.h> 
struct point
{
    int x;
    int y;
};
typedef struct point Point;
void coverTiles(int size, int type, const Point p)
{
    if(size == 2)
    {
        printf("%d %d %d\n", type, p.x, p.y);
        return;
    }
    int displacement = size / 4;
    const int smallerSize = size / 2;
    switch(type)
    {
        case 1:
        {
            coverTiles(smallerSize, 1, p);
            Point tempPoint;
            tempPoint.x = p.x - displacement;
            tempPoint.y = p.y - displacement;
            coverTiles(smallerSize, 1, tempPoint);
            tempPoint.x = p.x - displacement;
            tempPoint.y = p.y + displacement;
            coverTiles(smallerSize, 4, tempPoint);
            tempPoint.x = p.x + displacement;
            tempPoint.y = p.y - displacement;
            coverTiles(smallerSize, 2, tempPoint);
            break;
        }
        case 2:
        {
            coverTiles(smallerSize, 2, p);
            Point tempPoint;
            tempPoint.x = p.x + displacement;
            tempPoint.y = p.y - displacement;
            coverTiles(smallerSize, 2, tempPoint);
            tempPoint.x = p.x + displacement;
            tempPoint.y = p.y + displacement;
            coverTiles(smallerSize, 3, tempPoint);
            tempPoint.x = p.x - displacement;
            tempPoint.y = p.y - displacement;
            coverTiles(smallerSize, 1, tempPoint);
            break;
        }
        case 3:
        {
            coverTiles(smallerSize, 3, p);
            Point tempPoint;
            tempPoint.x = p.x + displacement;
            tempPoint.y = p.y + displacement;
            coverTiles(smallerSize, 3, tempPoint);
            tempPoint.x = p.x - displacement;
            tempPoint.y = p.y + displacement;
            coverTiles(smallerSize, 4, tempPoint);
            tempPoint.x = p.x + displacement;
            tempPoint.y = p.y - displacement;
            coverTiles(smallerSize, 2, tempPoint);
            break;
        }
        case 4:
        {
            coverTiles(smallerSize, 4, p);
            Point tempPoint;
            tempPoint.x = p.x - displacement;
            tempPoint.y = p.y + displacement;
            coverTiles(smallerSize, 4, tempPoint);
            tempPoint.x = p.x + displacement;
            tempPoint.y = p.y + displacement;
            coverTiles(smallerSize, 3, tempPoint);
            tempPoint.x = p.x - displacement;
            tempPoint.y = p.y - displacement;
            coverTiles(smallerSize, 1, tempPoint);
            break;
        }
        default: break;
    }
    return;
}
int main(void)
{
    int l, m;
    scanf("%d%d", &l, &m);
    Point init;
    init.x = init.y = l - m;
    while(l >= m * 2)
    {
        coverTiles(m * 2, 1, init);
        m *= 2;
        init.x = init.y = l - m;
    }
    return 0;
}