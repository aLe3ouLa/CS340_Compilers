#ifndef __LIBFUNC_H__
#define __LIBFUNC_H__


#define MAXLIBFUN 12

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char * buffer;

char *libraryFunctions[]= { "print", "input", "objectmemberkeys", "objecttotalmembers", "objectcopy", "totalarguments", "argument",
								 "typeof", "strtonum", "sqrt", "cos", "sin" };
	

struct argNames {
	char * name;
	struct argNames *next;
};


void insert( struct argNames ** list, char *name){
		
	struct argNames *newNode, *temp;
	temp = *list;
	
	newNode = malloc (sizeof(struct argNames));
	
	newNode->name  = malloc (strlen(name)*sizeof(char));
	strcpy(newNode->name, name);
	
	if (temp == NULL){
	    *list = newNode;
	    newNode->next = NULL;
	}else{
		newNode->next = *list;
		*list = newNode;
	}	
	
}

void deleteList(struct argNames ** list){
	struct argNames * temp = *list;
	
	while (*list!=NULL){
		temp = *list;
		*list = (*list)->next;
		
		free(temp->name);
		free(temp);
	}
	
    *list =  NULL;

}
	
void print(struct argNames * list){
    
    struct argNames * temp = list;
	
	while (temp != NULL){
	        printf("Arg: %s\n", temp->name);
	        temp = temp->next;
	}
    
}
							 
								 
char *counterUnnamed(int unnamedNumber){
    
    char prefix[5] = "_func"; 
    char number[10];
	buffer = malloc (15 * sizeof(char));
    sprintf(number, "%d", unnamedNumber);
    
    strcat(buffer, prefix);
    strcat(buffer, number);
    
    return buffer;

}	
						 
								 
								 
								 
#endif