#include "vlist.h"
 
void makelooplist(looplist **head, int value) {
	looplist * temp = *head;
	looplist *newNode = NULL;
	
	newNode = malloc(sizeof(looplist));

	if (newNode == NULL){
		fprintf(stderr, "Cannot allocate the hash table.\n");
	   	exit(EXIT_FAILURE);
	}	
	
	newNode->quadLabel = value;

	if (temp == NULL){
		*head = newNode;
		newNode->next = NULL;
	}else{
		newNode->next = *head;
		*head = newNode;
	}
	
} 

 
int poplooplist (looplist **head){
    int item;
    looplist* current = *head;
    looplist* prev;
    if (current != NULL){
        item=current->quadLabel;
        prev = current->next;
        free(current);
        current=prev;
        *head=current;
        return item;
    }
    return 0;
}
 
void deletelooplist(looplist **head){
	looplist * temp = *head;
	
	while (*head!=NULL){
		temp = *head;
		*head = (*head)->next;
	
		free(temp);
	}
	
    *head =  NULL;

}
 
looplist* mergelooplist (looplist *head1, looplist *head2){
        while (head2 != NULL){
		makelooplist(&head1, head2->quadLabel);
                head2 = head2->next;
        }
        deletelooplist(&head2);
        return head1;
}

void printLoopList(looplist *head1){
	looplist * temp = head1;
	
	while (temp!= NULL){
		printf("quad: %d\n", temp->quadLabel);
		temp = temp->next;
	}
	
	
}
 
 /*
boolList *exchangeTrueFalse (boolList* head1){
        boolList *tmp=head1, *tmp2=NULL; 
        while (tmp!=NULL){
                makeList(&tmp2,tmp->quadlabel);
                tmp = tmp->next;
        }
        return  tmp2;
}*/
