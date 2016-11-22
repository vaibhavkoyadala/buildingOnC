#ifndef JSONPARSER_H
#define JSONPARSER_H

struct JSON{
    char *src;

    void *root;
};
typedef struct JSON JSON;

/* d for Data type */
#define tDict   1
#define tList   2
#define tPrimitive 3

#define tError 4

/*Data type objects*/
typedef struct Obj{
    int type;
}Obj;
typedef struct Dict{
    int type;
    void **keys;
    void **vals;
    int size;
}Dict;
typedef struct List{
    int type;
    void **vals;
    int size;
}List;
typedef struct Primitive{
    int type;
    int start;
    int end;
}Primitive;
typedef struct Error{
    int type;
    char *text;
}Error;

void JSON_parse(JSON *json);
void JSON_print(JSON *json, FILE *out);
void* JSON_get(JSON *json, char *query);

#endif