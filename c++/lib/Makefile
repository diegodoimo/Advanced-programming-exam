CXX= c++
SRC=../src/bst.cpp
OBJ=$(SRC:.cpp=.o)

all: libbst.so

libbst.so: $(OBJ)
	$(CXX) -shared -o $@ $^

%.o:%.cpp
	$(CXX) -fpic -o $@ -c $< -Iinclude  

src/tree_exam.o: ../src/bst.h

clean:
	rm -rf $(EXE) *~

.PHONY: clean all
