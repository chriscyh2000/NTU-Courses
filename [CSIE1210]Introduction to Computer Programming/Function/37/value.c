int value(int type, int width, int height, int length)
{
    int num, smallvolumn, unitprice;
    switch(type)
    {
        case 79:
            unitprice = 30; break;
        case 47:
            unitprice = 10; break;
        case 29:
            unitprice = 4; break;
        case 82:
            unitprice = 5; break;
        case 26:
            unitprice = 3; break;
        case 22:
            unitprice = 9; break;
        default:
            return -1;
    }
    if((width <= 0) || (height <= 0) || (length <= 0))
        return -2;
    int sidelength[3];
    sidelength[0] = width; sidelength[1] = height; sidelength[2] = length;
    for (int i = 2; i > 0; i--)
    {
        for (int j = 0; j < i; j++)
        {
            if (sidelength[j] > sidelength[j+1])
            {
                int temp;
                temp = sidelength[j];
                sidelength[j] = sidelength[j+1];
                sidelength[j+1] = temp;
            }
        }
    }
    int i, j, gcd;
    i = sidelength[2]; j = sidelength[1];
    while (i % j != 0)
    {
        int temp;
        temp = i % j;
        i = j;
        j = temp;
    }
    gcd = j;
    i = gcd; j = sidelength[0];
    if (gcd < sidelength[0]) 
    {
        i = sidelength[0]; j = gcd;
    }
    while (i % j != 0)
    {
        int temp;
        temp = i % j;
        i = j;
        j = temp;
    }
    gcd = j;
    smallvolumn = gcd * gcd * gcd;
    num = width * length * height / smallvolumn;
    int answer;
    answer = smallvolumn * smallvolumn * unitprice * num;
    return answer;
}