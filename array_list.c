#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "array_list.h"

struct array_list *new_array_list()
{
    struct array_list *list = malloc(sizeof(struct array_list));
    list->content = malloc(sizeof(int) * MAX_SIZE);
    list->capacity = MAX_SIZE;
    list->size = 0;
    return list;
}

void expand_array_list(struct array_list *list) {
    // allocate a list of twice the size of the previous
    int new_capacity = (list->capacity) * 2;
    list->capacity = new_capacity;

    // copy values
    list->content = realloc(list->content, sizeof(int) * new_capacity);

    // initialize remaining slots to NULL
    for(int i = list->size; i < new_capacity; ++i) {
        list->content[i] = 0;
    }
}

// appends to list, if len exceeds size then size is increased by 2 times
void array_list_add_to_end(struct array_list *list, int num)
{
    if ((list->size + 1) > list->capacity)
    {
        // reallocate to a larger array
        expand_array_list(list);
    }
    // append to list content
    list->content[list->size] = num;
    ++(list->size);
}

void destroy_array_list(struct array_list *list) 
{
    free(list->content);
    free(list);
}

// returns the index of the first instance of the target if it is found
int find_element(struct array_list *list, int target) {
    for (int i = 0; i<list->size; ++i) {
        if (list->content[i] == target) {
            return i;
        }
    }
    return -1;
}

// returns the element at the supplied index, otherwise -1
int get_element(struct array_list *list, int index) {
    if (index > list->size || index < 0) return -1;
    return list->content[index];
}
