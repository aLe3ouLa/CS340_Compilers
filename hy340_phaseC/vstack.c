#include "vstack.h"


/*
    init the stack
*/
void initVoidStack(void_stack *head)
{
    head = NULL;
}
 
/*
    push an element into stack
*/
void_stack * pushVoidStack(void_stack *head,void *value)
{
    void_stack* newNode = malloc(sizeof(void_stack));
    if (newNode == NULL){
        fprintf(stderr, "ERROR: Cannot allocate a new argument.\n");
        exit(EXIT_FAILURE);
    }
    /* Get the value and connect the node to stack */
    newNode->value = value;
    newNode->next = head;
    head = newNode;
    return head;
}


/*
    pop an element from the stack
*/
void_stack * popVoidStack(void_stack *head,void **element)
{
    void_stack* tmp = head;
    *element = head->value;
    head = head->next;
    free(tmp);
    return head;
}



/*
    returns 1 if the stack is empty, otherwise returns 0
*/
int emptyVoidStack(void_stack *head)
{
    return head == NULL ? 1 : 0;
}
 
/*
    display the stack content
*/
void printVoidStack(void_stack *head)
{
    void_stack *current = head;
    if(current!= NULL){
        printf("Stack: ");
        do
        {
            printf("%s ",(char*)current->value);
            current = current->next;
        }
        while (current!= NULL);
        printf("\n");
    }
    else
    {
        printf("The Stack is empty\n");
    }
 
}

