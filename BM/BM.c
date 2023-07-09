#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
char* BM_search(const char *needle, char *haystack) {
    ////////////Массив сдвигов/////////
    size_t len_needle = strlen(needle);
    int* dist = calloc(len_needle, sizeof(int)); //массив смещений из 0
    if (dist == NULL) {
        printf("Error\n");
        exit(1);
    }
    size_t len_hay = strlen(haystack);
    if (len_needle == 0 || len_hay == 0 || len_needle> len_hay)
        return NULL;
    for (int i = (int)len_needle - 2; i > -1; i--) { //считаем сдвиги для всего кроме последнего
        for (int j = i + 1; j < len_needle - 1; j++)
            if (needle[i] == needle[j])
                dist[i] = dist[j];
        if (dist[i] == 0)
            dist[i] = (int)len_needle - 1 - i;
    }
    for (int i = 0; i < len_needle; i++) //для последнего
        if (needle[len_needle - 1] == needle[i])
            dist[len_needle - 1] = dist[i];
    if (dist[len_needle - 1] == 0)
        dist[len_needle - 1] = len_needle;
    /////////////Совмещаем строки и ищем 'иголку' в 'стоге сена'////////////
    int k = (int)len_needle - 1; //haystack
    int i = (int)len_needle - 1; //needle
    int find = 0;
    while (k<len_hay) {
        if (haystack[k] != needle[i]) {
            int p = 0;
            for (int j = (int)len_needle - 1; j > -1; j--) {
                if (needle[j] == haystack[k]) {
                    k += dist[j];
                    p++;
                }
                if (p != 0)
                    break;
            }
            if (p == 0) //если нет такого символа в массиве сдвигов, сдвигаем на длину строки
                k += (int)len_needle;
            i = (int)len_needle - 1;
        }
        else {
            if (i!= 0) {
                k--;
                i--;
            }
            if (i == 0) {
                find++;
                break;
            }
        }
    }
    free(dist);
    if (find == 0){
//        printf("Error: couldn't find your string.\n");
        return NULL;
    }
    else return haystack + k;
}

bool find(char *haystack, char *needle, int sub){
    char *str1 = _strdup(haystack);
    char *str2 = _strdup(needle);
    if (str1 == NULL || str2 == NULL) return false;
    if ((int)BM_search(str2, str1)-(int)(&str1) == sub) {
        free(str1);
        free(str2);
        return true;
    }
    else return false;
}

bool find(char *haystack, char*needle, size_t pos) {
    return BM_search(needle, haystack) == haystack + pos;
}

int main() {
//    assert(find("personal data", "data", 9));
    {
        char string[] = "data";
        char hay[] = "personal data";
        assert((BM_search(string, hay) - hay) == 9);
    }
    {
        char string[] = "ta";
        char hay[] = "personal data";
        assert(((int) BM_search(string, hay) - (int) &hay) == 11);
    }
    {
        char string[] = "h";
        char hay[] = "personal data";
        assert(BM_search(string, hay) == NULL);
    }
    {
        char string[] = "personal data";
        char hay[] = "personal data";
        assert(((int) BM_search(string, hay) - (int) &hay) == 0);
    }
    {
        char string[] = "personal data";
        char hay[] = "ersonal dat";
        assert(BM_search(string, hay) == NULL);
    }
    {
        char string[] = "";
        char hay[] = "personal data";
        assert(BM_search(string, hay) == NULL);
    }
    {
        char string[] = "personal data";
        char hay[] = "";
        assert(BM_search(string, hay) == NULL);
    }
    return 0;
}