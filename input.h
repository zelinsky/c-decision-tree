#ifndef INSTANCE_H_
#define INSTANCE_H_

// Instance
typedef struct Instance {
  int class;
  double* featureValues;
} Instance;

void printInstance(Instance* instance, int numFeatures);
void freeInstance(Instance* instance);



// List
typedef struct InstanceListNode {
  Instance* instance;
  int size; // The number of nodes to the right of this node (including this node)
  struct InstanceListNode* next;  
} InstanceListNode;

void push(InstanceListNode** head, Instance* instance);
void printList(InstanceListNode* head, int numFeatures);
void freeList(InstanceListNode* head);
void freeListAndInstances(InstanceListNode* head);



// Names
typedef struct Names { // Where all input data is held
  // All possible classifications
  int numClasses;

  // All features that the instances have
  int numFeatures;

  // All instances
  InstanceListNode* instances; // Linked list
} Names;

void printNames(Names* names);

#endif
