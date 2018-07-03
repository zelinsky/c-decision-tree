# c-decision-tree
Decision Tree written in C


Compile:'gcc readfile.c input.c decision_tree.c -lm'

Run:	'./a.out data/yourfilenamehere'


Data File Format
----------------------------------------------------------------------------------------------------------------------
The first line specifies the number of possible classifications and the number of features all instances have.

Every line after that specifies a single instance. Each instance must have a value for all the features
followed by its classification. The order of feature values must be the same across instances.

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



TO DO
---------------------------
- Write up files explaining data
- Reading another file as testing data
