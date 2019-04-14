#ifndef LINKEDLIST_H_INCLUDED
#define LINKEDLIST_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Truelist and Falselist lists*/ 
typedef struct listnode{	
	int quadlabel;
 	struct listnode* next;
}boolList;
 
int poplist (boolList ** list_ref);
boolList *exchangeTrueFalse (boolList* head1);
void makeList(boolList **head, int quadlabel);
boolList* mergelist (boolList *head1, boolList *head2);
void deleteboolList(boolList **head);

#endif






