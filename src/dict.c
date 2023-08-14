#include <assert.h>
#include "../inc/dict.h"
#include <stdlib.h>
#include "../inc/dict_helpers.h"

struct _node_t {
    dict_t left;
    dict_t right;
    key_t key;
    value_t value;
};

//Estructura de datos para mantener la dinamica de la funcion max_leaf_killnfetch
struct string_pair{
    value_t val_p;
    key_t key_p;
};

static bool invrep(dict_t d) {
    bool ret;
    if(d == NULL){
        ret = true; 
    }
    else{
        ret = invrep(d->left) && 
        invrep(d->right) && 
        (d->left != NULL? key_less(d->left->key, d->key) : true) && 
        (d->right != NULL? key_less(d->key, d->right->key) : true);
    }
    return ret;
}

dict_t dict_empty(void) {
    dict_t dict = NULL;
    assert(invrep(dict));
    return dict;
}

static struct string_pair max_leaf_killnfetch(dict_t dict){
    struct _node_t *terminator = NULL;
    struct _node_t *terminator_padre = NULL;
    struct string_pair output;

    terminator_padre = dict->left;
    //si hay cosas a derecha
    if(terminator_padre->right != NULL){
        terminator = terminator_padre->right;

        while(terminator->right!=NULL){
            terminator_padre = terminator_padre->right;
            terminator = terminator->right;
        }
        output.key_p = key_copy(terminator->key);
        output.val_p = value_copy(terminator->value);
        key_destroy(terminator->key);    
        value_destroy(terminator->value);
        free(terminator);
        terminator_padre->right = NULL;
    }
    //si solo hay cosas a izquierda, la raiz es el maximo
    else{
        output.key_p = key_copy(terminator_padre->key);
        output.val_p = value_copy(terminator_padre->value);
        dict->left = terminator_padre->left;
        key_destroy(terminator_padre->key);
        value_destroy(terminator_padre->value);
        free(terminator_padre);
    }
    return output;
}

dict_t dict_add(dict_t dict, key_t word, value_t def) {
    assert(invrep(dict));
    struct _node_t *new_def = NULL;

    if(dict == NULL){
        dict = malloc(sizeof(struct _node_t));
        dict->key = word;
        dict->value = def;
        dict->left = NULL;
        dict->right = NULL;
    }
    else if(key_eq(dict->key,word)){
        string_destroy(dict->value);
        dict->value = def;
    }
    else{
        if(key_less(word,dict->key)){
            if(dict->left == NULL){
                new_def = malloc(sizeof(struct _node_t));
                new_def->left = NULL;
                new_def->right = NULL;
                new_def->key = word;
                new_def->value = def;

                dict->left = new_def;
            }
            else{
                dict->left = dict_add(dict->left,word,def);
            }
        }
        else{
            if(dict->right == NULL){
                new_def = malloc(sizeof(struct _node_t));
                new_def->left = NULL;
                new_def->right = NULL;
                new_def->key = word;
                new_def->value = def;

                dict->right = new_def;
            }
            else{
                dict->right = dict_add(dict->right,word,def);
            }
        }
    }
    assert(invrep(dict) && dict_exists(dict, word));
    return dict;
}

value_t dict_search(dict_t dict, key_t word) {
    key_t def=NULL;
    if(key_eq(dict->key,word)){
        //Por seguridad se devuelve una copia 
        def = string_clone(dict->value);
    }
    else if(key_less(word,dict->key)){
        def = dict_search(dict->left,word);
    }
    else{
        def = dict_search(dict->right,word);
    }
    return def;
}

bool dict_exists(dict_t dict, key_t word) {
    bool ret = false;
    if(dict == NULL){
        ret = false;
    }
    else if(key_eq(dict->key,word)){
        ret = true;
    }
    else if(key_less(dict->key,word)){
        ret = dict_exists(dict->right,word);
    }
    else{
        ret = dict_exists(dict->left,word);
    }
    return ret;
}

unsigned int dict_length(dict_t dict) {
    unsigned int len = 0u;
    if(dict != NULL){
        len++;
        len = len + dict_length(dict->left) + dict_length(dict->right);
    }
    return len;
}

dict_t dict_remove(dict_t dict, key_t key_to_remov) {
    struct _node_t *dict_pointer;
    struct string_pair max_key_pair;
    assert(invrep(dict));

    dict_pointer = dict;
    if(key_eq(dict_pointer->key,key_to_remov)){

//Padre de dos elementos
        if(dict_pointer->left != NULL && dict_pointer->right != NULL){
            max_key_pair  = max_leaf_killnfetch(dict_pointer);
            dict_pointer->key = max_key_pair.key_p;
            dict_pointer->value = max_key_pair.val_p;
        }

//Padre de un solo elemento, para alguna de las 2 direcciones
        else if(dict_pointer->left == NULL && dict_pointer->right != NULL){
            dict = dict_pointer->right;
            key_destroy(dict_pointer->key);
            value_destroy(dict_pointer->value);
            free(dict_pointer);
        }

        else if(dict_pointer->right == NULL && dict_pointer->left != NULL){
            dict = dict_pointer->left;
            key_destroy(dict_pointer->key);
            value_destroy(dict_pointer->value);
            free(dict_pointer);
        }
//Arbol de un elemento
        else if(dict_pointer->right == NULL && dict_pointer->left == NULL){
            key_destroy(dict_pointer->key);
            value_destroy(dict_pointer->value);
            free(dict_pointer);
            dict = NULL;
        }

//Si el elemento buscado es un leaf tenemos que verlo desde un nodo anterior, a menos que sea el arbol de un elemento
    }
    //Con el if anterior sabemos que left y right no son null
    //Ya que asumimos que el elemento existe en el arbol (es precondicion en el teorico)
    else if(key_less(dict_pointer->key,key_to_remov)){
        dict->right = dict_remove(dict_pointer->right,key_to_remov);
    }
    else if(key_less(key_to_remov,dict_pointer->key)){
        dict->left = dict_remove(dict_pointer->left,key_to_remov);
    }

    assert(invrep(dict));
    assert(!dict_exists(dict, key_to_remov));
    return dict;
}

dict_t dict_remove_all(dict_t dict){

    if(dict != NULL){
        dict->right = dict_remove_all(dict->right);
        dict-> left = dict_remove_all(dict->left);
        dict->key = key_destroy(dict->key);         //RECORDAR QUE DE LAS PALABRAS SE ESTA PASANDO UNA COPIA
        dict->value = value_destroy(dict->value);
        free(dict);
    }
    return NULL;
}

void dict_dump(dict_t dict, FILE * out_stream) {
    assert(invrep(dict));
    if (dict != NULL) {
        //Para obtener en el dump el orden en que se reconstruye el arbol:
            //Hay que mostrar siempre la raiz primero y luego correr la funcion en las ramas
        key_dump(dict->key,out_stream);
        fprintf(out_stream,":");
        value_dump(dict->value,out_stream);
        printf("\n");
        dict_dump(dict->right,out_stream);
        dict_dump(dict->left,out_stream);
    }
}

dict_t dict_destroy(dict_t dict){//En consecuencia de la implementacion particular,esta funcion es la misma que dict_remove_all

    if(dict != NULL){
        dict->right = dict_destroy(dict->right);
        dict-> left = dict_destroy(dict->left);
        dict->key = key_destroy(dict->key);         //RECORDAR QUE DE LAS PALABRAS SE ESTA PASANDO UNA COPIA
        dict->value = value_destroy(dict->value);
        free(dict);
    }
    return NULL;
}