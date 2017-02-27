# streaming_median

Toy example of two solutions of streaming (i.e. not provided at once as an array) median:
 * brute-force/naive which sorts array every time an element is added;
 * two-heaps that are interested in only keeping the max and min element sorted as these are the only elements that are needed for pivot calculation.
