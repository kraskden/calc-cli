#include "var.h"
#include <stdlib.h>
#include <string.h>

int add_var(var arg, var_list **head)
{
    var_list *new = (var_list*) malloc(sizeof (*new));
    if (new) {
        new->variable = arg;
        new->next = *head;
        *head = new;
        return 1;
    }
    return 0;
}

var* get_var(const char *name, var_list *head)
{
    while (head) {
        if (!strcmp(head->variable.name, name)) { //Indentic!
            return &(head->variable);
        }
        head = head->next;
    }
    return NULL;
}

void free_var(var_list **head)
{
    if (!*head)
        return;
    free_var(&((*head)->next));
    free(*head);
    *head = NULL;
}
