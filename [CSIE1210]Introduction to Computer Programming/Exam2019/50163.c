#include <stdio.h>
#include <stdbool.h>
#define MAXCARNUM 1000
typedef struct seat{
    int car;
    int row;
    int col;
}Seat;
Seat queue[50001];
int front = 0;
int rear = 0;
int check = false;
int main(void){
    int carNum, request;
    int curCar = 1, curRow = 1, curCol = 1;
    scanf("%d", &carNum);
    int i = 0;
    while(scanf("%d", &request) != EOF){
        ++i;
        if(request == 1){
            if(check){
                printf("%d %d %d\n", queue[front].car, queue[front].row, queue[front].col);
                ++front;
            }
            else{
                if(front != rear){
                    printf("%d %d %d\n", queue[front].car, queue[front].row, queue[front].col);
                    ++front;
                }
                else {
                    printf("%d %d %d\n", curCar, curRow, curCol);
                    ++curCol;
                    if(curCol == 6){
                        curCol = 1;
                        ++curRow;
                    }
                    if(curRow == 21){
                        curRow = 1;
                        ++curCar;
                        if(curCar > carNum) check = true;
                    }
                }
            }
        }
        else{
            // if(i == 6807) printf("curCol = %d\n", curCol);
            if(check){
                printf("%d %d %d ", queue[front].car, queue[front].row, queue[front].col);
                ++front;
                printf("%d %d %d\n", queue[front].car, queue[front].row, queue[front].col);
                ++front;
            }
            else if(curCar == carNum && curCol == 5 && curRow == 20){
                printf("%d %d %d ", queue[front].car, queue[front].row, queue[front].col);
                ++front;
                if(front == rear)
                    printf("%d 20 5\n", curCar);
                else{
                    queue[rear].car = curCar;
                    queue[rear].row = curRow;
                    queue[rear++].col = curCol;
                    printf("%d %d %d\n", queue[front].car, queue[front].row, queue[front].col);
                    ++front;
                }
                check = true;
            }
            else{
                if(curCol < 3){
                    printf("%d %d %d ", curCar, curRow, curCol);
                    ++curCol;
                    printf("%d %d %d\n", curCar, curRow, curCol);
                    ++curCol;
                }
                else if(curCol == 4){
                    printf("%d %d %d ", curCar, curRow, curCol);
                    ++curCol;
                    printf("%d %d %d\n", curCar, curRow, curCol);
                    ++curRow;
                    curCol = 1;
                    if(curRow == 21){
                        curRow = 1;
                        ++curCar;
                        if(curCar > carNum) check = true;
                    }
                }
                else if(curCol == 3){
                    queue[rear].car = curCar;
                    queue[rear].row = curRow;
                    queue[rear++].col = curCol;
                    ++curCol;
                    printf("%d %d %d ", curCar, curRow, curCol);
                    ++curCol;
                    printf("%d %d %d\n", curCar, curRow, curCol);
                    curCol = 1;
                    ++curRow;
                    if(curRow == 21){
                        curRow = 1;
                        ++curCar;
                        if(curCar > carNum) check = true;
                    }
                }
                else{
                    queue[rear].car = curCar;
                    queue[rear].row = curRow;
                    queue[rear++].col = curCol;
                    ++curRow;
                    curCol = 1;
                    if(curRow == 21){
                        curRow = 1;
                        ++curCar;
                        if(curCar > carNum) check = true;
                    }
                    printf("%d %d %d ", curCar, curRow, curCol);
                    ++curCol;
                    printf("%d %d %d\n", curCar, curRow, curCol);
                    ++curCol;
                }
            }
        }
    }
}