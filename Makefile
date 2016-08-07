.PHONY: all build clean

CARSFILTER_SRC = src/main.cpp src/carstorage.cpp
CXX = g++ -std=c++11 -Wall -Wextra -pedantic -O2 -g
LINK = g++ -std=c++11

all: build

clean:
	rm -f objs/* bin/carsfilter

build: bin/carsfilter

bin/carsfilter: ${CARSFILTER_SRC:src/%.cpp=objs/%.d} ${CARSFILTER_SRC:src/%.cpp=objs/%.o}
	${LINK} ${CARSFILTER_SRC:src/%.cpp=objs/%.o} -o $@


objs/%.d: src/%.cpp
	@g++ $< -MM -MT $@ > $@
	@g++ $< -MM -MT objs/$*.o >> $@

objs/%.o: src/%.cpp
	${CXX} -c $< -o $@

-include $(wildcard objs/*.d)
