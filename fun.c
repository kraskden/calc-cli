#include "fun.h"
#include <stdlib.h>
#include <string.h>

int add_fun(fun arg, fun_list **head)
{
    fun_list *new = (fun_list*) malloc(sizeof (*new));
    if (new) {
        new->function = arg;
        new->next = *head;
        *head = new;
        return 1;
    }
    return 0;
}

fun* get_fun(const char *name, fun_list *head)
{
    while (head) {
        if (!strcmp(head->function.name, name)) { //Indentic!
            return &(head->function);
        }
        head = head->next;
    }
    return NULL;
}

void free_fun(fun_list **head)
{
    if (!*head)
        return;
    free_fun(&((*head)->next));
    free(*head);
    *head = NULL;
}

void fun_init_base(fun_list **head)
{
    fun add;
    add.type = fun_base;
#define ADD_F(NAME) strcpy(add.name, (NAME)); add_fun(add, head)
    ADD_F("sin"); ADD_F("cos"); ADD_F("tan"); ADD_F("ctg");
    ADD_F("arcsin"); ADD_F("arccos"); ADD_F("arctan"); ADD_F("arcctg");
    ADD_F("exp"); ADD_F("ln"); ADD_F("lg");
    ADD_F("negative");
}
