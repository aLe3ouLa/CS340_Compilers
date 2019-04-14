#include "list.h"
 
void makeList(boolList **head, int quadlabel) {
	boolList * temp = *head;
	boolList *newNode = NULL;
	
	newNode = malloc(sizeof(boolList));

	if (newNode == NULL){
		fprintf(stderr, "Cannot allocate the hash table.\n");
	   	exit(EXIT_FAILURE);
	}	
	
	newNode->quadlabel = quadlabel;

	if (temp == NULL){
		*head = newNode;
		newNode->next = NULL;
	}else{
		newNode->next = *head;
		*head = newNode;
	}
	
} 

 
int poplist (boolList **head){
    int item;
    boolList* current = *head;
    boolList* prev;
    if (current != NULL){
        item=current->quadlabel;
        prev = current->next;
        free(current);
        current=prev;
        *head=current;
        return item;
    }
    return 0;
}
 
void deleteboolList(boolList **head){
	boolList * temp = *head;
	
	while (*head!=NULL){
		temp = *head;
		*head = (*head)->next;
	
		free(temp);
	}
	
    *head =  NULL;

}
 
 
boolList* mergelist (boolList *head1, boolList *head2){
        while (head2 != NULL){
		makeList(&head1, head2->quadlabel);
                head2 = head2->next;
        }
        deleteboolList(&head2);
        return head1;
}
 
 
boolList *exchangeTrueFalse (boolList* head1){
        boolList *tmp=head1, *tmp2=NULL; 
        while (tmp!=NULL){
                makeList(&tmp2,tmp->quadlabel);
                tmp = tmp->next;
        }
        return  tmp2;
}
