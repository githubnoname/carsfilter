Carsfilter
==========

Test task about fitering and sorting data (cars). Consists from two main utilities: bin/carsfilter and bin/carsgen.

Requirements
------------

* C++11, was tested with gcc-4.8.5.
* Unix-like OS, was tested on Centos 7.

Build and check
---------------

     make && make check

Hack
----
If you want to research the code, tests are a good point to start. See `src/test_readere.cpp`, `src/test_parser.cpp`, `src/test_filters.cpp`, `src/test_sort.cpp`.

bin/carsgen
-----------
This utility could be used to generate cars.

     bin/carsgen -o cars100 -l 100
     bin/carsgen -o cars100k -l 100000

The initial set of cars templates is very limited. Feel free to hack it in `src/carsgen.cpp`.

bin/carsfilter
--------------

Utility reads cars from file or stdin, filter and sort them. Filtering is pretty fast, it process data line by line. Sorting needs to collect all data in memory. Since sorting works after filtering, usually it doesn't collect all input, but all filtered data. Utility uses intermediate representation for filters and sort rules, i.e. it calculates field position, comparison operation, and type conversion just once before data processing. If you are interested in how it works, see `src/axcmp.*`.
