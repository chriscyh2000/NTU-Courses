#include "ada-hw4-p3.h"

#include <cassert>
#include <stdio.h>
#include <stdlib.h>

namespace {
    const int MAXN = 3000, MAXQ = 2000;
    unsigned int score[MAXN + 5];
    bool alive[MAXN + 5], conceal[MAXN + 5];
    int counter;
    std::vector<int> contest;
    struct Operation {
        int type, p, id;
    } operation[MAXQ + 5];
    void wa(const char* msg) {
        printf("Wrong Answer: %s\n", msg);
        exit(0);
    }
    void verify(std::vector<int> choice) {
        printf("%lu ", choice.size());
        for (int i : choice)
            printf("%d ", i);
        printf("(%d)\n", counter);
        for (int i : contest)
            conceal[i] = false;
        for (int i : choice) {
            if (!alive[i])
                wa("invalid choice.1");
            if (conceal[i])
                wa("invalid choice.2");
            conceal[i] = true;
        }
        std::vector<int> remain_contest;
        for (int i : contest)
            if (!conceal[i])
                remain_contest.push_back(i);
        for (int i = 1; i < int(remain_contest.size()); ++i)
            if (score[remain_contest[i]] < score[remain_contest[i - 1]])
                wa("invalid choice.3");
    }
}

bool compare(int x, int y) {
    ++counter;
    if (x < 0 || y < 0 || x >= MAXN || y >= MAXN || x == y || !alive[x] || !alive[y])
        wa("invalid compare query.");
    assert(score[x] != score[y]);
    bool rt = score[x] < score[y];
    return rt;
}

int main() {
    int N, Q, insert_counter = 0;
    assert(2 == scanf("%d%d", &N, &Q));
    for (int i = 0; i < N; ++i)
        assert(1 == scanf("%d", &score[i]));
    contest.resize(N);
    for (int i = 0; i < N; ++i) {
        contest[i] = i;
        alive[i] = true;
    }
    for (int i = 0; i < Q; ++i) {
        char op[10];
        assert(1 == scanf("%s", op));
        if (op[0] == 'i') {
            operation[i].type = 1, operation[i].id = N + insert_counter;
            assert(2 == scanf("%d%u", &operation[i].p, &score[operation[i].id]));
            ++insert_counter;
        }
        else {
            operation[i].type = 2;
            assert(1 == scanf("%d", &operation[i].p));
        }
    }
    counter = 0;
    verify(init(N));
    for (int i = 0; i < Q; ++i) {
        counter = 0;
        if (operation[i].type == 1) {
            assert(operation[i].p >= 0 && operation[i].p <= int(contest.size()));
            alive[operation[i].id] = 1;
            contest.insert(contest.begin() + operation[i].p, operation[i].id);
            verify(insert(operation[i].p, operation[i].id));
        }
        else {
            assert(operation[i].p >= 0 && operation[i].p < int(contest.size()));
            alive[contest[operation[i].p]] = 0;
            contest.erase(contest.begin() + operation[i].p);
            verify(remove(operation[i].p));
        }
    }
}
