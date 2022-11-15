#include "kernel/types.h"
#include "user/user.h"
#include "user/threads.h"

#define NULL 0


int startTime = 0;
int nowTime = -1;
int main_id = 0;
int flag = 0;
int thrdstop_test_time = 0;
int tmpTime;
char str1[] = "HEY", str2[] = "Meet you in YT.";
char str3[] = "RED SCHOOL";
void *argAddrRecord = NULL;


void gg(int *k) 
{
  int x = 0;
  for (int i = 1; i <= 1000; ++i) {
    x = x + i;
  }
  if (x != (1 + 1000) * 500) {
    printf("FAILED!!!!! %d\n", x);
    exit(0);
  }
  int old = *k + 1;
  *k = x;
  *k = old;
}



void thrdstop_test1(void *addr)
{
    thrdstop_test_time = uptime();
    printf("thrdstop test1 at %d\n", thrdstop_test_time-startTime);
    printf("arg = %s\n", (char *)addr);
    flag += 1;
    argAddrRecord = addr;
    thrdresume(main_id);
}

void thrdstop_test2(void *addr)
{
    thrdstop_test_time = uptime();
    printf("eac8a36c12c94b93e22928c1174372d15dca00a5724481788a7a0a901e065338 1\narg = %p\n", addr);    
    flag += 1;
    thrdresume(main_id);
}



int main(int argc, char **argv)
{
    printf("str1=%p\nstr2=%p\nstr3=%p\n", &str1[0], &str2[0], &str3[0]);

    startTime = uptime();
    main_id = thrdstop( 5, -1, thrdstop_test2, (void *)0xbbbb87);
    int i = 0, j = 0;
    int tmp_time = 0;
    while (1)
    {   
        gg(&j);
        i ++;
        if(flag == 1)
        {
            if( thrdstop_test_time-startTime > 6 || thrdstop_test_time-startTime < 4 )
            {
                printf("FAILED. thrdstop counts down incorrectly\n");
                exit(0);
            }
            thrdstop( 10, main_id, thrdstop_test1, &str1[0]);
            tmp_time = uptime();
            printf("tmp_time=%d\n", tmp_time);
            flag += 1;
        }
        else if( flag == 2 && uptime() - tmp_time >= 2 )
        {
            int time_consumed = cancelthrdstop( -1, 0 );
            printf("time_consumed = %d, and it should be 2\n", time_consumed);
            thrdstop( 2, main_id, thrdstop_test1, &str2[0]);
            flag += 1;
        }
        else if( flag == 4 )
        {
            if( thrdstop_test_time-startTime > 10 || thrdstop_test_time-startTime < 8 )
            {
                printf("FAILED. thrdstop counts down incorrectly\n");
                exit(0);
            }
            else if ( argAddrRecord != &str2[0] ){
                printf("FAILED. the argument of the handler is incorrect.\n");
                exit(0);
            }
            thrdstop( 10, main_id, thrdstop_test1, &str3[0]);
            flag += 1;
        }
        else if( flag == 5 )
        {
            // want to call uptime() many times.

            tmpTime = uptime();
            if( tmpTime != nowTime && flag == 5 )
            {
                printf("ttt %d ", tmpTime-startTime);
                nowTime = tmpTime;
            }
            
        }
        else if( flag == 6 )
        {
            if( thrdstop_test_time-startTime > 20 || thrdstop_test_time-startTime < 18 )
            {
                printf("\nFAILED, thrdstop counts down incorrectly\n");
                exit(0);
            }
            else if ( argAddrRecord != &str3[0] ){
                printf("\n FAILED. the argument of the handler is incorrect.\n");
                exit(0);
            }
            printf("\n");

            thrdstop( 1, main_id, thrdstop_test1, &str1[0]);
            flag += 1;
        }
        else if( flag == 8 ){
            if( thrdstop_test_time-startTime > 21 || thrdstop_test_time-startTime < 19 )
            {
                printf("FAILED, thrdstop counts down incorrectly\n");
                exit(0);
            }
            else if ( argAddrRecord != &str1[0] ){
                printf("FAILED. the argument of the handler is incorrect.\n");
                exit(0);
            }
            else{
                while( tmpTime - startTime <= 25 ){
                    tmpTime = uptime();
                    if( tmpTime != nowTime )
                    {
                        printf("%d ", tmpTime);
                        nowTime = tmpTime;
                    }
                }
                if( i != j) printf("\n%d %d FAILED, i should equal to j\n", i, j);
                else if( flag != 8 ) printf("\nFAILED, flag should equal to 8\n");
                else printf("\neac8a36c12c94b93e22928c1174372d15dca00a5724481788a7a0a901e065338 2\n");
            }
            break;
        }
    }

    exit(0);
}
