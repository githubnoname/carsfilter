Carsfilter
==========

Test task about fitering and sorting data (cars). Consists from two main utilities: `bin/carsfilter` and `bin/carsgen`.

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

Examples
--------

Filter all 'Lada' cars:

     bin/carsfilter -f cars100 --filter brand == Lada
     
Filter 'Lada' cars that was issued after 2000 year. And sort by year:

     bin/carsfilter -f cars100 --filter brand == Lada --filter issued '>' 2000 --asc issued
     
Filter only new cars, group result by brand (ascending sort), by year (descending sort), and finally ascending sort by price:

     bin/carsfilter -f cars100 --filter issued '>' 2010 --asc brand --desc issued --asc price
     
The resul of the last command will looks like the following (columns are brand, model, issued, price respectively):

     Lada;Kalina;2016;3519.140545
     Lada;Kalina;2016;7589.517699
     Lada;Kalina;2016;8108.238297
     Lada;Vesta;2015;1661.252030
     Lada;Granta;2014;3490.872472
     Lada;Kalina;2013;3978.035132
     Lada;Priora;2013;6453.417415
     Lada;Granta;2013;8673.840588
     Lada;Priora;2011;9657.466825
     Nissan;Cube;2016;36997.863669
     Nissan;Versa;2014;41999.144197
     Nissan;Altima;2013;23366.692108
     Nissan;Versa;2012;24469.029261
     Nissan;GT-R;2011;22656.491041
     Nissan;Altima;2011;38069.231178
