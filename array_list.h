

#define MAX_SIZE 8

struct array_list
{
    int* content;
    int capacity;
    int size;
};

struct array_list *new_array_list();

void array_list_add_to_end(struct array_list *list, int num);

int find_element(struct array_list *list, int target);

int get_element(struct array_list *list, int index);

void destroy_array_list(struct array_list *list);
