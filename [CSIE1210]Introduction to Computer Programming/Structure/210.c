#include <stdio.h>
#include <string.h>
struct data{
    char lastname[80];
    char firstname[80];
    char ID[80];
    int salary;
    int age;
};
typedef struct data Data;
int main(void){
    int n;
    scanf("%d", &n);
    Data D[n];
    for(int i = 0; i < n; i++)
        scanf("%s%s%s%d%d", (D[i].lastname), (D[i].firstname),\
                            (D[i].ID), &(D[i].salary), &(D[i].age));
    int inquireTime;
    scanf("%d", &inquireTime);
    for(int i = 0; i < inquireTime; i++){
        char select[8];
        scanf("%s", select);
        char field[9][15];
        int j = 0;
        scanf("%s", field[j]);
        while(field[j][0] != 'w'){
            j++;
            scanf("%s", field[j]);
        }
        char condition1[15], condition2[4];
        scanf("%s%s", condition1, condition2);
        if(condition1[0] == 'l'){
            char condition3[80];
            scanf("%s", condition3);
            for(int k = 0; k < n; k++){
                if(strcmp(condition2, "==") == 0){
                    if(strcmp(D[k].lastname, condition3) != 0)
                        continue;
                }
                else{
                    if(strcmp(D[k].lastname, condition3) == 0)
                        continue;
                }
                for(int a = 0; a < j; a++){
                    if(field[a][0] == 'l'){
                        if(a != j - 1) printf("%s ", D[k].lastname);
                        else printf("%s\n", D[k].lastname);
                        continue;
                    }
                    if(field[a][0] == 'f'){
                        if(a != j - 1) printf("%s ", D[k].firstname);
                        else printf("%s\n", D[k].firstname);
                        continue;
                    }
                    if(field[a][0] == 'I'){
                        if(a != j - 1) printf("%s ", D[k].ID);
                        else printf("%s\n", D[k].ID);
                        continue;
                    }
                    if(field[a][0] == 's'){
                        if(a != j - 1) printf("%d ", D[k].salary);
                        else printf("%d\n", D[k].salary);
                        continue;
                    }
                    if(field[a][0] == 'a'){
                        if(a != j - 1) printf("%d ", D[k].age);
                        else printf("%d\n", D[k].age);
                        continue;
                    }
                }
            }
            continue;
        }
        if(condition1[0] == 'f'){
            char condition3[80];
            scanf("%s", condition3);
            for(int k = 0; k < n; k++){
                if(strcmp(condition2, "==") == 0){
                    if(strcmp(D[k].firstname, condition3) != 0)
                        continue;
                }
                else{
                    if(strcmp(D[k].firstname, condition3) == 0)
                        continue;
                }
                for(int a = 0; a < j; a++){
                    if(field[a][0] == 'l'){
                        if(a != j - 1) printf("%s ", D[k].lastname);
                        else printf("%s\n", D[k].lastname);
                        continue;
                    }
                    if(field[a][0] == 'f'){
                        if(a != j - 1) printf("%s ", D[k].firstname);
                        else printf("%s\n", D[k].firstname);
                        continue;
                    }
                    if(field[a][0] == 'I'){
                        if(a != j - 1) printf("%s ", D[k].ID);
                        else printf("%s\n", D[k].ID);
                        continue;
                    }
                    if(field[a][0] == 's'){
                        if(a != j - 1) printf("%d ", D[k].salary);
                        else printf("%d\n", D[k].salary);
                        continue;
                    }
                    if(field[a][0] == 'a'){
                        if(a != j - 1) printf("%d ", D[k].age);
                        else printf("%d\n", D[k].age);
                        continue;
                    }
                }
            }
            continue;
        }
        if(condition1[0] == 'I'){
            char condition3[80];
            scanf("%s", condition3);
            for(int k = 0; k < n; k++){
                if(strcmp(condition2, "==") == 0){
                    if(strcmp(D[k].ID, condition3) != 0)
                        continue;
                }
                else{
                    if(strcmp(D[k].ID, condition3) == 0)
                        continue;
                }
                for(int a = 0; a < j; a++){
                    if(field[a][0] == 'l'){
                        if(a != j - 1) printf("%s ", D[k].lastname);
                        else printf("%s\n", D[k].lastname);
                        continue;
                    }
                    if(field[a][0] == 'f'){
                        if(a != j - 1) printf("%s ", D[k].firstname);
                        else printf("%s\n", D[k].firstname);
                        continue;
                    }
                    if(field[a][0] == 'I'){
                        if(a != j - 1) printf("%s ", D[k].ID);
                        else printf("%s\n", D[k].ID);
                        continue;
                    }
                    if(field[a][0] == 's'){
                        if(a != j - 1) printf("%d ", D[k].salary);
                        else printf("%d\n", D[k].salary);
                        continue;
                    }
                    if(field[a][0] == 'a'){
                        if(a != j - 1) printf("%d ", D[k].age);
                        else printf("%d\n", D[k].age);
                        continue;
                    }
                }
            }
            continue;
        }
        if(condition1[0] == 's'){
            int condition3;
            scanf("%d", &condition3);
            int b = 0;
            for(int k = 0; k < n; k++){
                if(condition2[0] == '='){
                    if(D[k].salary != condition3) continue;
                }
                else if(condition2[0] == '>'){
                    if(D[k].salary <= condition3) continue;
                }
                else if(condition2[0] == '<'){
                    if(D[k].salary >= condition3) continue;
                }
                for(int a = 0; a < j; a++){
                    if(field[a][0] == 'l'){
                        if(a != j - 1) printf("%s ", D[k].lastname);
                        else printf("%s\n", D[k].lastname);
                        continue;
                    }
                    if(field[a][0] == 'f'){
                        if(a != j - 1) printf("%s ", D[k].firstname);
                        else printf("%s\n", D[k].firstname);
                        continue;
                    }
                    if(field[a][0] == 'I'){
                        if(a != j - 1) printf("%s ", D[k].ID);
                        else printf("%s\n", D[k].ID);
                        continue;
                    }
                    if(field[a][0] == 's'){
                        if(a != j - 1) printf("%d ", D[k].salary);
                        else printf("%d\n", D[k].salary);
                        continue;
                    }
                    if(field[a][0] == 'a'){
                        if(a != j - 1) printf("%d ", D[k].age);
                        else printf("%d\n", D[k].age);
                        continue;
                    }
                }
            }
        }
        if(strcmp(condition1, "age") == 0){
            int condition3;
            scanf("%d", &condition3);
            for(int k = 0; k < n; k++){
                if(condition2[0] == '='){
                    if(D[k].age != condition3) continue;
                }
                else if(condition2[0] == '>'){
                    if(D[k].age <= condition3) continue;
                }
                else if(condition2[0] == '<'){
                    if(D[k].age >= condition3) continue;
                }
                for(int a = 0; a < j; a++){
                    if(field[a][0] == 'l'){
                        if(a != j - 1) printf("%s ", D[k].lastname);
                        else printf("%s\n", D[k].lastname);
                        continue;
                    }
                    if(field[a][0] == 'f'){
                        if(a != j - 1) printf("%s ", D[k].firstname);
                        else printf("%s\n", D[k].firstname);
                        continue;
                    }
                    if(field[a][0] == 'I'){
                        if(a != j - 1) printf("%s ", D[k].ID);
                        else printf("%s\n", D[k].ID);
                        continue;
                    }
                    if(field[a][0] == 's'){
                        if(a != j - 1) printf("%d ", D[k].salary);
                        else printf("%d\n", D[k].salary);
                        continue;
                    }
                    if(field[a][0] == 'a'){
                        if(a != j - 1) printf("%d ", D[k].age);
                        else printf("%d\n", D[k].age);
                        continue;
                    }
                }
            }
            continue;
        }
    }
    return 0;
}