#include <stdio.h>
int main(){
    int year, month, firstday;
    scanf("%d%d%d", &year, &month, &firstday);
    int totalday;
    switch(month){
    case 1: case 3: case 5: case 7: case 8: case 10: case 12:{
        totalday = 31;
        break;
    }
    case 4: case 6: case 9: case 11:{
        totalday = 30;
        break;
    }
    case 2:{
        if((year % 400 == 0) || ((year % 4 == 0) && (year % 100 != 0)))
            totalday = 29;
        else
            totalday = 28;
        break;
    }
    default:
        break;
    }
    if((month >= 1) && (month <= 12) && (firstday <= 6) && (firstday >= 0))
        printf(" Su Mo Tu We Th Fr Sa\n=====================\n");
    int i, j;
    int date, day;
    day = firstday;
    if((month >= 1) && (month <= 12) && (firstday <= 6) && (firstday >= 0)){
        for(date = 1; date <= totalday; ++date){   
            if(day == 7){
                printf("\n");
                day = 0 ;
            }
            if(date == 1){
                switch(firstday){
                    case 0:
                        printf("");
                        break;
                    case 1:
                        printf("   ");
                        break;
                    case 2:
                        printf("      ");
                        break;
                    case 3:
                        printf("         ");
                        break;
                    case 4:
                        printf("            ");
                        break;
                    case 5:
                        printf("               ");
                        break;
                    case 6:
                        printf("                  ");
                        break;
                    default:
                        break;
                }
            }
            if ( date < 10 ) printf("  %d", date);
            else printf(" %d", date);
            ++day;
        }
    }
    else printf("invalid\n");
    if(( month >= 1) && (month <= 12) && (firstday <= 6) && (firstday >= 0))
        printf("\n=====================\n");
}