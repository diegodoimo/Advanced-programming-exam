EXE = test.x
GDIR=lib

CXX= c++
CXXFLAGS= -Wall -Wextra -std=c++11 -I src #-03
LFLAGS= -lbst -L $(GDIR)

all: $(EXE)
%.o: %.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS)

$(EXE): test.o
	$(CXX) $^ -o $(EXE) $(LFLAGS)

clean:
	rm -rf $(EXE) *~

.PHONY: clean all
