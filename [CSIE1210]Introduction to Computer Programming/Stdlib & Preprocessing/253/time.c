#include "time.h"
#include <stdio.h>
#include <stdlib.h>
void initTime(Time *time)
{
    time->hour = time->minute = time->second = 0;
    return;
}
void setTime(Time *time, int hour, int minute, int second)
{
	time->hour = hour;
	time->minute = minute;
	time->second = second;
	return;
}
void addTime(Time *time, int hour, int minute, int second)
{
    time->second += second;
    time->minute += (time->second / 60);
    time->second %= 60;
    time->minute += minute;
    time->hour += (time->minute / 60);
    time->minute %= 60;
    time->hour += hour;
    time->hour %= 24;
    return;
}
void printTime(Time *time)
{
    #ifdef H24
    printf("%02d:%02d:%02d\n", time->hour, time->minute, time->second);
    #endif
    #ifndef H24
    if(time->hour < 12)
    {
        if(time->hour == 0) printf("12am:%02d:%02d\n", time->minute, time->second);
        else printf("%02dam:%02d:%02d\n", time->hour, time->minute, time->second);
    }
    else
    {
        if(time->hour == 12) printf("12pm:%02d:%02d\n", time->minute, time->second);
        else printf("%02dpm:%02d:%02d\n", time->hour - 12, time->minute, time->second);
    }
    #endif
    return;
}