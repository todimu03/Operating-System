// list.c
#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Helper: allocate a new node (heap)
node_t* getNode(elem value) {
    node_t* new_node = (node_t*)malloc(sizeof(node_t));
    if (!new_node) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    new_node->value = value;
    new_node->next = NULL;
    return new_node;
}

// Allocate an empty list (heap)
list_t* list_alloc() {
    list_t* l = (list_t*)malloc(sizeof(list_t));
    if (!l) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    l->head = NULL;
    return l;
}

// Free all nodes and the list itself
void list_free(list_t* l) {
    if (!l) return;
    node_t* curr = l->head;
    while (curr) {
        node_t* tmp = curr;
        curr = curr->next;
        free(tmp);
    }
    free(l);
}

// Print list in "100->90->...->NULL" format (matches the tests)
void list_print(list_t* l) {
    if (!l) {
        printf("NULL\n");
        return;
    }
    node_t* curr = l->head;
    while (curr) {
        printf("%d", curr->value);
        if (curr->next) printf("->");
        curr = curr->next;
    }
    printf("->NULL\n");
}

// Convert list to a heap‑allocated string like "100->90->...->NULL"
char* listToString(list_t* l) {
    if (!l || !l->head) {
        char* empty = (char*)malloc(5 * sizeof(char));
        if (empty) strcpy(empty, "NULL");
        return empty;
    }

    // First pass: compute total length needed
    int len = 0;
    node_t* curr = l->head;
    while (curr) {
        len += snprintf(NULL, 0, "%d", curr->value);
        len += 2;  // for "->"
        curr = curr->next;
    }
    // Replace the last "->" with "NULL" (4 chars) and add null terminator
    len += 4;  // for "NULL"
    char* str = (char*)malloc(len + 1);
    if (!str) return NULL;

    char* ptr = str;
    curr = l->head;
    while (curr) {
        ptr += sprintf(ptr, "%d", curr->value);
        if (curr->next) {
            ptr += sprintf(ptr, "->");
        } else {
            ptr += sprintf(ptr, "->NULL");
        }
        curr = curr->next;
    }
    *ptr = '\0';
    return str;
}

// List length
int list_length(list_t* l) {
    if (!l) return 0;
    int count = 0;
    node_t* curr = l->head;
    while (curr) {
        count++;
        curr = curr->next;
    }
    return count;
}

// Add to back
void list_add_to_back(list_t* l, elem value) {
    if (!l) return;
    node_t* new_node = getNode(value);
    if (!l->head) {
        l->head = new_node;
        return;
    }
    node_t* curr = l->head;
    while (curr->next) curr = curr->next;
    curr->next = new_node;
}

// Add to front
void list_add_to_front(list_t* l, elem value) {
    if (!l) return;
    node_t* new_node = getNode(value);
    new_node->next = l->head;
    l->head = new_node;
}

// Add at index (1‑based). If index is 1 -> front; if > length+1, ignore.
void list_add_at_index(list_t* l, elem value, int index) {
    if (!l || index < 1) return;
    if (index == 1) {
        list_add_to_front(l, value);
        return;
    }
    node_t* curr = l->head;
    int pos = 1;
    while (curr && pos < index - 1) {
        curr = curr->next;
        pos++;
    }
    if (!curr) return; // index out of range
    node_t* new_node = getNode(value);
    new_node->next = curr->next;
    curr->next = new_node;
}

// Remove from back and return element (0 if empty)
elem list_remove_from_back(list_t* l) {
    if (!l || !l->head) return 0;
    if (!l->head->next) {
        elem val = l->head->value;
        free(l->head);
        l->head = NULL;
        return val;
    }
    node_t* curr = l->head;
    while (curr->next->next) curr = curr->next;
    node_t* last = curr->next;
    elem val = last->value;
    free(last);
    curr->next = NULL;
    return val;
}

// Remove from front and return element (0 if empty)
elem list_remove_from_front(list_t* l) {
    if (!l || !l->head) return 0;
    node_t* old = l->head;
    elem val = old->value;
    l->head = old->next;
    free(old);
    return val;
}

// Remove at index (1‑based). Return element or 0 if invalid.
elem list_remove_at_index(list_t* l, int index) {
    if (!l || !l->head || index < 1) return 0;
    if (index == 1) return list_remove_from_front(l);

    node_t* curr = l->head;
    int pos = 1;
    while (curr && pos < index - 1) {
        curr = curr->next;
        pos++;
    }
    if (!curr || !curr->next) return 0; // invalid index
    node_t* to_remove = curr->next;
    elem val = to_remove->value;
    curr->next = to_remove->next;
    free(to_remove);
    return val;
}

// Check if element exists
bool list_is_in(list_t* l, elem value) {
    if (!l) return false;
    node_t* curr = l->head;
    while (curr) {
        if (curr->value == value) return true;
        curr = curr->next;
    }
    return false;
}

// Get element at 1‑based index; return -1 if out of range
elem list_get_elem_at(list_t* l, int index) {
    if (!l || index < 1) return -1;
    node_t* curr = l->head;
    int pos = 1;
    while (curr && pos < index) {
        curr = curr->next;
        pos++;
    }
    return curr ? curr->value : -1;
}

// Get 1‑based index of element; return -1 if not found
int list_get_index_of(list_t* l, elem value) {
    if (!l) return -1;
    node_t* curr = l->head;
    int pos = 1;
    while (curr) {
        if (curr->value == value) return pos;
        curr = curr->next;
        pos++;
    }
    return -1;
}