.PHONY: all build clean tests

CXX = g++ -std=c++11 -Wall -Wextra -pedantic -O2 -g
LINK = g++ -std=c++11

# bins sources
CARSFILTER_SRC = src/carsfilter.cpp src/cxfilereader.cpp src/cxparser.cpp src/axcmp.cpp src/cxpipefilter.cpp src/cxpipesorter.cpp
CARSGEN_SRC = src/carsgen.cpp src/cxgen.cpp src/cxrandom.cpp

# tests sources
READERSTEST_SRC = src/test_readers.cpp src/cxfilereader.cpp
PARSERTEST_SRC  = src/test_parser.cpp src/cxparser.cpp
FILTERSTEST_SRC = src/test_filters.cpp src/axcmp.cpp src/cxpipefilter.cpp
SORTTEST_SRC    = src/test_sort.cpp src/axcmp.cpp src/cxpipefilter.cpp src/cxpipesorter.cpp

TESTS = bin/test_readers bin/test_parser bin/test_filters bin/test_sort

noop =
space = $(noop) $(noop)


all: build

clean:
	rm -f objs/* bin/*

build: bin/carsfilter bin/carsgen

bin/carsfilter: ${CARSFILTER_SRC:src/%.cpp=objs/%.d} ${CARSFILTER_SRC:src/%.cpp=objs/%.o}
	${LINK} ${CARSFILTER_SRC:src/%.cpp=objs/%.o} -o $@

bin/carsgen: ${CARSGEN_SRC:src/%.cpp=objs/%.d} ${CARSGEN_SRC:src/%.cpp=objs/%.o}
	${LINK} ${CARSGEN_SRC:src/%.cpp=objs/%.o} -o $@

bin/test_readers: ${READERSTEST_SRC:src/%.cpp=objs/%.d} ${READERSTEST_SRC:src/%.cpp=objs/%.o}
	${LINK} ${READERSTEST_SRC:src/%.cpp=objs/%.o} -o $@

bin/test_parser: ${PARSERTEST_SRC:src/%.cpp=objs/%.d} ${PARSERTEST_SRC:src/%.cpp=objs/%.o}
	${LINK} ${PARSERTEST_SRC:src/%.cpp=objs/%.o} -o $@

bin/test_filters: ${FILTERSTEST_SRC:src/%.cpp=objs/%.d} ${FILTERSTEST_SRC:src/%.cpp=objs/%.o}
	${LINK} ${FILTERSTEST_SRC:src/%.cpp=objs/%.o} -o $@

bin/test_sort: ${SORTTEST_SRC:src/%.cpp=objs/%.d} ${SORTTEST_SRC:src/%.cpp=objs/%.o}
	${LINK} ${SORTTEST_SRC:src/%.cpp=objs/%.o} -o $@

check: ${TESTS}
	$(subst $(space), && ,$(foreach x,${TESTS},./$(x)))


objs/%.d: src/%.cpp
	@g++ -std=c++11 $< -MM -MT $@ > $@
	@g++ -std=c++11 $< -MM -MT objs/$*.o >> $@

objs/%.o: src/%.cpp
	${CXX} -c $< -o $@

-include $(wildcard objs/*.d)
