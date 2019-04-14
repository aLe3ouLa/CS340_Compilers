#ifndef VLINKEDLIST_H_INCLUDED
#define VLINKEDLIST_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Contlist and Breaklist lists*/ 
typedef struct vlistnode{	
	int quadLabel;
 	struct vlistnode* next;
}looplist;
 
//int poplist (looplist ** list_ref);
//looplist *exchangeTrueFalse (looplist* head1);
void makelooplist(looplist **head, int value);
void deletelooplist(looplist **head);

#endif






