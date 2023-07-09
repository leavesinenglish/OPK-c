#include <ctype.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
void strclean(char *string){
//    if (*string == '\0')
//        return;
//    int count = 0 , cnull = 0;
//    for (i = 0; string[i] != '\0'; i++) {
//        if (isspace(string[i]))
//            cnull++;
//        count++;
//    }
//    if (cnull == count) {
//        *string = '\0';
//        return;
//    }

    bool lastSpace = false; //1 - встретили пробел; 0 - не встретили;

    int dst = 0;
    for (int src = 0; string[src] != '\0'; src++) {
        if (!isspace(string[src])){ //не нашли пробел
            string[dst++] = string[src];
            lastSpace = false;
        } else{ //наткнулись на пробел
            if(!lastSpace && dst != 0){
                string[dst++] = string[src]; //если встретили пробел первый раз - оставляем его,
            }                           //если не первый раз - пропускаем
            if (dst == 0){}
            lastSpace = true;
        }
    }
    if (dst >= 1 && isspace(string[dst - 1]))
        dst--;

    string[dst] = '\0';
}

bool compare(char *str1Literal, char *str2){
    char *str1 = _strdup(str1Literal);
    if (str1 == NULL) return false;
    strclean(str1);
    bool res = strcmp(str1, str2) == 0;
    free(str1);
    return res;
}

int main(){
    assert(compare("abc    def", "abc def"));
    assert(compare("abc  \t  def  ", "abc def"));
    assert(compare("   abc    def  ", "abc def"));
    assert(compare("", ""));
    assert(compare(" \n ", ""));
    return 0;
}