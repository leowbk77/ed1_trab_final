#include "Tlist.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct list_node node;

struct list {
    node *head;
    node *end;
};

struct list_node {
    ponto p;
    node *next;
};

list *list_create() {
    list *list; 
    
    list = malloc(sizeof(list));
    if (list != NULL) {
        list -> head = NULL;
        list -> end = NULL;
    }

    return list;
}

int list_free(list *list) {
    if (list == NULL) {
        return INVALID_NULL_POINTER;
    }

    node *aux;
    aux = list -> head;

    while (aux != NULL) {
        aux = aux -> next;

        free(list -> head);

        list -> head = aux;
    }

    free(list);
    return SUCCESS;
}

int push(list *list, ponto ponto) {
    if (list == NULL) {
        return INVALID_NULL_POINTER;
    }

    node *node_data;
    node_data = malloc(sizeof(node_data));
    if (node_data == NULL) {
        return OUT_OF_MEMORY;
    }

    node_data -> p = ponto; 
    
    if (list -> head == NULL) {
        list -> head = node_data;
        list -> end = node_data;
        node_data -> next = NULL;

        return SUCCESS;
    }

    (list -> end) -> next = node_data;
    list -> end = node_data;
    node_data -> next = NULL;

    return SUCCESS;
}

int pop(list *list, ponto *p) {
    if (list == NULL) {
        return INVALID_NULL_POINTER;
    }

    if (list -> head == NULL) {
        return ELEM_NOT_FOUND;
    }

    *p = (list -> end) -> p;

    node *aux;
    aux = list -> head;

    if (aux -> next == NULL) {
        free(aux);
        list -> head = NULL;
        list -> end = NULL;

        return SUCCESS;
    }

    while (aux -> next != list -> end) {
        aux = aux -> next;
    }

    aux -> next = NULL;
    free(list -> end);
    list -> end = aux;

    return SUCCESS;
}

int is_empty(list *list) {
    if (list == NULL) {
        return INVALID_NULL_POINTER;
    }

    if (list -> head == NULL) {
        return 1; // se estiver vazia, retorna 1:true
    } else {
        return 0; // se não estiver vazia, retorna 0:false
    }
}

int list_insert_front(list *list, ponto p) {
    if (list == NULL) return INVALID_NULL_POINTER;

    node *node_data;
    node_data = malloc(sizeof(node_data));
    if (node_data == NULL) return OUT_OF_MEMORY;

    node_data -> p = p;
    node_data -> next = list -> head;
    list -> head = node_data;

    return SUCCESS;
}

int list_print(list *list) {
    if (list == NULL) return INVALID_NULL_POINTER;

    int pos = 1;

    node *aux;
    aux = list -> head;

    while (aux != list -> end) {
        printf("Posição %d: (%d, %d)\n", pos, aux -> p.x
        , aux -> p.y);
        pos++;
        aux = aux -> next;
    }

    printf("Posição %d: (%d, %d)\n", pos, aux -> p.x, aux -> p.y); //printando list -> end

    return SUCCESS;
}

int top(list *list, ponto *p) {
    if (list == NULL) return INVALID_NULL_POINTER;

    if (is_empty(list)) {
        return ELEM_NOT_FOUND;
    }

    *p = (list -> end) -> p;

    return SUCCESS;
}