#ifndef DECISION_TREE_H_
#define DECISION_TREE_H_

#include "input.h"

// Node
typedef struct DecisionTreeNode {
  _Bool isLeaf;
  union {
    struct {
      struct DecisionTreeNode* left;  // For feature values <= the split
      struct DecisionTreeNode* right; // For feature values > the split
      int feature; // The feature to split on
      double split; // The value to split at
    } decision;

    // Leaf Node
    int class; // The classification
  } info;
} DecisionTreeNode;

void printNode(DecisionTreeNode* node);



// Tree
typedef struct DecisionTree {
  DecisionTreeNode* root;
} DecisionTree;

void verifyEntropy(Names* names);
DecisionTree* makeTree(Names* names);
int classify(DecisionTree* tree, Instance* instance);
double accuracy(DecisionTree* tree, Instance** instances, int numInstances);
void printTree(DecisionTreeNode* node, int n);
void freeTree(DecisionTreeNode* node);

#endif
