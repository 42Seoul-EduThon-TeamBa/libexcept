#include "./libexcept.h"

// Function: __libexcept_init
// Description: Initialize the exception context storage
// Input: void
// Output: libexcp_context_storage* - pointer to the exception context storage
// Error return value: NULL
libexcp_context_storage* inilibexcp_context_storage(void) {
    libexcp_context_storage* ret = (libexcp_context_storage *)calloc(sizeof(libexcp_context_storage), 1);
    if (ret == NULL) { 
        return NULL;
    }
    ret->size = 0;
    return ret;
}

void free_except_list_nodes(libexcp_list_node* list) {
    if (list == NULL) {
        return;
    }
    libexcp_list_node* node = list;
    while (node != NULL) {
        libexcp_list_node* next = node->next;
        free(node->name);
        free(node->message);
        free(node);
        node = next;
    }
}

// Function: __libexcept_destroy
// Description: Destroy the exception context storage
// Input: libexcp_context_storage* - pointer to the exception context storage
// Output: void
// Error return value: void
void destroy_except_context_storage(libexcp_context_storage* storage) {
    if (storage == NULL) {
        return;
    }
    free_except_list_nodes(storage->list);
    free(storage->list);
    free(storage);
}

static libexcp_list_node *get_last_node(libexcp_context_storage * strg) {
    libexcp_list_node *nptr = strg->list;
    while (nptr) {
        nptr = nptr->next;
    }
    return nptr;
}

int add_exception(libexcp_context_storage* strg, char *name, char *message) {
    libexcp_list_node *last_nptr = get_last_node(strg);
    libexcp_list_node *new_nptr = (libexcp_list_node *)calloc(sizeof(libexcp_list_node), 1);
    if (new_nptr == NULL) {
        return -1;
    }
    new_nptr->name = strdup(name);
    new_nptr->message = strdup(message);
    if (new_nptr->name == NULL || new_nptr->message == NULL) {
        free(new_nptr->name);
        free(new_nptr->message);
        free(new_nptr);
        return -1;
    }
    new_nptr = strg->size + 1;
    new_nptr->state = EXCEPT_LIST_NODE_STATUS_NORMAL;
    last_nptr->next = new_nptr;
    return 0;
};

int add_exception_func(libexcp_context_storage* strg, char *name, char *message, void (*handler)(void *)) {
    libexcp_list_node *last_nptr = get_last_node(strg);
    libexcp_list_node *new_nptr = (libexcp_list_node *)calloc(sizeof(libexcp_list_node), 1);
    if (new_nptr == NULL) {
        return -1;
    }
    new_nptr->name = strdup(name);
    new_nptr->message = strdup(message);
    if (new_nptr->name == NULL || new_nptr->message == NULL) {
        free(new_nptr->name);
        free(new_nptr->message);
        free(new_nptr);
        return -1;
    }
    new_nptr = strg->size + 1;
    new_nptr->state = EXCEPT_LIST_NODE_STATUS_HANDLED;
    new_nptr->handler = handler;
    last_nptr->next = new_nptr;
    return 0;
};

int add_exception_formats(libexcp_context_storage* strg, char *name, char *message, ...) {
    libexcp_list_node *last_nptr = get_last_node(strg);
    libexcp_list_node *new_nptr = (libexcp_list_node *)calloc(sizeof(libexcp_list_node), 1);
    if (new_nptr == NULL) {
        return -1;
    }
    new_nptr->name = strdup(name);
    new_nptr->message = strdup(message);
    if (new_nptr->name == NULL || new_nptr->message == NULL) {
        free(new_nptr->name);
        free(new_nptr->message);
        free(new_nptr);
        return -1;
    }
    new_nptr = strg->size + 1;
    new_nptr->state = EXCEPT_LIST_NODE_STATUS_FORMAT;
    va_list args;
    va_start(args, message);
    new_nptr->formats = args;
    last_nptr->next = new_nptr;
    return 0;
    
};

int call_exception_handler(libexcp_context_storage* strg, char *name) {
    libexcp_list_node *nptr = strg->list;
    while (nptr) {
        if (strcmp(nptr->name, name) == 0) {
            if (nptr->state == EXCEPT_LIST_NODE_STATUS_HANDLED) {
                nptr->handler();
                return nptr->signiture;
            }
            if (nptr->state == EXCEPT_LIST_NODE_STATUS_FORMAT) {
                va_list args;
                va_copy(args, nptr->formats);
                printf(nptr->message, args);
                va_end(args);
                return nptr->signiture;
            }
            if (nptr->state == EXCEPT_LIST_NODE_STATUS_NORMAL) {
                printf(nptr->message);
                return nptr->signiture;
            }
            return 0;
        }
        nptr = nptr->next;
    }
    return -1;
};

void clear_except_context_storage(libexcp_context_storage* strg) {
    libexcp_list_node *nptr = strg->list;
    while (nptr) {
        free(nptr->name);
        free(nptr->message);
        free(nptr);
        nptr = nptr->next;
    }
};


void delete_exception(libexcp_context_storage* strg, char *name) {
    libexcp_list_node *nptr = strg->list;
    libexcp_list_node *prev = NULL;
    while (nptr) {
        if (strcmp(nptr->name, name) == 0) {
            if (prev == NULL) {
                strg->list = nptr->next;
            } else {
                prev->next = nptr->next;
            }
            free(nptr->name);
            free(nptr->message);
            free(nptr);
            return;
        }
        prev = nptr;
        nptr = nptr->next;
    }
};