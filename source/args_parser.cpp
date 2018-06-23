#include <stdio.h>
int main(int nargs, char* args[]) {
    printf("%d\n", nargs - 1);
    for (int i = 1; i < nargs; ++i) {
        printf("%s\n", args[i]);
    }
}
