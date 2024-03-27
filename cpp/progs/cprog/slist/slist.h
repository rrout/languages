#include <stdio.h>
#include <stdlib.h>
#ifndef __SLIST_H__
#define __SLIST_H__
typedef struct _list {
	int data;
	struct _list *next;
}list_t;

list_t *insert(list_t *root, int data);
void printlist(list_t *root);


#endif //__SLIST_H__
