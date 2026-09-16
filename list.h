// list.c
#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Helper: allocate a new node with given value (heap)
node_t* getNode(elem value) {
    node_t* new_node = (node_t*)malloc(sizeof(node_t));
    if (new_node == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    new_node->value = value;
    new_node->next = NULL;
    return new_node;
}

// Allocate a new empty list (heap)
list_t* list_alloc() {
    list_t* l = (list_t*)malloc(sizeof(list_t));
    if (l == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    l->head = NULL;
    return l;
}

// Free the entire list (all nodes and the list struct)
void list_free(list_t* l) {
    if (l == NULL) return;
    node_t* current = l->head;
    while (current != NULL) {
        node_t* temp = current;
        current = current->next;
        free(temp);
    }
    free(l);
}

// Print the list (for debugging)
void list_print(list_t* l) {
    if (l == NULL) {
        printf("NULL list\n");
        return;
    }
    node_t* curr = l->head;
    printf("[ ");
    while (curr != NULL) {
        printf("%d ", curr->value);
        curr = curr->next;
    }
    printf("]\n");
}

// Return a string representation (allocate memory on heap – caller must free)
char* listToString(list_t* l) {
    if (l == NULL) {
        char* str = (char*)malloc(10 * sizeof(char));
        if (str) strcpy(str, "NULL");
        return str;
    }
    // First pass: compute length needed (rough)
    node_t* curr = l->head;
    int len = 0;
    while (curr != NULL) {
        len += snprintf(NULL, 0, "%d ", curr->value);
        curr = curr->next;
    }
    // Add brackets and null terminator
    int total_len = len + 4; // for "[ " "]" and possibly extra
    char* str = (char*)malloc(total_len * sizeof(char));
    if (str == NULL) return NULL;
    char* ptr = str;
    ptr += sprintf(ptr, "[ ");
    curr = l->head;
    while (curr != NULL) {
        ptr += sprintf(ptr, "%d ", curr->value);
        curr = curr->next;
    }
    sprintf(ptr, "]");
    return str;
}

// Length of list
int list_length(list_t* l) {
    if (l == NULL) return 0;
    int count = 0;
    node_t* curr = l->head;
    while (curr != NULL) {
        count++;
        curr = curr->next;
    }
    return count;
}

// Add to back (append)
void list_add_to_back(list_t* l, elem value) {
    if (l == NULL) return;
    node_t* new_node = getNode(value);
    if (l->head == NULL) {
        l->head = new_node;
        return;
    }
    node_t* curr = l->head;
    while (curr->next != NULL) {
        curr = curr->next;
    }
    curr->next = new_node;
}

// Add to front (prepend)
void list_add_to_front(list_t* l, elem value) {
    if (l == NULL) return;
    node_t* new_node = getNode(value);
    new_node->next = l->head;
    l->head = new_node;
}

// Add at specific index (1-based index, as per comment in header)
void list_add_at_index(list_t* l, elem value, int index) {
    if (l == NULL || index < 1) return; // invalid index
    if (index == 1) {
        list_add_to_front(l, value);
        return;
    }
    // Traverse to node before the desired position
    node_t* curr = l->head;
    int pos = 1;
    while (curr != NULL && pos < index - 1) {
        curr = curr->next;
        pos++;
    }
    if (curr == NULL) {
        // Index out of bounds – we can either ignore or append.
        // Let's append if index == length+1, else ignore.
        if (pos == index - 1) {
            // curr is the last node, add to back
            list_add_to_back(l, value);
        }
        return;
    }
    // Insert after curr
    node_t* new_node = getNode(value);
    new_node->next = curr->next;
    curr->next = new_node;
}

// Remove from back and return removed element (or 0 if empty / error)
elem list_remove_from_back(list_t* l) {
    if (l == NULL || l->head == NULL) {
        return 0; // error, but we can't signal error cleanly – assume non-negative elem
    }
    if (l->head->next == NULL) {
        // Single node
        elem val = l->head->value;
        free(l->head);
        l->head = NULL;
        return val;
    }
    node_t* curr = l->head;
    while (curr->next->next != NULL) {
        curr = curr->next;
    }
    // curr points to second-to-last
    node_t* last = curr->next;
    elem val = last->value;
    free(last);
    curr->next = NULL;
    return val;
}

// Remove from front
elem list_remove_from_front(list_t* l) {
    if (l == NULL || l->head == NULL) {
        return 0;
    }
    node_t* old_head = l->head;
    elem val = old_head->value;
    l->head = old_head->next;
    free(old_head);
    return val;
}

// Remove at index (1-based)
elem list_remove_at_index(list_t* l, int index) {
    if (l == NULL || l->head == NULL || index < 1) {
        return 0;
    }
    if (index == 1) {
        return list_remove_from_front(l);
    }
    node_t* curr = l->head;
    int pos = 1;
    while (curr != NULL && pos < index - 1) {
        curr = curr->next;
        pos++;
    }
    if (curr == NULL || curr->next == NULL) {
        // index out of range
        return 0;
    }
    node_t* to_remove = curr->next;
    elem val = to_remove->value;
    curr->next = to_remove->next;
    free(to_remove);
    return val;
}

// Check if element exists
bool list_is_in(list_t* l, elem value) {
    if (l == NULL) return false;
    node_t* curr = l->head;
    while (curr != NULL) {
        if (curr->value == value) return true;
        curr = curr->next;
    }
    return false;
}

// Get element at index (1-based). Return -1 if not found (assuming non-negative elems)
elem list_get_elem_at(list_t* l, int index) {
    if (l == NULL || index < 1) return -1;
    node_t* curr = l->head;
    int pos = 1;
    while (curr != NULL && pos < index) {
        curr = curr->next;
        pos++;
    }
    if (curr == NULL) return -1; // not found
    return curr->value;
}

// Get index of element (1-based). Return -1 if not found.
int list_get_index_of(list_t* l, elem value) {
    if (l == NULL) return -1;
    node_t* curr = l->head;
    int pos = 1;
    while (curr != NULL) {
        if (curr->value == value) return pos;
        curr = curr->next;
        pos++;
    }
    return -1;
}