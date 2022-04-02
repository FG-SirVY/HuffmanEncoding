#ifndef LINKED_LIST_H
#define LINKED_LIST_H


#include <stdlib.h>
#include <stdarg.h>

#include "error.h"


/**
 * @brief A node of a doubly linked list.
 * 
 */
struct linked_list_node {
    struct linked_list_node* next;
    struct linked_list_node* prev;
    void* item;
};

/**
 * @brief A doubly linked list.
 * 
 */
struct linked_list {
    int length;
    struct linked_list_node* head;
    struct linked_list_node* tail; 
};

/**
 * @brief Creates a node with a specified pointer to the item.
 * 
 * @param item the item this node should contain.
 * @return struct linked_list_node* the node created containing <item>.
 * Must be freed with a call to linked_list_free_node().
 */
struct linked_list_node* linked_list_create_node(void* item);

/**
 * @brief Frees a node. This will however not free the content of this node.
 * This must be freed manually before freeing this node.
 * 
 * @param node the node to be freed.
 */
void linked_list_free_node(struct linked_list_node* node);

/**
 * @brief Creates an empty doubly linked list.
 * 
 * @return struct linked_list* the created linked list.
 * Must be freed with a call to linked_list_free().
 */
struct linked_list* linked_list_create();

/**
 * @brief Frees a linked list and all of its node. Contents will however not be freed.
 * They must be freed manually before.
 * 
 * @param list the list which will be freed.
 */
void linked_list_free(struct linked_list* list);

/**
 * @brief Determines the number of items in a list.
 * 
 * @param list the list of which the length should be determined.
 * @return int the number of items in <list>.
 */
int linked_list_length(struct linked_list* list);

/**
 * @brief Appends an item to as linked list.
 * 
 * @param list the list to which the item should be appended.
 * @param item the item that should be appended.
 * @return int non-zero if an error occurred, zero otherwise.
 */
int linked_list_append(struct linked_list* list, void* item);

/**
 * @brief Removes a node from a list and returns its item.
 * 
 * @param list the list from which a node should be removed.
 * @param index the index at which the node is that should be removed.
 * @return void* the item that was at this index.
 */
void* linked_list_pop(struct linked_list* list, int index);

/**
 * @brief Returns the item at a certain index in a linked list.
 * 
 * @param list the list containing the item.
 * @param index the index of the item to be returned.
 * @return void* the item at index <index> in list <list>, or NULL if the index
 * does not exist.
 */
void* linked_list_at(struct linked_list* list, int index);

/**
 * @brief Returns the node at a certain index.
 * 
 * @param list the list containing the node.
 * @param index the index of the node to be returned.
 * @return struct linked_list_node* the node at index <index> in list <list>,
 * or NULL if the index does not exist.
 */
struct linked_list_node* linked_list_node_at(
    struct linked_list* list, int index);

/**
 * @brief Inserts a new item at a specified index in a linked list.
 * 
 * @param list the list this item should be inserted in.
 * @param index the index this item should be inserted at.
 * @param item the item to be inserted.
 * @return int non-zero if an error occurred, zero otherwise.
 */
int linked_list_insert_at(struct linked_list* list, int index, void* item);

/**
 * @brief Inserts an item before a specific node in a linked list.
 * 
 * @param list the list in which the item should be inserted.
 * @param before_node the node before which the item should be inserted.
 * @param item the item to be inserted into <list>.
 * @return int non-zero if an error occurred, zero otherwise.
 */
int linked_list_insert_before(struct linked_list* list,
    struct linked_list_node* before_node, void* item);

/**
 * @brief Finds a node that has certain attributes determined through a
 * specified function.
 * 
 * @param list the list which should be analyzed.
 * @param finder a function that will receive the item to be checked and
 * a user-defined payload. Iff this returns non-zero, then the node containg
 * <item> will be returned.
 * @param payload the payload that should be passed on to the checker function.
 * @return struct linked_list_node* the node if it was found, NULL otherwise.
 */
struct linked_list_node* linked_list_find_node(struct linked_list* list,
    int (*finder)(void* item, void* payload), void* payload);

#endif
