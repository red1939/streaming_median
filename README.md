# streaming_median

Toy example of two solutions of streaming (i.e. not provided at once as an array) median:
 * brute-force/naive which sorts array every time an element is added;
 * two-heaps that are interested in only keeping the max and min element sorted as these are the only elements that are needed for pivot calculation.

Space complexity:
 - brute force: n
 - optimized: n

Rough complexity estimates (time):
 - brute force: n * n * log(n) -> n^2*log(n) -> n^2
 - optimized: 3*log(n/2) -> log(n)
 
Note 1: I am not taking into considerations worst-case scenario for quick sort. Quick sort should only exhibit this behavior when the pivot is the smallest/biggest element, which won't happen as we are constantly choosing median.
 
Note 2: For binary tree, I assume that we will need to go through the whole tree when inserting, so I pessimize it to log(n). I also assume that every time we insert an element it requires balancing (think of highly skewed case where elements are sorted), thus we always need to: insert, erase and insert. As the sequence is always divided into two halves (+/- 1) we work on n/2 heaps.

Note 3: Calculation of median is constant time for both solutions, thus we can safely ignore it.
 
For both solutions space complexity is simply n (we need to store all elements).