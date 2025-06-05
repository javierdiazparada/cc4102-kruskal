# Compilador y variables
CXX = g++
EXECUTABLE = main.o
CXXFLAGS = -std=c++23 -pthread -O2 -Wall
LDLIBS = -pthread

# directorio incluws
INCLUDE_PATH = include/

compilate: $(EXECUTABLE)

execute: $(EXECUTABLE)
	./$(EXECUTABLE)

clean:
	rm -rf $(EXECUTABLE)

check:
	echo "Build, run and clean completed successfully!"

run: compilate execute clean check

$(EXECUTABLE): main.cpp src/kruskal.cpp
	$(CXX) $(CXXFLAGS) $^ -I$(INCLUDE_PATH) $(LDLIBS) -o $@


.PHONY: compilate execute clean run check
