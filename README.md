# 20Newsgroups-Classifier

Naïve Bayes classifier and Boolean retrieval done on the 20Newsgroups dataset that has been written from scratch. Extremely lightweight and produces decent results. Also currently working on classification using word embedding's.

## Naive Bayes
A classifier based on the Bayes' theorem. It's a probabilistic model that does not take into consideration the relationships between features - they are considered to be independent from one another - and is called a __Naive__ classifier. It has been used extensively to categorize text documents into categories, using information such as word frequency and occurrences in the sample set as the features.

### Conditional Probability
The Naive Bayes model is based on conditional and total probabilities, where if you give a vector of features \x, a probability is assigned for all available k classes.

<p align="center">
  <img width="150" height="100" src="http://latex.codecogs.com/svg.latex?P%28C_%7Bk%7D%7Cx_%7B1%7D%2Cx_%7B2%7D%2C.....%2C_%7Bn%7D%29"><br>
</p>

Supervised learning method we introduce is the multinomial Naive Bayes or multinomial NB model, a probabilistic learning method. The probability of a document d being in class c is computed as

<p align="center">
  <img width="320" height="65" src="assets/NB.JPG"><br>
  <b>Multinomial Naive Bayes</b>
</p>

In text classification, our goal is to find the best class for the document. The best class in NB classification is the most likely or maximum a posteriori (MAP) class -

<p align="center">
  <img width="500" height="65" src="assets/Cmap.JPG"><br>
  <b>MAP</b>
</p>

Because of how small these values actually are, due to the monotonic nature of the logarithm function, we can simply rewrite the above equation and implement it as below -

<p align="center">
  <img width="500" height="65" src="assets/Cmap_log.JPG"><br>
  <b>Logarithmic MAP</b>
</p>

### Working

Needs to be added


Credits: text and equation borrowed from "Introduction to Information Retrieval", P.R. Raghavan et al.
