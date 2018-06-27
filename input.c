#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "input.h"

// Instance
// Prints out the instance's feature values and class
void printInstance(Instance* instance) {
  assert(instance != NULL);
  printf("Feature Values: ");
   for (int i = 0; i < instance->numFeatures; i++)
      printf("%lf ", instance->featureValues[i]);
    printf("Class: %d", instance->class);
}

// Frees the array inside of the instance, and then frees the instance itself
void freeInstance(Instance* instance) {
  free(instance->featureValues);
  free(instance);
}



// List
// Adds a node with the specified instance onto the front of the list (head) and assigns the new node to head
void push(InstanceListNode** head, Instance* instance) {
  assert(instance != NULL);
  InstanceListNode* new_node = (InstanceListNode*)malloc(sizeof(InstanceListNode));

  if (*head == NULL) {
    new_node->size = 1;
  } else {
    new_node->size = (*head)->size + 1;
  }
  
  new_node->instance = instance;
  new_node->next = *head;
  *head = new_node;
}

// Prints each instance in the list
void printList(InstanceListNode* head) {
  InstanceListNode* current = head;
  while (current){
    printInstance(current->instance);
    printf("\n");
    current = current->next;
  }
}

// Frees the nodes of the list, but not the instances the nodes point to
void freeList(InstanceListNode* head) {
  InstanceListNode* current = head;
  InstanceListNode* previous;
  while(current) {
    previous = current;
    current = current->next;
    free(previous);
  }
}

// Frees the nodes of the list and the instances they point to
void freeListAndInstances(InstanceListNode* head) {
  InstanceListNode* current = head;
  InstanceListNode* previous;
  while(current) {
    freeInstance(current->instance);
    previous = current;
    current = current->next;
    free(previous);
  }
}



// Names
// Prints out the input data (all classes, features, and instances)
void printNames(Names* names) {
  assert(names != NULL);
  printf("Classes\n");
  for (int i = 0; i < names->numClasses; i++)
    printf("%d ", i);
  printf("\n\n");

  printf("Features\n");
  for (int i = 0; i < names->numFeatures; i++)
    printf("%d ", i);
  printf("\n\n");

  printf("Instances\n");
  printList(names->instances);
}
