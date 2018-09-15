#ifndef LIST_H
#define LIST_H

void push(void **head, void* data, int data_size);
void *pop(void **head, int data_size);
void put(void **head, void *data, int data_size);
int is_exist(void *head, void *data, int data_size, int cmp_size);
void clear(void **head, int data_size);

#define PUSH(head, item) push((void**)&head, &item, sizeof(item))
#define POP(head, type) pop((void**)&head, sizeof(type))
#define PUT(head, item) put((void**)&head, &item, sizeof(item))
#define IS_EXIST(head, item, cmp_size) is_exist(head, (void*)&item, sizeof(item), cmp_size)
#define CLEAR(head, type) clear((void**)&head, sizeof(type))

#endif
