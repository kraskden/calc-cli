#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void push(void **head, void* data, int data_size)
{
    void *elem = malloc(data_size + sizeof (void*));
    memcpy(elem, data, data_size);
    memcpy((elem + data_size), head, sizeof (void*));
    *head = elem;
}

void *pop(void **head, int data_size)
{
    if (!*head)
        return NULL;
    void *tmp = *head;
    memcpy(head, tmp + data_size, sizeof (void*));
    return tmp;
}

void put(void **head, void *data, int data_size)
{
    if (!*head) {
        push(head, data, data_size);
    } else {
        void *prev = *head;
        void *next;
        do {
            memcpy(&next, prev + data_size, sizeof (void*));
            if (next)
                memcpy(&prev, prev + data_size, sizeof (void*));
        } while (next);
        /* Now in prev locate last element in list
         * Now I create new element */
        void *elem = malloc(data_size + sizeof (void*));
        memcpy(elem, data, data_size);
        memset(elem + data_size, 0, sizeof (void*));
        memcpy(prev + data_size, &elem, sizeof (void*));
    }
}


int is_exist(void *head, void *data, int data_size, int cmp_size)
{
    for (; head; memcpy(&head, head + data_size, sizeof (void*))) {
        if (!memcmp(head, data, cmp_size))
            return 1;
    }
    return 0;
}

void clear(void **head, int data_size) {
    if (!*head)
        return;
    void *next;
    memcpy(&next, *head + data_size, sizeof (void*));
    clear(&next, data_size);
    //printf("FREE - %p\n", *head);
    free(*head);
    *head = NULL;
}
