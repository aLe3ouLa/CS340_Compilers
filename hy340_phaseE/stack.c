#include "stack.h"


/*
    init the stack
*/
void initStack(stack* head)
{
    head = NULL;
}
 
/*
    push an element into stack
*/
stack* push(stack* head,int value)
{
    stack* newNode = malloc(sizeof(stack));
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
stack* pop(stack *head,int *element)
{
    stack* tmp = head;
    *element = head->value;
    head = head->next;
    free(tmp);
    return head;
}


/*
    returns 1 if the stack is empty, otherwise returns 0
*/
int emptyStack(stack* head)
{
    return head == NULL ? 1 : 0;
}
 
/*
    display the stack content
*/
void printStack(stack* head)
{
    stack *current = head;
    if(current!= NULL){
        printf("Stack: ");
        do
        {
            printf("%d ",current->value);
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

