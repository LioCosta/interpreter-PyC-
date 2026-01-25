#include <stdio.h>

int main() {
    int x = 10;
    printf("%d\n", x);
    if ((x > 5)) {
        printf("%d\n", 100);
    } else {
        printf("%d\n", 0);
    }
    int y = 0;
    while ((y < 3)) {
        printf("%d\n", y);
    }
    y = (y + 1);
    int z = (x + (y * 2));
    printf("%d\n", z);
    return 0;
}
