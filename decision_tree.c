#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "decision_tree.h"
#include "input.h"

#define LEFT 0
#define RIGHT 1

// Node
// Prints out the feature and split value of the node or the class if it is a lead node
void printNode(DecisionTreeNode* node) {
  assert(node != NULL);
  if (node->class == -1) { 
    printf("Feature: %d Split: %lf\n", node->feature, node->split);
  } else {          // Leaf Node
    printf("Class: %d\n", node->class);
  }
}



// Tree
// Returns the class that appears the most in the list of instances
int majorityClass(InstanceListNode* instances, int numClasses){
  assert(instances != NULL);
  assert(numClasses > 0);
  int classCount[numClasses];

  for (int i = 0; i < numClasses; i++)
    classCount[i] = 0;

  InstanceListNode* current = instances;
  while (current) {
    classCount[current->instance->class]++;
    current = current->next;
  }

  int majClass = 0;
  for (int i = 0; i < numClasses; i++)
    if (classCount[i] > classCount[majClass])
      majClass = i;

  return majClass;
}

// Returns 1 if all instances in the list have the same class, 0 if not
char sameClass(InstanceListNode* instances){
  assert(instances != NULL);
  int class = instances->instance->class;
  char retval = 1;

  InstanceListNode* current = instances->next;

  while (current) {
    if (current->instance->class != class){
      retval = 0;
      break;
    }
    current = current->next;
  }

  return retval;
}

// Returns the number of instances in the list that have the specified class
int classFrequency(InstanceListNode* instance, int class) {
  int freq = 0;
  InstanceListNode* current = instance;

  while (current) {
    if (current->instance->class == class)
      freq++;
    current = current->next;
  }

  return freq;
}

// Returns a new list that contains only instances that have a value for the specified feature
// either less than or equal to (for side = LEFT/0) or greater than (for side = RIGHT/1) the specified split value
InstanceListNode* filterByFeatureValue(InstanceListNode* instances, int feature, double split, char side) {
  assert(feature < instances->instance->numFeatures && feature >= 0);
  assert(side == LEFT || side == RIGHT);
  InstanceListNode* newList = NULL;

  InstanceListNode* current = instances;

  if (side == LEFT) { 
    while (current) {
      if (current->instance->featureValues[feature] <= split)
	push(&newList, current->instance);
      current = current->next;
    }
  } else if (side == RIGHT) {
    while (current ) {
      if (current->instance->featureValues[feature] > split)
	push(&newList, current->instance);
      current = current->next;
    }
  }

  return newList;
}

// Returns the result of an entropy helper function on the class types of the list of instances
double info(InstanceListNode* instances, int numClasses) {
  assert(numClasses > 0);
  assert(instances != NULL);
  double info = 0.0;
  int numInstances = instances->size;


  for (int i = 0; i < numClasses; i++) {
    double d = ((double) classFrequency(instances, i)) / ((double) numInstances);
    if (d != 0)
      info += -d * log2(d);
  }

  assert(info >= 0);
  return info;
}

// Returns the entropy of the list of instances split on the specified feature and split value
double calcEntropy(InstanceListNode* instances, int feature, double split, int numClasses) {
  assert(instances != NULL);
  assert(feature < instances->instance->numFeatures && feature >=0);
  assert(numClasses > 0);
  double infoX = 0;
  int numTotalInstances = instances->size;

  InstanceListNode* leftList = filterByFeatureValue(instances, feature, split, LEFT);

  InstanceListNode* rightList = filterByFeatureValue(instances, feature, split, RIGHT);

  if (leftList)
    infoX += (((double) leftList->size) / numTotalInstances) * (info(leftList, numClasses));
  if (rightList)
    infoX += (((double) rightList->size) / numTotalInstances) * (info(rightList, numClasses));

  freeList(leftList);
  freeList(rightList);

  assert(infoX >= 0);
  return infoX;
}

// Finds the feature and split value that minimize the entropy on the list of instances
// Changes the input parameters featureOut and splitOut to the best feature and split value
void findBestFeatureAndSplit(InstanceListNode* instances, int numFeatures, int numClasses, int* featureOut, double* splitOut) {
  assert(instances != NULL);
  assert(numFeatures > 0);
  assert(numClasses > 0);
  double minEntropy = -1;
  int bestFeature = 0;
  double bestSplit = 0.0;
  
  for (int i = 0; i < numFeatures; i++) {

    InstanceListNode* current = instances;
    while (current) {

      double entropy = calcEntropy(instances, i, current->instance->featureValues[i], numClasses);
      if (minEntropy == -1 || entropy < minEntropy) {
	minEntropy = entropy;
	bestFeature = i;
	bestSplit = current->instance->featureValues[i];
      }

      current = current->next;
    }
  }

  *featureOut = bestFeature;
  *splitOut = bestSplit;  
}

DecisionTreeNode* learn(InstanceListNode* instances, int numFeatures, int parentMajority, int numClasses) {
  assert(numFeatures > 0);
  assert(numClasses > 0);
  assert(parentMajority < numClasses && parentMajority >= 0);
  DecisionTreeNode* node = (DecisionTreeNode*)malloc(sizeof(DecisionTreeNode));

  if (instances == NULL) {
    node->class = parentMajority;
  } else if (sameClass(instances) == 1) {
    node->class = instances->instance->class;
  } else {
    int bestFeature = 0;
    double bestSplit = 0.0;
    findBestFeatureAndSplit(instances, numFeatures, numClasses, &bestFeature, &bestSplit);
    node->feature = bestFeature;
    node->split = bestSplit;
    node->class = -1;

    InstanceListNode* leftList = filterByFeatureValue(instances, bestFeature, bestSplit, LEFT);
    InstanceListNode* rightList = filterByFeatureValue(instances, bestFeature, bestSplit, RIGHT);
    int majClass = majorityClass(instances, numClasses);
    
    node->left = learn(leftList, numFeatures, majClass, numClasses);
    freeList(leftList);
    node->right = learn(rightList, numFeatures, majClass, numClasses);
    freeList(rightList);
  }

  return node;
}

// Returns the class that the tree gives for the instance
int classify(DecisionTree* tree, Instance* instance) {
  assert(tree != NULL);
  assert(instance != NULL);
  DecisionTreeNode* current = tree->root;

  // While the current node isn't a leaf node
  while (current->class == -1) {

    // Split on the feature and split value of the currentNode
    if (instance->featureValues[current->feature] <= current->split) {
      current = current->left;
    } else {
      current = current->right;
    }
  }
  
  // Return the leaf node's class
  return current->class;
}

// Classifies each instance in the list of instances with the given tree, and returns
// the ratio of correct classifications to the number of instances in the list
double accuracy(DecisionTree* tree, InstanceListNode* instances) {
  assert(tree != NULL);
  assert(instances != NULL);
  int numInstances = instances->size;
  double countCorrect = 0.0;

  InstanceListNode* current = instances;
  while (current) {
    if (classify(tree, current->instance) == current->instance->class)
      countCorrect++;
    
    current = current->next;
  }
  
  return countCorrect/((double) numInstances);
}

// Prints out the nodes of the tree in order
// Call the function with n=0
void printTree(DecisionTreeNode* node, int n) {
  assert(node != NULL);

  for (int i = 0; i < n; i++)
    printf("| ");
  printNode(node);
  
  if (node->left)
    printTree(node->left, n+1);



  if (node->right)
    printTree(node->right, n+1);
}

// Frees the nodes of the tree
void freeTree(DecisionTreeNode* node) {
  if (node->left)
    freeTree(node->left);

  if (node->right)
    freeTree(node->right);

  free(node);
}
  
