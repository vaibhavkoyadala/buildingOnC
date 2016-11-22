#include"strutils.h"
#include"macros.h"
int countDigits(int n);

/// Deprecated (Bad design)
char* int2str(int n)
{
    int count = countDigits(n);
    char* str;
    if (n>0)
        str = (char*)malloc((count + 1)*sizeof(int));
    else{
        str = (char*)malloc((count + 2)*sizeof(int));
        str[0] = '-';
        n = -n;
    }
    str[count] = '\0';

    while (count-- > 0)
    {
        str[count] = '0' + n % 10;
        n /= 10;
    }
    return str;
}
int str2int(char *str, int *error){
    if (str == NULL)
        goto Error;

    int n = 0;
    while (*str && isDigit(*str)){
        n = n*10 + toDigit(*str);
        str++;
    }
    if (*str) goto Error;
    return n;
Error:
    if (error != NULL) *error = 1;
    return -1;
}
int countDigits(int n)
{
    if (n == 0) return 1;

    int count = 0;
    while (n){
        count++;
        n = n / 10;
    }
    return count;
}

char* cat(char *a, char *b)
{
    int tLen = len(a) + len(b);
    char *ab = (char*)malloc((tLen + 1)*sizeof(char));
    char *iter = ab;
    while (*a)
        *(iter++) = *(a++);
    while (*b)
        *(iter++) = *(b++);
    *iter = 0;
    return ab;
}
void makeUpper(char *str)
{
    if (str == NULL)    return;
    while (*str){
    if (isLower(*str))
        *str -= 'a' + 'A';
    str++;
    }
}
void makeLower(char *str)
{
    if (str == NULL)    return;
    while (*str){
    if (isUpper(*str))
        *str -= 'A' + 'a';
    str++;
    }

}
int len(char *str){
    if (str == NULL)
        return 0;
    int l = 0;
    while (*(str++))l++;
    return l; 
}
int strcmp(char *a, char *b){
    if (a == NULL && b == NULL) return 1;
    if (a == NULL || b == NULL) return 0;

    while (*a && *a == *b)
        a++, b++;

    return *a == 0 && *b == 0;
}