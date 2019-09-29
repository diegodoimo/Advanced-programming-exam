CXX=clang++
CXXFLAGS= -Wall -Wextra -std=c++11 -Isrc -O3

SRC=test.cpp
EXE=$(SRC:.cpp=.x)

all: $(EXE)

%x:%cpp
	$(CXX) $< -o $@ $(CXXFLAGS)

clean:
	rm -rf $(EXE) *~

.PHONY: clean all
