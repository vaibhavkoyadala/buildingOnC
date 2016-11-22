#ifndef MACROS_H
#define MACROS_H

#define True 1
#define False 0

#define isLower(ch) (ch >= 'a' && ch <= 'z' ? (1):(0))
#define isUpper(ch) (ch >= 'A' && ch <= 'Z' ? (1):(0))
#define isLetter(ch) (isLower(ch) || isUpper(ch) ? (1):(0))

#define isDigit(ch) (ch >= '0' && ch <= '9' ? (1):(0))
#define toDigit(ch) (ch-'0')

#ifndef max
#define max(a, b) (a>b ? (a):(b))
#endif
#ifndef min
#define min(a, b) (a>b ? (b):(a))
#endif
#ifndef NEW
#define NEW(n, t) ((t*)malloc((n)*sizeof(t)))
#endif

#define CAST(o, t) ((t)(o))

#endif MACROS_H