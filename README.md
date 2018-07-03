# c-decision-tree
Decision Tree written in C


Compile:'gcc readfile.c input.c decision_tree.c -lm'

Run:	'./a.out data/yourfilenamehere'


Data File Format
----------------------------------------------------------------------------------------------------------------------
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



PROPERTIES TO VERIFY
---------------------------
- If the training data contains only instances of the same classification X, then all instances in the testing data will be classified as X.
- The decision tree always terminates
- If in the training data, feature X is most discrimatory (i.e. resulsts in the lowest entropy), X will be split on first in the learning of the tree.
- The children nodes of a parent node will have an expected entropy less than or equal to that of the parent.
- The learning of the decision tree will not introduce bias i.e. if all instances' classifications in the training data are independent of some feature X, then instances in the testing data will be classified independent of X.
- Features that aren't statistiacally significant will not be split on in the learning of the tree. (Not true yet; would need to implement chi-squared pruning).
- If there is a relationship or function between two features in all instances in the training data...



TO DO
---------------------------
- Write up files explaining data
- Reading another file as testing data
