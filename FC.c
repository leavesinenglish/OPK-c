//#include <stdio.h>
//
//int main() {
//    printf("Hello, World!\n");
//    return 0;
//}
#include <stdio.h>
float FtoC(float F) {
    float C = (F-32)*5/9;
    return C;
}
float CtoF(float C) {
    float F = (9*C/5)+32;
    return F;
}


int main() {
    float a;
    char b;
    float r;
    scanf("%f %c", &a, &b);
    if (b == 'C'){
        r = CtoF(a);
    }
    else if (b == 'F'){
        r = FtoC(a);
    }
    else {
        printf("%s", "error");
        return 0;
    }
    printf("%f\n", r);
    return 0;
}

