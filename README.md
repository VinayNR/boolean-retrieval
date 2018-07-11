# 20Newsgroups-Classifier

Naïve Bayes classifier and Boolean retrieval done on the 20Newsgroups dataset that has been written from scratch. Extremely lightweight and produces decent results. Also currently working on classification using word embedding's.

## Naive Bayes
A classifier based on the Bayes' theorem. It's a probabilistic model that does not take into consideration the relationships between features - they are considered to be independent from one another - and is called a __Naive__ classifier. It has been used extensively to categorize text documents into categories, using information such as word frequency and occurrences in the sample set as the features.

### Conditional Probability
The Naive Bayes model is based on conditional and total probabilities, where if you give a vector of features \x, a probability is assigned for all available k classes.

![img](http://latex.codecogs.com/svg.latex?P%28C_%7Bk%7D+%7C+x_%7B1%7D%2C+x_%7B2%7D%2C+.....%2C+x_%7Bn%7D%29)

But, for cases where the number of features are too high (n is too high), we adopt Bayes' theorem, to write the equation simply as -

![img](http://latex.codecogs.com/svg.latex?posterior+%3D+%28prior+%5Ctimes+likelihood%29+%2F+evidence)

The mathematical representation of the above formula can be seen below, which clearly seems to be based on Bayes' theorem, and hence Conditional probability.

![img](http://latex.codecogs.com/svg.latex?p%28C_%7Bk%7D%5Cmid+%5Cmathbf+%7Bx%7D+%29%3D%7B%5Cfrac+%7Bp%28C_%7Bk%7D%29%5C+p%28%5Cmathbf+%7Bx%7D+%5Cmid+C_%7Bk%7D%29%7D%7Bp%28%5Cmathbf+%7Bx%7D+%29%7D%7D)

Finally, to get the classifier from the Bayes' probabilistic model a function is written that assigns the particular sample the highest posteriori of all the possible k posterioris calculated. It is represented mathematically as below -

![img](http://latex.codecogs.com/svg.latex?%7B%5Chat+%7By%7D%7D%3D%7B%5Cunderset+%7Bk%5Cin+%5C%7B1%2C%5Cdots+%2CK%5C%7D%7D%7B%5Coperatorname+%7Bargmax%7D+%7D%7D%5C+p%28C_%7Bk%7D%29%5Cdisplaystyle+%5Cprod+_%7Bi%3D1%7D%5E%7Bn%7Dp%28x_%7Bi%7D%5Cmid+C_%7Bk%7D%29)

### Working

Needs to be added


Credits: Wikipedia.org for the equations
