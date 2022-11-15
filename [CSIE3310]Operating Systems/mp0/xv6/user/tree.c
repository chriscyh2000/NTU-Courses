#include "kernel/types.h"
#include "kernel/fs.h"
#include "kernel/stat.h"
#include "user/user.h"

int curLevel = 0;
int table[5] = {0};
int dir_num = 0, file_num = 0;


void tree(char *path){
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if((fd = open(path, 0)) < 0){
        fprintf(2, "%s [error opening dir]\n", path);
        return;
    }

    if(fstat(fd, &st) < 0){
        fprintf(2, "%s [error opening dir]\n", path);
        close(fd);
        return;
    }

    if(curLevel == 0){
        if(st.type == T_FILE){
            fprintf(2, "%s [error opening dir]\n", path);
            close(fd);
            return;
        }
        else{
            printf("%s\n", path);
        }
    }

    switch(st.type){
        case T_FILE: {
            ++file_num;
            break;
        }
        case T_DIR: {
            ++dir_num;
            strcpy(buf, path);
            p = buf + strlen(buf);
            *p++ = '/';

            int count = 0;
            while(read(fd, &de, sizeof(de)) == sizeof(de)){
                if(de.inum == 0) continue;
                memmove(p, de.name, DIRSIZ);
                p[DIRSIZ] = 0;
                if(strcmp(".", p) == 0 || strcmp("..", p) == 0) continue;
                ++count;
            }

            strcpy(buf, path);
            p = buf + strlen(buf);
            *p++ = '/';

            int i = 1;
            close(fd);
            fd = open(path, 0);
            
            while(read(fd, &de, sizeof(de)) == sizeof(de)){
                if(de.inum == 0) continue;

                memmove(p, de.name, DIRSIZ);
                p[DIRSIZ] = 0;
                if(strcmp(".", p) == 0 || strcmp("..", p) == 0) continue;

                for(int i = 0; i <= curLevel; ++i){
                    if(!table[i]){
                        printf((i == 0) ? "|" :  (table[i-1]) ? "    |" : "   |");
                    }
                    else{
                        printf((i == 0) ? "" : (table[i-1]) ?  "    " : "   ");
                    }
                }
                printf("\n");
                for(int i = 0; i < curLevel; ++i){
                    if(!table[i]){
                        printf("|   ");
                    }
                    else{
                        printf("    ");
                    }
                }
                printf("+-- %s\n", p);

                curLevel++;
                if(i == count){
                    table[curLevel-1] = 1;
                }
                
                tree(buf);
                ++i;
            }
        }
    }
    table[curLevel] = 0;
    curLevel--;
    close(fd);
}

int main(int argc, char *argv[]) {
    // add your code!
    if(argc < 2){
        tree(".");
        exit(0);
    }

    int fd[2];
    pipe(fd);

    if(fork() == 0){
        close(fd[0]);
        tree(argv[1]);
        write(fd[1], &dir_num, sizeof(int));
        write(fd[1], &file_num, sizeof(int));
    }
    else{
        close(fd[1]);
        read(fd[0], &dir_num, sizeof(int));
        read(fd[0], &file_num, sizeof(int));
        if(dir_num > 0) dir_num--;
        printf("\n%d directories, %d files\n", dir_num, file_num);
    }

    exit(0);
}