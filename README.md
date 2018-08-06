# Program
Decision Tree written in C


Compile:'make'

Run:	'./a.out data/training-data-file data/testing-data-file'
- The training data file is mandatory
- The testing data file is optional

TRAINING DATA FILE FORMAT
----------------------------------------------------------------------------------------------------------------------
This file contains information about the instances and the instances themselves which are used to create the decision tree.

The first line specifies the number of possible classifications and the number of features all instances have.

Every line after that specifies a single instance. Each instance must have a numerical value for all the features
followed by its classification (an integer). The order of feature values must be the same across instances.

There can be no empty lines in the file, including at the end of the file.

If there are two possible classes, the values of those classes are 0 and 1.

If there are three possible classes, the values of those classes are 0, 1, and 2.

And so on.



Generic Format

#ofClasses (N), #ofFeatures (M)

feature0, feature1, ..., featureM-1, 0 <= class < N

feature0, feature1, ..., featureM-1, 0 <= class < N

....



Example

2, 3

2, 2.1, 30, 0

1, 4.5, 20, 0

1, 4.5, 10, 1

4, 1.1, 12, 0

3, 1.4, 15, 1


TESTING DATA FILE FORMAT
---------------------------
This file contains only instances to test on the decision tree that was created with the training data.

There should be no line specifying the number of classes and features, as it should be specified in the training file.

The instances should be formatted as specified previously.



Example

1, 2.3, 34, 0

2, 4.5, 42, 1

3, 4.1, 50, 1

# Verification

**You will need [CIVL](https://vsl.cis.udel.edu/civl/) to verify this program.**

Setup: You will need to edit the Makefile to include the path to the civl jar file on your computer.

Verification of the decision tree program is broken into two parts:
 1. Verification of the construction of the tree itself, with the function for picking the best feature abstracted away
 2. Verification of the function for picking the best feature

To verify the first part:   'make verify_tree'

To verify the second part:  'make verify_entropy'
