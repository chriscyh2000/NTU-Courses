#include "kernel/types.h"

#include "kernel/fcntl.h"
#include "kernel/stat.h"
#include "user/user.h"
void mkfile(char *filename) {
    int fd = open(filename, O_CREATE | O_RDWR);
    write(fd, "hi", 3);
    close(fd);
}

void mkd(char *dirname) {
    if (mkdir(dirname) < 0) {
        fprintf(2, "mkdir %s failed.", dirname);
        exit(1);
    }
}
void test0() {
    mkd("os2022");

    mkd("os2022/d1");
    mkd("os2022/d2");
    mkd("os2022/d3");

    mkd("os2022/d2/a");
    mkd("os2022/d2/b");
    mkfile("os2022/d2/c");

    mkd("os2022/d3/a");
    mkfile("os2022/d3/b");
}

void test1() {
    mkd("a");
    mkd("a/0");
    mkd("a/1");
    mkd("a/2");
    mkd("a/3");
    mkd("a/4");
    mkd("a/5");
    mkd("a/6");
    mkd("a/7");
    mkd("a/8");
    mkd("a/9");
    mkd("a/10");
}

void private() {
    // 25 pts pa/
    // 10 pts pa/a/a/a/b
    mkd("pa");
    mkd("pa/a");
    mkd("pa/a/a");
    mkd("pa/a/a/a");
    mkd("pa/a/a/a/a");
    mkfile("pa/a/a/a/b");

    mkd("pa/b");
    mkd("pa/b/b");
    mkd("pa/b/b/b");
    mkd("pa/b/b/b/b");
    mkfile("pa/b/b/b/c");

    mkd("pa/c");
    mkd("pa/c/b");
    mkd("pa/c/b/b");
    mkd("pa/c/b/b/b");
    mkfile("pa/c/b/b/c");

    mkd("pa/d");
    mkd("pa/d/b");
    mkd("pa/d/b/b");
    mkfile("pa/d/b/b/c");

    // 25 pts
    mkd("pb");
    mkfile("pb/a");
    mkfile("pb/b");
    mkfile("pb/c");
    mkfile("pb/d");
    mkfile("pb/e");
    mkfile("pb/f");
    mkfile("pb/g");
    mkfile("pb/h");
    mkfile("pb/i");
    mkfile("pb/j");
    mkfile("pb/k");
    mkfile("pb/l");
    mkfile("pb/m");
    mkfile("pb/n");
    mkfile("pb/o");
    mkfile("pb/p");
    mkfile("pb/q");
    mkfile("pb/r");
    mkfile("pb/s");
    mkfile("pb/t");
}

int main(int argc, char *argv[]) {
    test0();
    test1();

private
    ();
    exit(0);
}
