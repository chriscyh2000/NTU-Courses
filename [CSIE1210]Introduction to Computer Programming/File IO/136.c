#include <stdio.h>
#include <assert.h>
typedef struct {
    char name[20];
    int id;
    char phone[10];
    float grade[4];
    int birth_year;
    int birth_month;
    int birth_day;
} Student;
int main(void)
{
    char infilename[81];
    char outfilename[81];
    scanf("%s%s", infilename, outfilename);
    FILE *fin = fopen(infilename, "rb");
    FILE *fout = fopen(outfilename, "w");
    assert(fin != NULL && fout != NULL);
    fprintf(fout, "<table border=\"1\">\n");
    fprintf(fout, "<tbody>\n");
    Student stu;
    while(fread(&stu, sizeof(Student), 1, fin) != 0)
    {
        fprintf(fout, "<tr>\n");
        fprintf(fout, "<td>%s</td>\n", stu.name);
        fprintf(fout, "<td>%d</td>\n", stu.id);
        fprintf(fout, "<td>%s</td>\n", stu.phone);
        fprintf(fout, "<td>%f, %f, %f, %f</td>\n", stu.grade[0], stu.grade[1], 
                                                   stu.grade[2], stu.grade[3]);
        fprintf(fout, "<td>%d, %d, %d</td>\n", stu.birth_year, stu.birth_month, stu.birth_day);
        fprintf(fout, "</tr>\n");                                   
    }
    fprintf(fout, "</tbody>\n"); 
    fprintf(fout, "</table>\n"); 
    fclose(fin);
	fclose(fout);
	return 0;
}