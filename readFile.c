#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "decision_tree.h"
#include "input.h"

#define BUFFER_SIZE 256

int main(int argc, char* argv[]) {

  //OPEN FILES
  
  if (argc < 2) {
    printf("You must specify a training file.\n");
    return -1;
  }
  
  char const* const trainFileName = argv[1]; // Training data input file (MANDATORY)
  
  FILE* trainFile = fopen(trainFileName, "r");

  if (!trainFile) {
    printf("Training file '%s' not found.\n", trainFileName);
    return -1;
  }
  
  char* testFileName; // Testing data input file (OPTIONAL)
  FILE* testFile;
  if (argc > 2) {
    testFileName = argv[2];
    testFile = fopen(testFileName, "r");
    if (!testFile) {
      printf("Testing file '%s' not found.\n", testFileName);
      return -1;
    }
  }
  
  double d = 0; // Read numbers in file into here

  FILE* stream; // Convert each line into a stream
  
  char line[BUFFER_SIZE]; // Read each line in file into here
  int lineNumber = 1; // Keep track of the line we're on in the file

  Names* names =(Names*)malloc(sizeof(Names)); // Where all the input data will be stored
  names->numClasses = 0;
  names->numFeatures = 0;
  names->instances = NULL; // Linked list

  // Read each line
  while (fgets(line, sizeof(line), trainFile)) {

    stream = fmemopen(line, BUFFER_SIZE, "r"); // Convert to stream
    int index = 0; // Keep track of number of numbers read in

    double temp[BUFFER_SIZE]; // Temporary array until we know the total size of the line
    
    // Read each number
    while (fscanf(stream, "%lf,", &d)){
      temp[index] = d;
      index++;
    }

    // After the entire line is read
    
    if (lineNumber == 1){
      // Classes & Features
      names->numClasses = (int) temp[0];
      names->numFeatures = (int) temp[1];
      assert(names->numClasses > 0);
      assert(names->numFeatures > 0);
    } else {
      // Instances
      Instance* instance = (Instance*)malloc(sizeof(Instance)); // Make an instance
      instance->featureValues = (double*)malloc(names->numFeatures * sizeof(double));

      for (int i = 0; i < names->numFeatures; i++) // Copy data
	instance->featureValues[i] = temp[i];

      instance->class = (int) temp[names->numFeatures];
      assert(instance->class < names->numClasses && instance->class >= 0);

      push(&(names->instances), instance); // Add to front of linked list
    }

    lineNumber++;
  }

  // Print back out the data to make sure we read it in correctly
  printNames(names);

  // Construct and test the tree
  DecisionTree* tree = makeTree(names);
  printf("\nTree:\n");
  printTree(tree->root, 0);
  printf("\nAccuracy of tree on training data: %lf\n", accuracy(tree, names->instances));

  // TESTING DATA
  if (argc > 2) {
    InstanceListNode* head = NULL;

    // READ IN DATA
    // Read each line
    while (fgets(line, sizeof(line), testFile)) {

      stream = fmemopen(line, BUFFER_SIZE, "r"); // Convert to stream
      int index = 0; // Keep track of number of numbers read in
      double temp[BUFFER_SIZE]; // Temporary array to read data into
      
      // Read each number
      while (fscanf(stream, "%lf,", &d)){
	temp[index] = d;
	index++;
      }

      // Create an instance
      Instance* instance = (Instance*)malloc(sizeof(Instance)); // Make an instance
      instance->featureValues = (double*)malloc(names->numFeatures * sizeof(double));

      for (int i = 0; i < names->numFeatures; i++) // Copy data
	instance->featureValues[i] = temp[i];

      instance->class = (int) temp[names->numFeatures];
      assert(instance->class < names->numClasses && instance->class >= 0);

      push(&head, instance); // Add to list
    }

    // RUN DATA THROUGH TREE
    //printf("\nAccuracy of tree on testing data: %lf\n", accuracy(tree, head)); // Accuracy of whole list
    printf("\nTESTING DATA:\n"); // See tree classifications of individual instances
    InstanceListNode* current = head;
    while (current) {
      printInstance(current->instance, names->numFeatures);
      printf("\nTree classifies as %d\n\n", classify(tree, current->instance));
      current = current->next;
    }

    // Memory cleanup and closing streams for testing data
    freeListAndInstances(head);
    fclose(testFile);
  }
  
  // Memory cleanup
  freeListAndInstances(names->instances);
  free(names);
  freeTree(tree->root);

  // Close streams
  fclose(stream);
  fclose(trainFile);
  
  return 0;
}
  
