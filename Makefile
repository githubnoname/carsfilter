.PHONY: all build clean tests

CXX = g++ -std=c++11 -Wall -Wextra -pedantic -O2 -g
LINK = g++ -std=c++11

# bins sources
CARSFILTER_SRC = src/main.cpp src/carstorage.cpp src/cxfilereader.cpp

# tests sources
READERSTEST_SRC = src/cxfilereader.cpp src/cxstringreader.cpp src/readers_test.cpp
PARSERTEST_SRC = src/parser_test.cpp src/cxparser.cpp src/cxstringreader.cpp

TESTS = bin/test_readers bin/test_parser

noop =
space = $(noop) $(noop)


all: build

clean:
	rm -f objs/* bin/carsfilter

build: bin/carsfilter

bin/carsfilter: ${CARSFILTER_SRC:src/%.cpp=objs/%.d} ${CARSFILTER_SRC:src/%.cpp=objs/%.o}
	${LINK} ${CARSFILTER_SRC:src/%.cpp=objs/%.o} -o $@

bin/test_readers: ${READERSTEST_SRC:src/%.cpp=objs/%.d} ${READERSTEST_SRC:src/%.cpp=objs/%.o}
	${LINK} ${READERSTEST_SRC:src/%.cpp=objs/%.o} -o $@

bin/test_parser: ${PARSERTEST_SRC:src/%.cpp=objs/%.d} ${PARSERTEST_SRC:src/%.cpp=objs/%.o}
	${LINK} ${PARSERTEST_SRC:src/%.cpp=objs/%.o} -o $@

check: ${TESTS}
	$(subst $(space), && ,$(foreach x,${TESTS},./$(x)))


objs/%.d: src/%.cpp
	@g++ $< -MM -MT $@ > $@
	@g++ $< -MM -MT objs/$*.o >> $@

objs/%.o: src/%.cpp
	${CXX} -c $< -o $@

-include $(wildcard objs/*.d)
