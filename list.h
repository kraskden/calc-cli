#ifndef LIST_H
#define LIST_H

void push(void **head, void* data, int data_size);
void *pop(void **head, int data_size);
void put(void **head, void *data, int data_size);

#define PUSH(head, item) push((void**)&head, &item, sizeof(item))
#define POP(head, type) pop((void**)&head, sizeof(type))
#define PUT(head, item) put((void**)&head, &item, sizeof(item))

#endif
