# one-shot-learning
The program *one_shot* uses training data to learn weights for a set of house attributes and then applies those weights to a set of input data to calculate prices for those houses. *one_shot* takes two arguments: paths to the file containing the training data and the input data. 
ex: .\one_shot data\train.00.txt data\data.00.txt

Training data format:
Line 1. "train"
Line 2. integer *k*, representing the number of attributes
Line 3. integer *n*, representing the number of houses
Next *n* lines. *k*+1 floating-point numbers, with each line giving data for a house. The first *k* numbers will give values to the *x*_1...*x*_*k* attributes and the last number gives its price *y*. 

Input data format:
Line 1. "data"
Line 2. integer *k*, representing the number of attributes
Line 3. integer *m*, representing the number of houses
Next *m* lines. *k* floating-point numbers, with each line giving data for a house, not including its price. 

For example, if we had a house with *x*_1 bedrooms, *x*_2 bathrooms, *x*_3 square footage, and was built in the year *x*_4, then with weights *w*_k, *one_shot* estimates the price *y* in the following formula:

*y* = *w*_0 + *w*_1*x*_1 + *w*_2*x*_2 + *w*_3*x*_3 + *w*_4*x*_4

where *w*_0 is 1. 
