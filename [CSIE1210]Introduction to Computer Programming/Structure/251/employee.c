#include "employee.h"
#include <string.h>
int finalboss(Employee *emp){
  if(!strcmp((emp -> boss)-> first_name, emp -> first_name) &&\
     !strcmp((emp -> boss)-> last_name, emp -> last_name)) return 0;
  return (1 + finalboss(emp -> boss));
}
int relation(Employee *employee1, Employee *employee2) {
    if(!strcmp((employee1 -> boss)-> first_name, employee2 -> first_name) &&\
       !strcmp((employee1 -> boss)-> last_name, employee2 -> last_name)) return 1;
    if(!strcmp((employee2 -> boss)-> first_name, employee1 -> first_name) &&\
       !strcmp((employee2 -> boss)-> last_name, employee1 -> last_name)) return 2;
    if(!strcmp((employee1 -> boss)-> first_name, (employee2 -> boss)-> first_name) &&\
       !strcmp((employee1 -> boss)-> last_name, (employee2 -> boss)-> last_name)) return 3;
    if(!strcmp((employee1 -> boss)-> first_name, employee1 -> first_name) &&\
       !strcmp((employee1 -> boss)-> last_name, employee1 -> last_name) &&\
       !strcmp((employee2 -> boss)-> first_name, employee2 -> first_name) &&\
       !strcmp((employee2 -> boss)-> last_name, employee2 -> last_name)) return 4;
    int i1 = finalboss(employee1), i2 = finalboss(employee2);
    if(i1 < i2) return relation(employee1, employee2 -> boss);
    return relation(employee1 -> boss, employee2);
}