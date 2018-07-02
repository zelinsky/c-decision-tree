#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "decision_tree.h"
#include "input.h"

#define BUFFER_SIZE 256

int main(int argc, char* argv[]) {
  char const* const fileName = argv[1]; // input file

  if (argc < 2) {
    printf("You must specify an input file.\n");
    return -1;
  }
  
  FILE* file = fopen(fileName, "r");

  if (!file) {
    printf("File '%s' not found.\n", argv[1]);
    return -1;
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
  while (fgets(line, sizeof(line), file)) {

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
      instance->numFeatures = names->numFeatures;

      for (int i = 0; i < instance->numFeatures; i++) // Copy data
	instance->featureValues[i] = temp[i];

      instance->class = (int) temp[instance->numFeatures];
      assert(instance->class < names->numClasses && instance->class >= 0);

      InstanceListNode** head = &(names->instances); // Add to front of linked list
      push(head, instance);
    }

    lineNumber++;
  }

  // Print back out the data to make sure we read it in correctly
  printNames(names);

  // Testing functions
  DecisionTree* tree = (DecisionTree*)malloc(sizeof(DecisionTree));
  tree->root = learn(names->instances, names->numFeatures, majorityClass(names->instances, names->numClasses), names->numClasses);
  printf("\nTree:\n");
  printTree(tree->root, 0);
  printf("\nAccuracy of tree: %lf\n", accuracy(tree, names->instances));
  
  // Close streams
  fclose(stream);
  fclose(file);

  // Memory cleanup
  freeListAndInstances(names->instances);
  free(names);
  freeTree(tree->root);
  
  return 0;
}
  
