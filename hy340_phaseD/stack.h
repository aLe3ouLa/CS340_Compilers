#ifndef LINKEDSTACK_H_INCLUDED
#define LINKEDSTACK_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* We want to be able to store various types so the value would be void* */ 
typedef struct node{	
	int value;
 	struct node* next;
}stack;
 
/* Initialize stack */
void initStack(stack *s);

/* Returns if the stack is empty*/
int emptyStack(stack *s);

/* Push an element into stack */
stack * push(stack *s,int value);

/* Pop an element from stack */
stack * pop(stack *s,int *value);

/* Prints stack elements*/
void printStack(stack *head);

#endif






