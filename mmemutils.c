#include"mmemutils.h"
void mmemset(char *ptr, char ch, int size)
{
    if (ptr == NULL || size <= 0) return;
    while (size--)
        *(ptr++) = ch;
}
void mmemcopy(char *from, char *to, int n)
{
    if (from == NULL || to == NULL || n <= 0)
        return;
    while (n--)
        *(to++) = *(from++);
}
void fmemset(FILE *f, char ch, int size)
{
    if (f == NULL || size <= 0)
        return;
    while (size--)
        fputc(ch, f);
}