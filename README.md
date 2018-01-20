# __**Computer Architecture Programs**__

Included topics:

 * Data structures (BST, Hash Tables, Linked Lists, etc.)
 * Machine Learning
 * Cache Simulation
 * Logic Gates Simulator

Written in C11.


## __PA1 - Data Structures in C__

First - Linked list insertion and deletion. No duplicates. Print node count, then linked list.<br>
Second - Array insertion sort. Print sorted array.<br>
Third - Integer hash table. Print ["inserted', "present", "duplicate", "absent"].<br>
Fourth - Calculate M^n of matrix M. Print resulting Matrix.<br>
Fifth - Detect if a matrix is a magic square (size n*n, sum of every individual row, column, and diagonal is the same, no number repeats).<br>
Sixth - Condense single command line argument string to a string of respective characters each followed by the numbers [1, infinity].<br>
Seventh - Take the final character of each argument string and print it.<br>
Eighth - Binary Search Tree w/ insertion and search functions only. Print "inserted"/"present" followed by height.<br>
Ninth - Binary Search Tree w/ insertion, search, and deletion functions. print "inserted"/"present" followed by height, or "success" for successful deletion.<br>

## __PA2 - "One-Shot" Machine Learning: House Prices__

As an introduction to Machine learning, this project implements a simple "one-shot" machine learning algorithm to predict prices of houses based off of current house prices in a given area.
The program takes two input files, one train-data file and one test-data file.
The train-data holds "weights" corresponding to attributes, like bedrooms, bathrooms, size, year, etc., of houses with known prices (which are also listed in the data).
The test-data holds weights corresponding to the same grouping of aforementioned attributes, but of houses with unknown prices.


The price of house y will be the result of each attribute multiplied by that attribute's weight, determined by our algorithm.

*		y = w0 + w1 * x1 + w2 * x2 + w3 * x3 + w4 * y4...<br>

In our training data of n examples, we will create/use a matrix of size n x (k+1) where k is the amount of attributes.
*	This is matrix X:

         1, x0,1, x0,2, x0,3, x0,4
        1, x1,1, x1,2, x1,3, x1,4
        1, x2,1, x2,2, x2,3, x2,4
        1, x3,1, x3,2, x3,3, x3,4
        ...
        1, xn,1, xn,2, xn,3, xn,4
*	Matrix Y will hold the house prices listed in the training data (final column). Size: n x 1

        y0
        y1
        ...
        yn
*	Matrix W will hold the calculated weights. Size (k+1) x 1

        w0
        w1
        ...
        wk

We can use the training data to find the weights using the following equation:

        W = (XT * X)^-1 * XT * Y
>XT is X transposed, (XT * X)^-1 is the inverse of X transpose times XT

To get the prices of each house corresponding to the attributes in the test-data with m houses:<br>
*	Matrix Z will hold the house prices listed in the training data (final column). Size: m x 1<br>

        z0,1, z0,2, z0,3, z0,4
        z1,1, z1,2, z1,3, z1,4
        z2,1, z2,2, z2,3, z2,4
        z3,1, z3,2, z3,3, z3,4
        ...
        zn,1, zn,2, zn,3, zn,4
*	To get each house price:

        z = w0 + w1 * z1 + w2 * z2 + w3 * z3...wn * zn

Each house price will be printed on it's own line.

## __PA3 - Cache Simulator__

This program simulates memory access traces of computer caches. The input is a memory access trace containing memory addresses and actions taken on the addresses.
This program simulates the running of these actions and determines if there is a hit, miss, and resulting actions to perform.
This program supports cache policies: FIFO and LRU, and can handle any cache associativity and size.

Format for trace file:

       "Program Counter": "Read (R) / Write (W)" "48-bit memory address"

        0x804be19: R 0x9cbad40
        0x804be19: W 0x9cbad40
        0x804be1c: R 0x9cbad44
        0x804be1c: W 0x9cbad44
        0x804be10: R 0xbf8ff498
        #eof

Usage interface:

    ./first <cache size> <associativity> <cache policy> <block size> <trace file>

<cache size>: size of cache in bytes, as a power of 2
<associativity>: "direct" for a direct mapped cache, "assoc" for a fully associative cache, or "assoc:n" for an n-way associative cache
<cache policy>: "fifo" for First-In-First-Out, or "lru" for Least Recently Used
<block size>: block size in bytes, as a power of 2
<tracefile>: name of trace file

Output Format:

The output prints out the reads, writes, hits and misses for a no-prefetch run and a pre-fetch run.

no-prefetch
Memory reads: 3498
Memory writes: 2862
Cache hits: 6502
Cache misses: 3498
with-prefetch
Memory reads: 3522
Memory writes: 2862
Cache hits: 8125
Cache misses: 1875

## __PA4 - Circuit Simulator__

Circuit Simulator

This program will cimulate digital logic gates and circuits. Included gates:

        NOT, AND, OR, NAND, NOR, XOR, DECODER, MULTIPLEXER

There is no limit to input and output counts.

#### Sample input file:

Each file consists of 3+ lines. The first line lists the inputs, the second line lists the outputs, and the following lines list the gates with their inputs and outputs. Gates do not have to be "in order" to function properly. The program dynamically builds the gate in memory and can execute appropriately.

INPUTS: Inputs are specified on a single line following the keyword INPUTVAR. All input variables will be identified with capital letters and digits. An identifier is comprised of at least one character (A-Z) followed by a series of digits (0-9) and/or characters (A-Z).

        Eg. IN1, IN2, VARA, VARB
OUTPUTS: Outputs are specified on a the next line following the keyword OUTPUTVAR. Output variables are simply identified by the string "OUT" followed by an integer. Outputs will be listed in consecutive order

        Eg. OUT1, OUT2, OUT3
Gates: The inputs and output are specified on each line. First, the name of the gate is listed (from above) in capital letters, followed by the selectors (if applicable), inputs, then outputs. Each gate  will produce an output that can be used as an input to another gate.

        NOT: This directive is followed by the name of one input, then the name of one output:
            NOT IN1 OUT1
        AND: This directive is followed by the name of two inputs, then the name of one output:
            AND IN1 IN2 OUT1
        OR: This directive  is followed by the name of two inputs, then the name of one output:
            OUT IN1 IN2 OUT1
        NOR: This directive  is followed by the name of two inputs, then the name of one output:
            NOR IN1 IN2 OUT1
        XOR: This directive  is followed by the name of two inputs, then the name of one output:
            XOR IN1 IN2 OUT1
        DECODER: This directive is followed by the number of inputs, then the names of the inputs, then the names of the outputs (ordered in graycode sequence):
            DECODER 2 IN1 IN2 OUT1 OUT2 OUT3 OUT4
        MULTIPLEXER: This directive is followed by the number of inputs, names of the inputs, names of the selectors, then the names the outputs. Inputs can be "1" or "0":
            MULTIPLEXER 4 0 0 1 0 IN1 IN2 OUT1

INPUTVAR 3 IN1 IN3 IN4
OUTPUTVAR 1 OUT1
MULTIPLEXER 4 1 0 1 0 IN3 IN4 temp1
MULTIPLEXER 2 temp1 1 IN1 OUT1

#### __Sample output file:__

The output file is the result of inputs being passed into the INPUTVAR variables in Grey Code order.

    0 0 0 1
    0 0 1 0
    0 1 1 1
    0 1 0 0
    1 1 0 1
    1 1 1 1
    1 0 1 1
    1 0 0 1

_Based on Rutgers University course 01:198:211_
