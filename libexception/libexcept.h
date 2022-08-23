#ifndef __LIBEXCEPT_H__
#define __LIBEXCEPT_H__

#include <setjmp.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#ifndef __LIBEXCEPT_H_BUFFER__
#define __LIBEXCEPT_H_BUFFER__

jmp_buf		jmp_buffer;

#endif // __LIBEXCEPT_H_BUFFER__

#define EXCEPT_LIST_NODE_STATUS_NORMAL 0
#define EXCEPT_LIST_NODE_STATUS_FORMAT 1
#define EXCEPT_LIST_NODE_STATUS_HANDLED 2

struct s_except_list_node {
	s_except_list_node *next;
	char    *name;
	char    *message;
	void	*format_args;
	void    (*handler)(void *);
	int		signiture;
	int		state;
};

typedef s_except_list_node  libexcp_list_node;

struct s_except_context_storage {
	libexcp_list_node	*list;
	size_t      		size;
};

typedef s_except_context_storage libexcp_context_storage;

libexcp_context_storage* inilibexcp_context_storage();
int add_exception(libexcp_context_storage* strg, char *name, char *message);
int add_exception_func(libexcp_context_storage* strg, char *name, char *message, void (*handler)(void *));
int call_exception_handler(libexcp_context_storage* strg, char *name);
void delete_exception(libexcp_context_storage* strg, char *name);
void clear_except_context_storage(libexcp_context_storage* storage);
void destroy_except_context_storage(libexcp_context_storage* storage);

#endif // __LIBEXCEPT_H__
