# C Projects

Included topics:

 * Data structures (BST, Hash Tables, Linked Lists, etc.)
 * Machine Learning

Written in C11.


## PA1 - Data Structures in C

<<<<<<< HEAD
* First - Linked list insertion and deletion. No duplicates. Print node count, then linked list.<br>
* Second - Array insertion sort. Print sorted array.<br>
* Third - Integer hash table. Print ["inserted', "present", "duplicate", "absent"].<br>
* Fourth - Calculate M^n of matrix M. Print resulting Matrix.<br>
* Fifth - Detect if a matrix is a magic square (size n*n, sum of every individual row, column, and diagonal is the same, no number repeats).<br>
* Sixth - Condense single command line argument string to a string of respective characters each followed by the numbers [1, infinity].<br>
* Seventh - Take the final character of each argument string and print it.<br>
* Eighth - Binary Search Tree w/ insertion and search functions only. Print "inserted"/"present" followed by height.<br>
* Ninth - Binary Search Tree w/ insertion, search, and deletion functions. print "inserted"/"present" followed by height, or "success" for successful deletion.<br>
=======
First - Linked list insertion and deletion. No duplicates. Print node count, then linked list.<br>
Second - Array insertion sort. Print sorted array.<br>
Third - Integer hash table. Print ["inserted', "present", "duplicate", "absent"].<br>
Fourth - Calculate M^n of matrix M. Print resulting Matrix.<br>
Fifth - Detect if a matrix is a magic square (size n*n, sum of every individual row, column, and diagonal is the same, no number repeats).<br>
Sixth - Condense single command line argument string to a string of respective characters each followed by the numbers [1, infinity].<br>
Seventh - Take the final character of each argument string and print it.<br>
Eighth - Binary Search Tree w/ insertion and search functions only. Print "inserted"/"present" followed by height.<br>
Ninth - Binary Search Tree w/ insertion, search, and deletion functions. print "inserted"/"present" followed by height, or "success" for successful deletion.<br>
>>>>>>> c6f6c4a7a5aeedf7c9228a940a6be558be765b50

## PA2 - "One-Shot" Machine Learning: House Prices

One-Shot Learning

As an introduction to Machine learning, this project implements a simple "one-shot" machine learning algorithm to predict prices of houses based off of current house prices in a given area.
The program takes two input files, one train-data file and one test-data file.
The train-data holds "weights" corresponding to attributes, like bedrooms, bathrooms, size, year, etc., of houses with known prices (which are also listed in the data).
The test-data holds weights corresponding to the same grouping of aforementioned attributes, but of houses with unknown prices.


The price of house y will be the result of each attribute multiplied by that attribute's weight, determined by our algorithm.<br>
<<<<<<< HEAD
    y = w0 + w1 * x1 + w2 * x2 + w3 * x3 + w4 * y4...<br>

In our training data of n examples, we will create/use a matrix of size n x (k+1) where k is the amount of attributes.
*	This is matrix X:<br>
*		1, x0,1, x0,2, x0,3, x0,4
        1, x1,1, x1,2, x1,3, x1,4
        1, x2,1, x2,2, x2,3, x2,4
        1, x3,1, x3,2, x3,3, x3,4
        ...
        1, xn,1, xn,2, xn,3, xn,4
*	Matrix Y will hold the house prices listed in the training data (final column). Size: n x 1
*		y0
        y1
        ...
        yn
*	Matrix W will hold the calculated weights. Size (k+1) x 1
*		w0
        w1
        ...
        wk
=======
	y = w0 + w1 * x1 + w2 * x2 + w3 * x3 + w4 * y4...<br>
    
In our training data of n examples, we will create/use a matrix of size n x (k+1) where k is the amount of attributes.
*	This is matrix X:<br>
*		1, x0,1, x0,2, x0,3, x0,4
		1, x1,1, x1,2, x1,3, x1,4
		1, x2,1, x2,2, x2,3, x2,4
		1, x3,1, x3,2, x3,3, x3,4
		...
		1, xn,1, xn,2, xn,3, xn,4
*	Matrix Y will hold the house prices listed in the training data (final column). Size: n x 1    
*		y0
		y1
		...
		yn
*	Matrix W will hold the calculated weights. Size (k+1) x 1
*		w0
		w1
		...
		wk
>>>>>>> c6f6c4a7a5aeedf7c9228a940a6be558be765b50

We can use the training data to find the weights using the following equation:

*		W = (XT * X)^-1 * XT * Y
>XT is X transposed, (XT * X)^-1 is the inverse of X transpose times XT

To get the prices of each house corresponding to the attributes in the test-data with m houses:<br>
*	Matrix Z will hold the house prices listed in the training data (final column). Size: m x 1<br>
<<<<<<< HEAD
*		z0,1, z0,2, z0,3, z0,4
        z1,1, z1,2, z1,3, z1,4
        z2,1, z2,2, z2,3, z2,4
        z3,1, z3,2, z3,3, z3,4
        ...
        zn,1, zn,2, zn,3, zn,4
*	To get each house price:
*		z = w0 + w1 * z1 + w2 * z2 + w3 * z3...wn * zn
=======
*		z0,1, z0,2, z0,3, z0,4<br>
		z1,1, z1,2, z1,3, z1,4<br>
		z2,1, z2,2, z2,3, z2,4<br>
		z3,1, z3,2, z3,3, z3,4<br>
		...<br>
		zn,1, zn,2, zn,3, zn,4<br>
*	To get each house price:<br>
*		z = w0 + w1 * z1 + w2 * z2 + w3 * z3...wn * zn<br>
>>>>>>> c6f6c4a7a5aeedf7c9228a940a6be558be765b50

Each house price will be printed on it's own line.
