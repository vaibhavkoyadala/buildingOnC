#include <stdio.h>
#include "jsonparser.h"
#include "macros.h"
#include "vec.h"
#define LOGGING_ON 
#include "logger.h"

#define SkipSpaces() while (src[i] == ' ' || src[i] == '\n' || src[i] == '\r'){\
                        i++;\
                        Assert(src[i] != 0, IncompleteStream); \
                     }
void printObject(Obj *o, FILE *out, char *src);
void printPrimitive(Primitive *p, FILE *out, char *src);
void printDict(Dict *dict, FILE *out, char *src);
void printList(List *list, FILE *out, char *src);
void printError(Error *e, FILE *out, char *src);
Error* Error_new(char *text){
    Error *error = NEW(1, Error);
    error->type = tError;
    error->text = text;
    return error;
}
void JSON_parse(JSON *json){
    if (json == NULL) return;
    if (json->src == NULL){
        json->root = Error_new("NULL text to parse\n");
        return;
    }
    parseObject(json->src, 0, &(json->root));
}
Primitive* Primitive_new(int type, int start, int end){
    Primitive *p = NEW(1, Primitive);
    p->type = type;
    p->start = start;
    p->end = end;
    return p;
}

int parsePrimitive(char  *src, int start, void **data){
    int i = start;
    SkipSpaces()
    start = i;
    if (src[i] == '\"'){
        i++;
        // String
        while (1){
            if (src[i] == '\"' && src[i - 1] != '\\')
                break;
            i++;
            Assert(src[i] != 0, IncompleteStream)
        }
    }
    else
        // Number or null or true or false
        while (1){
            if (src[i] == '}' || src[i] == ']' || src[i] == ',' || src[i] == ' ' || src[i] == '\n' || src[i] == '\r')
                break;
            if (src[i] == '{' || src[i] == '[')
                goto TokenError;
            i++;
            Assert(src[i] != 0, IncompleteStream)
        }
    if (src[i] == '}' || src[i] == ']' || src[i] == ','){
        *data = Primitive_new(tPrimitive, start, i-1);
        return i;
    }
    *data = Primitive_new(tPrimitive, start, i);
    return i+1;
IncompleteStream: 
    *data = Error_new(tError, "Incomplete srceam");
    return -1;
TokenError:
    return Error_new(tError, "Token error");
}
int parseDict(char *src, int start, void **data){
    // Assuming src[0] = '{'
    int i = start+1;

    // Special case, when dict is empty
    SkipSpaces()
    if (src[i] == '}'){
        Dict *dict = NEW(1, Dict);
        dict->type = tDict;
        dict->keys = NULL;
        dict->vals = NULL;
        dict->size = 0;
        *data = dict;
        return i + 1;
    }

    void *tmp = NULL;
    Vec *keys = Vec_new(2);
    Vec *vals = Vec_new(2);

    

    while (1){
        SkipSpaces()
        if ((i = parsePrimitive(src, i, &tmp)) == -1)
            goto PushError;
        // The key must be a string
        if (src[CAST(tmp, Primitive*)->start] != '\"'){
            tmp = Error_new("Key is not a String !");
            goto PushError;
        }

        Vec_append(keys, tmp);
        
        SkipSpaces()
        Assert(src[i++] == ':', TokenError);
        SkipSpaces()

        if ((i = parseObject(src, i, &tmp)) == -1)
            goto PushError;
        Vec_append(vals, tmp);
        SkipSpaces()
        if (src[i] == '}')
            break;
        
        Assert(src[i++] == ',', TokenError);
    }
    Dict *dict = NEW(1, Dict);
    dict->type = tDict;
    dict->keys = Vec_vec2Arr(keys, &(dict->size));
    dict->vals = Vec_vec2Arr(vals, NULL);
    *data = dict;
    return start + i + 1;

IncompleteStream: 
    return Error_new(tError, "Incomplete srceam");
TokenError:
    return Error_new(tError, "Token error");
PushError:
    // Cleanup
    Vec_destroy(keys, free);
    Vec_destroy(vals, free);

    *data = tmp;
    return -1;
}
int parseList(char *src, int start, void **data){
    // Assuming src[0] = '['
    int i = start+1;
    
    // Special case, when dict is empty
    SkipSpaces()
    if (src[i] == ']'){
        List *list = NEW(1, List);
        list->type = tDict;
        list->vals = NULL;
        list->size = 0;
        *data = list;
        return i + 1;
    }

    void *tmp = NULL;
    Vec *vals = Vec_new(2);
    while (1){
        SkipSpaces()
        if ((i = parseObject(src, i, &tmp)) == -1)
            goto PushError;
        Vec_append(vals, tmp);

        SkipSpaces()
        if (src[i] == ']'){
            i++;
            break;
        }
        Assert(src[i++] == ',', TokenError);
    }
    List *list = NEW(1, List);
    list->type = tList;
    list->vals = Vec_vec2Arr(vals, &(list->size));
    *data = list;
    return  i;

IncompleteStream:
    return Error_new(tError, "Incomplete srceam");
TokenError:
    return Error_new(tError, "Token error");
PushError:
    *data = tmp;
    return -1;
}
int parseObject(char *src, int start, void **data){
    int i = start;
    SkipSpaces()
    if (src[i] == '[')
        return parseList(src, i, data);
    if (src[i] == '{')
        return parseDict(src, i, data);
    return parsePrimitive(src, i, data);
    
IncompleteStream:
    return Error_new(tError, "Incomplete srceam");
}

int equals(char *key, char delimiter, char *src, int from, int to){
    // Purpose built
    // True: If the srcing in 'key' upto 'delimiter' equals
    // the srcing in 'src' in (from, to).

    from++; 
    while (*key && *key != delimiter && from < to)
        if (*(key++) != src[from++]) 
            return False;

    return (*key == delimiter || *key == 0 ) && from == to;
}
void* getInDict(Dict *dict, char *src, char *key, char delimiter){
    int size = dict->size;
    Primitive **keys = dict->keys;
    for (int i = 0; i < size; i++){
        if (equals(key, delimiter, src, keys[i]->start, keys[i]->end))
            return dict->vals[i];
    }
    return NULL;
}
void* getInList(List *list, char *key, char delimiter){
    int index = 1;
    while (*key && *key != delimiter){
        if (!isDigit(*key)) return NULL;
        index *= toDigit(*key);
        key++;
    }
    if (index >= list->size) return NULL;
    return list->vals[index];
}
void* JSON_get(JSON *json, char *query){
    if (json == NULL || query == NULL)
        return NULL;
    void *obj = json->root;
    while (*query){
        if (isDigit(*query))
            obj = getInList(obj, query, '|');
        else
            obj = getInDict(obj, json->src, query, '|');
        if (obj == NULL) return NULL;
        while (1){
            if (*query == 0)
                break;
            if (*(query++) == '|')
                break;
        }
    }
    return obj;
}


void JSON_print(JSON *json, FILE *out){
    if (json == NULL) return;
    if (json->src == NULL || json->root == NULL){
        warn("[JSON_print()] json object has NULL elements.");
        return;
    }
    if (out == NULL) out = stdout;
    printObject(json->root, out, json->src); 
}
void printObject(Obj *o, FILE *out, char *src){
    switch ((o->type))
    {
    case tDict:
        printDict(o, out, src);
        break;
    case tList:
        printList(o, out, src);
        break;
    case tPrimitive:
        printPrimitive(o, out, src);
        break;
    case tError:
        printError(o, out, src);
        break;
    default:
        warn("[ JSON printObject()] Unknown Object type");
    }
}
void printError(Error *e, FILE *out, char *src){
    fprintf(out, "Error: %s", e->text);
}
void printPrimitive(Primitive *p, FILE *out, char *src){
    int end = p->end;
    char backup = src[end + 1];
    src[end + 1] = 0;
    fprintf(out, "%s", src + p->start);
    src[end + 1] = backup;
}
void printDict(Dict *dict, FILE *out, char *src){
    void **keys = dict->keys;
    void **vals = dict->vals;
    int size = dict->size;
    fprintf(out, "{ ");
    while (size--){
        printObject(*keys, out, src);
        fprintf(out, ": ");
        printObject(*vals, out, src);
        if(size) fprintf(out, ", ");
        keys++, vals++;
    }
    fprintf(out, " }");
}
void printList(List *list, FILE *out, char *src){
    void **vals = list->vals;
    int size = list->size;
    fprintf(out, "[ ");
    while (size--){
        printObject(*vals, out, src);
        if(size) fprintf(out, ", ");
        vals++;
    }
    fprintf(out, " ]");
}
/*
void main(){
    
    char src[] = "{ \"hey\": [1, 2, null] }";
    JSON json;
    json.src = src;
    JSON_parse(&json);
    JSON_print(&json, NULL);
    printObject(JSON_get(&json, "hey|2"), stdout, json.src); 
    getch();
    SOCKET conn()
    }
    */