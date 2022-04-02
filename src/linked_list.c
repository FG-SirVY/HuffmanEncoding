#include "linked_list.h"


struct linked_list_node* linked_list_create_node(void* item) {
    struct linked_list_node* node = malloc(sizeof(struct linked_list_node));
    if (!node) {
        errno = ERR_MEM_ERROR;
        return NULL;
    }

    node->prev = NULL;
    node->next = NULL;
    node->item = item;

    return node;
}

void linked_list_free_node(struct linked_list_node* node) {
    free(node);
}

struct linked_list* linked_list_create() {
    struct linked_list* ret = calloc(1, sizeof(struct linked_list));
    if (!ret) {
        errno = ERR_MEM_ERROR;
    }

    return ret;
}

void linked_list_free(struct linked_list* list) {
    struct linked_list_node* current = list->head;

    while (current != NULL) {
        struct linked_list_node* next = current->next;
        linked_list_free_node(current);
        current = next;
    }
}

int linked_list_length(struct linked_list* list) {
    return list->length;
}

int linked_list_append(struct linked_list* list, void* item) {
    struct linked_list_node* node = linked_list_create_node(item);

    if (!node) return 1;

    if (linked_list_length(list) == 0) {
        list->head = node;
        list->tail = node;
    } else {
        node->prev = list->tail;
        list->tail->next = node;
        list->tail = node;
    }

    list->length += 1;

    return 0;
}

struct linked_list_node* linked_list_node_at(struct linked_list* list,
        int index) {
    if (index <= linked_list_length(list) / 2) {
        struct linked_list_node* node = list->head;
        while (index > 0) {
            node = node->next;
            index -= 1;
        }

        return node;
    } else {
        struct linked_list_node* node = list->tail;
        index = linked_list_length(list) - index - 1;
        while (index > 0) {
            node = node->prev;
            index -= 1;
        }

        return node;
    }
}

void* linked_list_at(struct linked_list* list, int index) {
    struct linked_list_node* node = linked_list_node_at(list, index);

    if (!node) return NULL;
    else return node->item;
}

void* linked_list_pop(struct linked_list* list, int index) {
    if (index < 0 || index >= linked_list_length(list)) {
        errno = ERR_ILLEGAL_ARG;
        return NULL;
    }

    struct linked_list_node* node = NULL;

    if (index == 0) {
        node = list->head;
        list->head = node->next;

        if (linked_list_length(list) == 1) {
            list->tail = NULL;
        } else {
            list->head->prev = NULL;
        }
        
    } else if (index == linked_list_length(list) - 1) {
        node = list->tail;
        list->tail = node->prev;
        list->tail->next = NULL;
    } else {
        node = linked_list_node_at(list, index);
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }

    void* item = node->item;
    linked_list_free_node(node);
    list->length -= 1;

    return item;
}

int linked_list_insert_at(struct linked_list* list, int index, void* item) {
    if (index < 0 || index >= linked_list_length(list)) {
        errno = ERR_ILLEGAL_ARG;
        return 1;
    }

    struct linked_list_node* node = linked_list_create_node(item);

    if (index == 0) {
        struct linked_list_node* next = list->head;
        next->prev = node;
        node->next = next;

        list->head = node;
    }  else {
        struct linked_list_node* next = linked_list_node_at(list, index);
        struct linked_list_node* prev = next->prev;

        prev->next = node;
        next->prev = node;

        node->prev = prev;
        node->next = next;
    }

    list->length += 1;

    return 0;
}

int linked_list_insert_before(struct linked_list* list,
        struct linked_list_node* before_node, void* item) {
    if (!before_node) {
        return linked_list_append(list, item);
    }

    struct linked_list_node* node = linked_list_create_node(item);
    if (!node) return 1;

    if (before_node == list->head) {
        list->head->prev = node;
        node->next = list->head;

        list->head = node;
    } else {
        struct linked_list_node* next = before_node;
        struct linked_list_node* prev = next->prev;

        prev->next = node;
        next->prev = node;

        node->prev = prev;
        node->next = next;
    }

    list->length += 1;

    return 0;
}

struct linked_list_node* linked_list_find_node(struct linked_list* list,
        int (*finder)(void*, void*), void* payload) {
    struct linked_list_node* node = list->head;

    while (node) {
        if (finder(node->item, payload)) {
            return node;
        }
        node = node->next;
    }

    return NULL;
}
