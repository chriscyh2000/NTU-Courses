#include <stdio.h>
#include <string.h>
struct employee {
  int id;
  char first_name[32];
  char last_name[32];
  int boss_id;
};
typedef struct employee Employee;
int finalboss(int n, int boss_index[], 
              Employee files[], Employee emp, int index)
{
  if(emp.boss_id == emp.id) return 0;
  int i = boss_index[index];
  return (1 + finalboss(n, boss_index, files, files[i], i));
}
char *checkRelation(int n, int boss_index[], 
                    Employee files[], Employee emp1, Employee emp2,
                    int index1, int index2)
{
  if(emp1.boss_id == emp2.id)
    return "subordinate";
  if(emp1.id == emp2.boss_id)
    return "supervisor";
  if(emp1.boss_id == emp2.boss_id)
    return "colleague";
  if((emp1.boss_id == emp1.id) && (emp2.boss_id == emp2.id))
    return "unrelated";
  int i1 = finalboss(n, boss_index, files, emp1, index1);
  int i2 = finalboss(n, boss_index, files, emp2, index2);
  if(i1 > i2)
    return checkRelation(n, boss_index, files, files[boss_index[index1]], 
                         emp2, boss_index[index1], index2);
  else
    return checkRelation(n, boss_index, files, emp1, 
                         files[boss_index[index2]], index1, boss_index[index2]);
}
int main(void)
{
    int n;
    scanf("%d", &n);
    Employee files[n];
    int boss_index[n];
    for(int i = 0; i < n; i++)
    {
      scanf("%d%s%s%d", &(files[i].id), files[i].first_name,
                        files[i].last_name, &files[i].boss_id);
      if(files[i].id == files[i].boss_id)
        boss_index[i] = i;
    }
    for(int i = 0; i < n; i++)
    {
      for(int j = 0; j < n; j++)
      {
        if(i == j) continue;
        if(files[i].boss_id == files[j].id)
        {
          boss_index[i] = j;
          break;
        }
      }
    }
    int inquireTime;
    scanf("%d", &inquireTime);
    char relationship[inquireTime][15];
    for(int i = 0; i < inquireTime; i++)
    {
      char firstname[2][32], lastname[2][32];
      scanf("%s%s%s%s", firstname[0], lastname[0],
                        firstname[1], lastname[1]);
 
      int index1 = -1, index2 = -1;
      for(int j = 0; j < n; j++)
      {
        if((strcmp(firstname[0], files[j].first_name) != 0) &&\
           (strcmp(firstname[1], files[j].first_name) != 0))
          continue;
        if(strcmp(firstname[0], files[j].first_name) == 0)
        {
          if(strcmp(lastname[0], files[j].last_name) == 0) 
          {
            index1 = j;
            continue;
          }
        }
        if(strcmp(lastname[1], files[j].last_name) == 0)
        {
          if(strcmp(lastname[1], files[j].last_name) == 0) 
          {
            index2 = j;
            continue;
          }
        }
        if(index1 != -1 && index2 != -1) break; 
      }
      strcpy(relationship[i], checkRelation(n, boss_index, files, 
                                        files[index1], files[index2], 
                                        index1, index2));
    }
    for(int i = 0; i < inquireTime; i++)
      printf("%s\n", relationship[i]);
}