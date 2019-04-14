#include <stdlib.h>
#include <string.h>
#include <assert.h>
 
#include "vstack.h"


 
void stack_new(vstack_t *st, int elementSize, freeFunction freeFn){
  assert(elementSize > 0);
  st->logicalLength = 0;
  st->elementSize = elementSize;
  st->head = st->tail = NULL;
  st->freeFn = freeFn;
}
 
void stack_destroy(vstack_t *st){
  stackNode *current;
  while(st->head != NULL) {
    current = st->head;
    st->head = current->next;
 
    if(st->freeFn) {
      st->freeFn(current->data);
    }
 
    free(current->data);
    free(current);
  }
}
 
void stack_prepend(vstack_t *st, void *element, int i){
  stackNode *node = malloc(sizeof(stackNode));
  node->data = malloc(st->elementSize);
  memcpy(node->data, element, st->elementSize);
 
  node->i = i;
  node->next = st->head;
  st->head = node;
 
  // first node?
  if(!st->tail) {
    st->tail = st->head;
  }
 
  st->logicalLength++;
}
 
void stack_append(vstack_t *st, void *element, int i){
  stackNode *node = malloc(sizeof(stackNode));
  node->data = malloc(st->elementSize);
  node->next = NULL;
 
  memcpy(node->data, element, st->elementSize);
  node->i = i;
 
  if(st->logicalLength == 0) {
    st->head = st->tail = node;
  } else {
    st->tail->next = node;
    st->tail = node;
  }
 
  st->logicalLength++;
}
 
void stack_for_each(vstack_t *st, stackIterator iterator){
  assert(iterator != NULL);
 
  stackNode *node = st->head;
  bool_ result = T;
  while(node != NULL && result) {
    result = iterator(node->data, node->i);
    node = node->next;
  }
}
 
void stack_head(vstack_t *st, void *element, bool_ removeFromList){
  assert(st->head != NULL);
 
  stackNode *node = st->head;
  memcpy(element, node->data, st->elementSize);
 
  if(removeFromList) {
    st->head = node->next;
    st->logicalLength--;
 
    free(node->data);
    free(node);
  }
}
 
void stack_tail(vstack_t *st, void *element){
  assert(st->tail != NULL);
  stackNode *node = st->tail;
  memcpy(element, node->data, st->elementSize);
}
 
int stack_size(vstack_t *st){
  return st->logicalLength;
}
