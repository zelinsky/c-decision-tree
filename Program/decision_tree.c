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
  
  if (!(node->isLeaf)) { 
    printf("Feature: %d Split: %lf\n", node->info.decision.feature, node->info.decision.split);
  } else {          // Leaf Node
    printf("Class: %d\n", node->info.class);
  }
}



// Tree

// Returns the class that appears the most in the array of instances
int majorityClass(Instance** instances, int numInstances, int numClasses){
  assert(instances != NULL);
  assert(numInstances > 0);
  assert(numClasses > 0);
  
  // array index corresponds to class value

  int classCount[numClasses];

  // initialize
  for (int i = 0; i < numClasses; i++)
    classCount[i] = 0;

  // count classes
  for (int i = 0; i < numInstances; i++)
    classCount[instances[i]->class]++;

  // pick the majority
  int majClass = 0;
  for (int i = 1; i < numClasses; i++)
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
_Bool noisyData(Instance** instances, int numInstances, int numFeatures) {
  assert(instances != NULL);
  assert(numInstances > 0);
  assert(numFeatures > 0);
  
  // the array of feature values to compare against
  double* initFeatureValues = instances[0]->featureValues; 
  
  // for each instance
  for (int i = 0; i < numInstances; i++)
    // for each feature
    for (int j = 0; j < numFeatures; j++)
      // compare
      if (instances[i]->featureValues[j] != initFeatureValues[j])
	return 0; // at least one different

  return 1; // all same
}

// Returns the number of instances in the array that have the specified class
int classFrequency(Instance** instances, int numInstances, int class) {
  int freq = 0;
 
  // for each instance
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
  int numRight = 0;

    // Assign each instance a side: left or right
    // and keep track of the count for each side
    for (int i = 0; i < numInstances; i++) {
      if (instances[i]->featureValues[feature] <= split) {
	side[i] = LEFT;
	numLeft++;
      } else {
	side[i] = RIGHT;
	numRight++;
      }
    }

  // Create arrays for the left and right sides
  Instance** left = (Instance**)malloc(sizeof(Instance*) * numLeft);
  Instance** right = (Instance**)malloc(sizeof(Instance*) * numRight);

  int leftIndex = 0;
  int rightIndex = 0;

  // Assign instances to the arrays based on their side
  for (int i = 0; i < numInstances; i++) {
    if (side[i] == LEFT) {
      left[leftIndex] = instances[i];
      leftIndex++;
    } else {
      right[rightIndex] = instances[i];
      rightIndex++;
    }
  }

  // Output
  *leftOut = left;
  *numLeftOut = numLeft;
  *rightOut = right;
  *numRightOut = numRight;
}

// calcEntropy helper function
// An implementation of the info function found in Properties.pdf
void info(Instance** instances, int numInstances, int numClasses, int feature, double split, int* numLeftOut, double* infoLeftOut, int* numRightOut, double* infoRightOut) {
  assert(numClasses > 0);

  // left
  int numLeft = 0;
  int leftClassCount[numClasses];
  double infoLeft = 0.0;
  
  // right
  int numRight = 0;
  int rightClassCount[numClasses];
  double infoRight = 0.0;

  // initialize
  for (int i = 0; i < numClasses; i++) {
    leftClassCount[i] = 0;
    rightClassCount[i] = 0;
  }

  // keep track the number of instances and their classes that
  // would end up on the left or right of the split value for the specified feature
  for (int i = 0; i < numInstances; i++) {
    if (instances[i]->featureValues[feature] <= split) {
      numLeft++;
      leftClassCount[instances[i]->class]++;
    } else {
      numRight++;
      rightClassCount[instances[i]->class]++;
    }
  }

  // calculation for each class
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

  // output
  *numLeftOut = numLeft;
  *infoLeftOut = infoLeft;
  *numRightOut = numRight;
  *infoRightOut = infoRight;
}

// Returns the entropy of the array of instances split on the specified feature and split value
// An implementation of the entropy/infox function found in Properties.pdf
double calcEntropy(Instance** instances, int numInstances, int feature, double split, int numClasses) {
  assert(instances != NULL);
  assert(numInstances > 0);
  assert(numClasses > 0);

  double entropy = 0;
 
  // left
  int numLeft = 0;
  double infoLeft = 0.0;

  // right
  int numRight = 0;
  double infoRight = 0.0;

  // helper function - an implementation of the info function found in Properties.pdf
  info(instances, numInstances, numClasses, feature, split, &numLeft, &infoLeft, &numRight, &infoRight);

  entropy += (((double) numLeft) / numInstances) * infoLeft;   // entropy of potential left node
  entropy += (((double) numRight) / numInstances) * infoRight;  // plus entropy of potential right node

  assert(entropy >= 0);
  return entropy;
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
  
  // for each feature
  for (int i = 0; i < numFeatures; i++) {

    // for each feature/split value
    for (int j = 0; j < numInstances; j++) {

      // calculate the expected entropy if we were to split at the current feature and split value
      double entropy = calcEntropy(instances, numInstances, i, instances[j]->featureValues[i], numClasses);

      // keep track of the feature and split value that result in the lowest entropy
      if (entropy < minEntropy || minEntropy == -1) {
	minEntropy = entropy;
	bestFeature = i;
	bestSplit = instances[j]->featureValues[i];
      }
    }
  }

  *featureOut = bestFeature;
  *splitOut = bestSplit;  
}

// Recursive function that creates a decision tree on the instances specified
// Initial function call will return a pointer to the root node
DecisionTreeNode* learn(Instance** instances, int numInstances, int numFeatures, int numClasses) {
  assert(numFeatures > 0);
  assert(numClasses > 0);

  // Create a node, it will either be:
  // - a decision node, where instances will be split on a feature and split value
  // - or a leaf node, where a class will be assigned to instances
  DecisionTreeNode* node = (DecisionTreeNode*)malloc(sizeof(DecisionTreeNode));

  if (sameClass(instances, numInstances)) {
    // leaf node
    // all instances have the same class, so choose that class as the class type for this leaf node
    node->isLeaf = 1;
    node->info.class = instances[0]->class;
  } else if (noisyData(instances, numInstances, numFeatures)) {
    // leaf node
    // instances have different classes, but all instances have the same values for all features,
    // so we cannot choose a feature and split value to split on
    // choose the most common class among the instances as the class type for this leaf node
    node->isLeaf = 1;
    node->info.class = majorityClass(instances, numInstances, numClasses);
    printf("\nTHE DATA HAS SOME NOISE\n");
    printInstances(instances, numInstances, numFeatures);
  } else {
    // decision node
    // find the best feature and split value to split on
    // then split the instances on those values
    node->isLeaf = 0;
    int bestFeature = 0;
    double bestSplit = 0.0;
    findBestFeatureAndSplit(instances, numInstances, numFeatures, numClasses, &bestFeature, &bestSplit);

    // assign node values
    node->info.decision.feature = bestFeature;
    node->info.decision.split = bestSplit;

    // left
    Instance** leftInstances = NULL;
    int numLeft = 0;

    // right
    Instance** rightInstances = NULL;
    int numRight = 0;

    split(instances, numInstances, bestFeature, bestSplit, &leftInstances, &numLeft, &rightInstances, &numRight);
    
    // recurse
    node->info.decision.left = learn(leftInstances, numLeft, numFeatures, numClasses);
    free(leftInstances);
    node->info.decision.right = learn(rightInstances, numRight, numFeatures, numClasses);
    free(rightInstances);
  }

  return node;
}

// Constructs a tree on the input data and returns a pointer to it
DecisionTree* makeTree(Names* names) {
  DecisionTree* tree = (DecisionTree*)malloc(sizeof(DecisionTree));
  tree->root = learn(names->instances, names->numInstances, names->numFeatures, names->numClasses);
  return tree;
}

// Returns the class that the tree gives for the instance
int classify(DecisionTree* tree, Instance* instance) {
  assert(tree != NULL);
  assert(instance != NULL);
  DecisionTreeNode* current = tree->root;

  // While the current node isn't a leaf node
  while (!(current->isLeaf)) {

    // Split on the feature and split value of the currentNode
    if (instance->featureValues[current->info.decision.feature] <= current->info.decision.split) {
      current = current->info.decision.left;
    } else {
      current = current->info.decision.right;
    }
  }
  
  // Return the leaf node's class
  return current->info.class;
}

// Classifies each instance in the list of instances with the given tree, and returns
// the ratio of correct classifications to the number of instances in the list
double accuracy(DecisionTree* tree, Instance** instances, int numInstances) {
  assert(tree != NULL);
  assert(instances != NULL);
  assert(numInstances > 0);
  int countCorrect = 0;

  for (int i = 0; i < numInstances; i++)
    if (classify(tree, instances[i]) == instances[i]->class)
      countCorrect++;
  
  return (double) countCorrect / (double) numInstances;
}

// Prints out the nodes of the tree in order
// Call the function with n=0
void printTree(DecisionTreeNode* node, int n) {
  assert(node != NULL);

  for (int i = 0; i < n; i++)
    printf("| ");
  printNode(node);
  
  if (!(node->isLeaf)) {
    printTree(node->info.decision.left, n+1);
    printTree(node->info.decision.right, n+1);
  }
}

// Frees the nodes of the tree
void freeTree(DecisionTreeNode* node) {
  if (!(node->isLeaf)) {
    freeTree(node->info.decision.left);
    freeTree(node->info.decision.right);
  }

  free(node);
}
  
