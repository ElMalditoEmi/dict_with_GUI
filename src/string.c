#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../inc/string.h"

struct _s_string {
    char *content;
    unsigned int length;
};

string string_create(const char *word) {
    string str = NULL;
    str = calloc(1, sizeof(struct _s_string));
    str->length = strlen(word);
    str->content = calloc(str->length+1, sizeof(char));
    str->content = strncpy(str->content, word, str->length + 1);
    return (str);
}

unsigned int string_length(string str) {
    return (str->length);
}

bool string_less(const string str1, const string str2) {
    bool ret = false;
    unsigned int i = 0;

    while(*(str1->content+i) == *(str2->content+i) && i < str1->length && i < str2->length){
        i++;
    }
    //Si i quedo en el ultimo caracter del mas corto ,entonces eran iguales hasta ese punto
    if(i == str1->length ){
        ret = true;
    }
    //Si eran iguales hasta el i-esimo caracter y se diferenciaron antes de un string completo
    else{
        ret = *(str1->content+i) < *(str2->content+i);
    }

    return ret;
}

bool string_eq(const string str1, const string str2) {
    int cmp = strcmp(str1->content, str2->content);
    return (cmp == 0);
}

string string_clone(const string str) {
    return (string_create(str->content));
}

string string_destroy(string str) {
    free(str->content);
    free(str);
    str = NULL;
    return (str);
}

void string_dump(string str, FILE *file) {
    fprintf(file, "%s", str->content);
}

const char *string_ref(string str) {
    return (str->content);
}
