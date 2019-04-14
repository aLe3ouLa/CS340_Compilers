#ifndef VOIDSTACK_H_INCLUDED
#define VOIDSTACK_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// a common function used to free malloc'd objects
typedef void (*freeFunction)(void *);
 
typedef enum { F, T } bool_;
 
typedef bool_ (*stackIterator)(void *, int);
 
typedef struct _stackNode {
  int i;
  void *data;
  struct _stackNode *next;
} stackNode;
 
typedef struct {
  int logicalLength;
  int elementSize;
  stackNode *head;
  stackNode *tail;
  freeFunction freeFn;
} vstack_t;
 
void stack_new(vstack_t *st, int elementSize, freeFunction freeFn);
void stack_destroy(vstack_t *st);
 
void stack_prepend(vstack_t *st, void *element, int i);
void stack_append(vstack_t *st, void *element, int i);
int stack_size(vstack_t *st);
 
void stack_for_each(vstack_t *st, stackIterator iterator);
void stack_head(vstack_t *st, void *element, bool_ removeFromList);
void stack_tail(vstack_t *st, void *element);
 
#endif



