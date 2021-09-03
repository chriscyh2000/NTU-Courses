#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "employee.h"
 
void init_storage(Employee **storage, int n)
{
    // allocate memory space to store employee data
    // no need to understand this function now.
 
    (*storage) = (Employee *)malloc(sizeof(Employee) * n);
}
 
void free_storage(Employee **storage)
{
    free(*storage);
    *storage = 0;
}
Employee* find_employee_by_id( Employee *set, int n, int id )
{
    for(int i = 0; i < n; i++)
        if(set[i].id == id)
            return (set + i);
}
Employee* find_employee_by_name( Employee *set, int n, const char *first_name, const char *last_name )
{
    for(int i = 0; i < n; i++)
    {
        if(strcmp(first_name, set[i].first_name) != 0)
            continue;
        if(strcmp(last_name, set[i].last_name) != 0)
            continue;
        return (set + i);
    }
}
Employee* find_root_boss( Employee *set, int n, Employee *employee )
{
    if(employee -> boss_id == employee -> id)
        return employee;
    return find_root_boss(set, n, find_employee_by_id(set, n, employee -> boss_id));
}
int rootBoss(Employee *set, int n, Employee *emp)
{
    if(emp -> boss_id == emp -> id)
        return 0;
    return (1 + rootBoss(set, n, find_employee_by_id(set, n, emp -> boss_id)));
}
int check_relationship(Employee *set, int n, Employee *A, Employee *B)
{
    if(A -> boss_id == B -> id)
        return 1;
    if(A -> id == B -> boss_id)
        return 2;
    if(A -> boss_id == B -> boss_id)
        return 3;
    if((A -> boss_id == A -> id) && (B -> boss_id == B -> id))
        return 4;
    int iA, iB;
    iA = rootBoss(set, n, A); iB = rootBoss(set, n, B);
    if(iA > iB)
        return (check_relationship(set, n, find_employee_by_id(set, n , A -> boss_id), B));
    else
        return (check_relationship(set, n, A, find_employee_by_id(set, n , B -> boss_id)));
}