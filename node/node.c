#include <stdio.h>
#include <stdlib.h>

int main() {
    int a;
    int b;
    int NOD;
    scanf_s("%d", &a);
    scanf_s("%d", &b);
    if (a<0){
        a = abs(a);
    }
    if (b<0){
        b = abs(b);
    }
    while (a != 0 && b != 0) {
        if (a > b)
        {
            a = a%b;
        }
        else if (b > a)
        {
            b = b%a;
        }
    }
    NOD = a + b;
    printf("%d", NOD);
    return 0;
}
