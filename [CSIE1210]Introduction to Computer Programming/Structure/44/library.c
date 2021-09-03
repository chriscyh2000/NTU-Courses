#include "book.h"
#include "date.h"
int freedays[4] = {90, 10, 100, 5};
int countDays(struct Date b, struct Date r){
    int sum = 1;
    while(b.year < r.year)
    {
        if(((b.year % 4 == 0 && b.year % 100 != 0) || b.year % 400 == 0) &&\
            (b.month <= 2)) sum += 366;
        else if((((b.year + 1) % 4 == 0 && (b.year + 1) % 100 != 0) || (b.year + 1) % 400 == 0) &&\
                 (b.month > 2)) sum += 366;
        else sum += 365;
        b.year++;
    }
    while(b.month != r.month)
    {
        if(b.month < r.month)
        {
            switch(b.month)
            {
                case 1: case 3: case 5: case 7: case 8: case 10: case 12:
                    sum += 31; 
                    break;
                case 4: case 6: case 9: case 11: 
                    sum += 30;
                    break;
                default:
                {
                    if((b.year % 4 == 0 && b.year % 100 != 0) || (b.year % 400 == 0))
                        sum += 29;
                    else sum += 28;
                    break;
                }
            }
            b.month++;
        }
        else
        {
            switch(b.month - 1)
            {
                case 1: case 3: case 5: case 7: case 8: case 10: case 12:
                    sum -= 31; 
                    break;
                case 4: case 6: case 9: case 11: 
                    sum -= 30;
                    break;
                default:
                {
                    if((b.year % 4 == 0 && b.year % 100 != 0) || (b.year % 400 == 0))
                        sum -= 29;
                    else sum -= 28;
                    break;
                }
            }
            b.month--;
        }
    }
    if(b.day != r.day)
        sum += (r.day - b.day);
    return sum;
}
unsigned int library_fine(struct Book book, struct Date date_borrowed, struct Date date_returned){
    int place1 = -1, place2 = -1;
    int i = 0, factor, counteddays;
    while(place2 == -1)
    {
        if((book.importance & ((unsigned int) 1 << i)) != 0)
        {
            if(place1 == -1) place1 = i;
            else place2 = i;
        }
        i++; 
    }
    factor = place2 - place1 - 1;
    counteddays = countDays(date_borrowed, date_returned);
    if(counteddays > freedays[book.type])
        return((counteddays - freedays[book.type]) * factor);
    return 0;
}