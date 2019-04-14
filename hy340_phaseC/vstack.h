#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* We want to be able to store various types so the value would be void* */ 
typedef struct void_s{	
	void *value;
 	struct void_s* next;
}void_stack;
 
/* Initialize stack */
void initVoidStack(void_stack *s);

/* Returns if the stack is empty*/
int emptyVoidStack(void_stack *s);

/* Push an element into stack */
void_stack * pushVoidStack(void_stack *s,void *value);

/* Pop an element from stack */
void_stack * popVoidStack(void_stack *s,void **value);

/* Prints stack elements*/
void printVoidStack(void_stack *head);

#endif


