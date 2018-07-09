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
// Returns the class that appears the most in the array of instances
int majorityClass(Instance** instances, int numInstances, int numClasses){
  assert(instances != NULL);
  assert(numInstances > 0);
  assert(numClasses > 0);
  
  int classCount[numClasses];

  for (int i = 0; i < numClasses; i++)
    classCount[i] = 0;

  for (int i = 0; i < numInstances; i++)
    classCount[instances[i]->class]++;

  int majClass = -1;
  for (int i = 0; i < numClasses; i++)
    if (classCount[i] > classCount[majClass])
      majClass = i;

  return majClass;
}

// Returns 1 if all instances in the array have the same class, 0 if not
_Bool sameClass(Instance** instances, int numInstances){
  assert(instances != NULL);
  assert(numInstances > 0);
  
  int class = instances[0]->class;

  for (int i = 0; i < numInstances; i++)
    if (instances[i]->class != class)
      return 0;

  return 1;
}

// Returns 1 if all instances have the same values for all features, 0 if not
// Assumes sameClass has already been run on instances and returned 0 (i.e. not all the same class)
_Bool noisyData(Instances** instances, int numInstances, int numFeatures) {
  assert(instances != NULL);
  assert(numInstances > 0);
  assert(numFeatures > 0);
  
  double* initFeatureValues = instances[0]->featureValues; 

  for (int i = 0; i < numInstances; i++)
    for (int i = 0; j < numFeatures; i++)
      if (instances[i]->featureValues[j] != initFeatureValues[j])
	return 0;

  return 1;
}

// Returns the number of instances in the array that have the specified class
int classFrequency(Instance** instances, int numInstances, int class) {
  int freq = 0;

  for (int i = 0; i < numInstances; i++)
    if (instances[i]->class == class)
      freq++;

  return freq;
}

// Splits the given array of instances into a left array and a right array based on the given feature and split value
// Instances in the left array have a value that is less than or equal to the split for the specified feature
// Instances in the right aeeay have a value that is greater than the split for the specified feature
void split(Instance** instances, int numInstances, int feature, double split, Instance*** leftOut, int* numLeftOut, Instance*** rightOut, int* numRightOut) {

  int side[numInstances];
  int numLeft = 0;
  int numRight = 0

    for (int i = 0; i < numInstances; i++) {
      if (instances[i]->featureValues[feature] <= split) {
	side[i] = LEFT;
	numLeft++;
      } else {
	side[i] = RIGHT;
	numRight++;
      }
    }

  Instance** left = (Instance**)malloc(sizeof(Instance*) * numLeft);
  Instance** right = (Instance**)malloc(sizeof(Instance*) * numRight);

  int leftIndex = 0;
  int rightIndex = 0;
  for (int i = 0; i < numInstances; i++) {
    if (side[i] == LEFT) {
      left[leftIndex] = instances[i];
      leftIndex++;
    } else {
      right[rightIndex] = instances[i];
      rightIndex++;
    }
  }

  *leftOut = left;
  *numLeftOut = numLeft;
  *rightOut = right;
  *numRightOut = numRight;
}

// Entropy helper function
void numSplit(Instance** instances, int numInstances, int numClasses, int feature, double split, int* numLeftOut, double* infoLeftOut, int* numRightOut, double* infoRightOut) {
  assert(numClasses > 0);
  int numLeft = 0;
  int leftClassCount[numClasses];
  double infoLeft = 0.0;
  
  int numRight = 0;
  int rightClassCount[numClasses];
  double infoRight = 0.0;


  for (int i = 0; i < numInstances; i++) {
    if (instances[i]->featureValues[feature] <= split) {
      numLeft++;
      leftClassCount[instance[i]->class]++;
    } else {
      numRight++;
      rightClassCount[instance[i]->class]++;
    }
  }

  for (int i = 0; i < numClasses; i++) {
    if (numLeft != 0) {
      double dLeft = ((double) leftClassCount[i]) / ((double) numLeft);

      if (dLeft != 0)
	infoLeft += -dLeft * log2(dLeft);
    }

    if (numRight != 0){
      double dRight = ((double) rightClassCount[i]) / ((double) numRight);

      if (dRight != 0)
	infoRight += -dRight * log2(dRight);
    }
  }

  *numLeftOut = numLeft;
  *infoLeftOut = infoLeft;
  *numRightOut = numRight;
  *infoRightOut = infoRight;
}

// Returns the entropy of the array of instances split on the specified feature and split value
double calcEntropy(Instance** instances, int numInstances, int feature, double split, int numClasses) {
  assert(instances != NULL);
  assert(numInstances > 0);
  assert(numClasses > 0);
  double infoX = 0;

  int numLeft;
  double infoLeft;
  int numRight;
  double infoRight;

  numSplit(instances, numInstances, feature, split, &numLeft, &infoLeft, &numRight, &infoRight);

  infoX += (((double) numLeft) / numInstances) * infoLeft;
  infoX += (((double) numRight) / numInstances) * infoRight;

  assert(infoX >= 0);
  return infoX;
}

// Finds the feature and split value that minimize the entropy on the list of instances
// Changes the input parameters featureOut and splitOut to the best feature and split value
void findBestFeatureAndSplit(Instance** instances, int numInstances, int numFeatures, int numClasses, int* featureOut, double* splitOut) {
  assert(instances != NULL);
  assert(numFeatures > 0);
  assert(numClasses > 0);
  double minEntropy = -1;
  int bestFeature = 0;
  double bestSplit = 0.0;
  
  for (int i = 0; i < numFeatures; i++) {

    for (int j = 0; j < numInstances; j++) {

      double entropy = calcEntropy(instances, numInstances, i, instances[j]->featureValues[i], numClasses);
      if (minEntropy == -1 || entropy < minEntropy) {
	minEntropy = entropy;
	bestFeature = i;
	bestSplit = instances[j]->featureValues[i];
      }
    }
  }

  *featureOut = bestFeature;
  *splitOut = bestSplit;  
}

DecisionTreeNode* learn(Instance** instances, int numInstances, int numFeatures, int parentMajority, int numClasses) {
  assert(numFeatures > 0);
  assert(numClasses > 0);
  assert(parentMajority < numClasses && parentMajority >= 0);
  DecisionTreeNode* node = (DecisionTreeNode*)malloc(sizeof(DecisionTreeNode));

  if (instances == NULL) {
    node->class = parentMajority;
  } else if (sameClass(instances)) {
    node->class = instances->instance->class;
  } else if (noisyData(instances, numFeatures)) {
    node->class = majorityClass(instances, numClasses);
    printf("\nTHE DATA HAS SOME NOISE\n");
    printList(instances, numFeatures);
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

// Constructs a tree on the input data and returns a pointer to it
DecisionTree* makeTree(Names* names) {
  DecisionTree* tree = (DecisionTree*)malloc(sizeof(DecisionTree));
  tree->root = learn(names->instances, names->numInstances, names->numFeatures, majorityClass(names->instances, names->numInstances, names->numClasses), names->numClasses);
  return tree;
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
double accuracy(DecisionTree* tree, Instance** instances, int numInstances) {
  assert(tree != NULL);
  assert(instances != NULL);
  assert(numInstances > 0);
  double countCorrect = 0.0;

  for (int i = 0; i < numInstances; i++)
    if (classify(tree, instances[i]) == instances[i]->class)
      countCorrect++;
  
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
  
