# c-decision-tree
Decision Tree written in C


Compile:'gcc readfile.c input.c decision_tree.c -lm'

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



PROPERTIES TO VERIFY
---------------------------
- If the training data contains only instances of the same classification X, then all instances in the testing data will be classified as X.
- During the generation of the decision tree, the number of instances passed to the recursive function call is always decreasing, therefore the decision tree always terminates.
- If in the training data, feature X is most discriminatory (i.e. results in the lowest entropy), X will be split on first in the learning of the tree.
- The children nodes of a parent node will have an expected entropy less than or equal to that of the parent.
- The learning of the decision tree will not introduce bias i.e. if all instances' classifications in the training data are independent of some feature X, then instances in the testing data will be classified independent of X.
- Features that aren't statistically significant will not be split on in the learning of the tree. (Not true yet; would need to implement chi-squared pruning).
- The generated decision tree is deterministic i.e. if the decision tree is given the same input twice, it will return the same classification both times.
- If two decision trees are generated on the same training data, the two trees will be identical i.e. they will both return the same classifications from a testing set.
- If there is no 'noise' in the training data, i.e. there are no two or more instances that have the sames values for every feature but different classifications, the decision tree will be 100% accurate on the training data it was generated with.
- Having two decision trees, a 'base' tree generated on a training set and tested on a testing set, and another tree that is generated and tested on the same sets as the 'base' tree, but has had a monotonic increasing function (i.e. a function that preserves order) to the feature values of both sets, the classifications the decision trees return shall be the same.
- If there is a relationship or function between two features in all instances in the training data...



TO DO
---------------------------
- Add more properties
- Code improvements
