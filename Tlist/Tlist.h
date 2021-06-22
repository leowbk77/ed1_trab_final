#ifndef _tlisth_
#define _tlisth_

/*
*   DEFINES
*/

#define SUCCESS 0
#define INVALID_NULL_POINTER -1
#define OUT_OF_MEMORY -2
#define OUT_OF_RANGE -3
#define ELEM_NOT_FOUND -4

typedef struct list list;
typedef struct ponto ponto;

struct ponto {
    int x;
    int y;
};

/*
*   FUNCOES
*/

list *list_create();

int list_free(list *list);

int push(list *list, ponto p);

int pop(list *list, ponto *p);

int top(list *list, ponto *p);

int is_empty(list *list);

int list_insert_front(list *list, ponto p);

int list_print(list *list);

#endif