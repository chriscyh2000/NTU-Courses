/* input file內的指令： A 0 10 10--> allocatememory(&myMemory, 10, 10) */
/* P 0--> printMemory(&myMemory) */
/* I 0 100--> initMemory(&myMemory, 100) */
/* F 0 10 10 freeMemory(&myMemory, 10, 10) */
/* 依此類推，看input file的指令是什麼就改成什麼就好！ */
#include "memory.h"
int main() {
        Memory myMemory;
        initMemory(&myMemory, 100);
        allocateMemory(&myMemory, 10, 10);
        printMemory(&myMemory);
        allocateMemory(&myMemory, 20, 10);
        printMemory(&myMemory);
        allocateMemory(&myMemory, 0, 10);
        printMemory(&myMemory);
        allocateMemory(&myMemory, 30, 70);
        printMemory(&myMemory);
        printMemory(&myMemory);
        freeMemory(&myMemory, 40, 20);
        printMemory(&myMemory);
        freeMemory(&myMemory, 80, 10);
        printMemory(&myMemory);
        freeMemory(&myMemory, 10, 10);
        printMemory(&myMemory);
        freeMemory(&myMemory, 0, 10);
        printMemory(&myMemory);
        freeMemory(&myMemory, 20, 20);
        printMemory(&myMemory);
        freeMemory(&myMemory, 90, 10);
        printMemory(&myMemory);
        freeMemory(&myMemory, 60, 20);
        printMemory(&myMemory);
        return 0;
}