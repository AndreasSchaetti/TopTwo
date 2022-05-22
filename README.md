# TopTwo
 Comparison of several solutions for finding the two largest integers in a vector of ints

# Sequential algorithms
Design decisions of the experiment

- Measurement variable: sum of the execution times over several random permutations of the input vector
- Comparison is done for a fixed number of permutations
- Use std::accumulate instead of std::reduce for simpler code
-- The binary function object that has to be passed to std::reduce is pretty involved. The lambda than is passed to std::algorithm is easy to understand

## Results
See results/comparison_of_algorithms.png

## Impact of input vector
In a separate analysis, I measured the execution time for each algorithm for 1'000 permutations. I adapted the size of the input vectors to the algorithm so that each measurement took longer than a few milliseconds. In this way, I hoped to get a reliable measurement of the execution time.

### Results
See results/analysis-timing-data-centered.png for the data centered around the median and results/analysis-timing-data.png for the original data.

# Parallel algorithms
TBA

# Environment
- WSL2 with Ubuntu 20.04
- Compiled with:
    - g++ 9.4
    - std=C++17
    - -O3
- Machine:
    - Intel i7-1185G7 @ 3 GHz
    - 16 GB RAM

# Credits
Inspired by Tyler Weaver (twitter handle: @squirrel428_) and the ADSP: the podcast by Bryce Adelstein Lelbach and Conor Hoekstra (twitter handle: @adspthepodcast).
