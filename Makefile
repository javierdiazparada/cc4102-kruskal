# Compilador y variables
CXX = g++
MAIN_EXECUTABLE = main.o
TEST_COMPREHENSIVE = test_comprehensive.o
TEST_EDGE_EXTRACTOR = test_edge_extractor.o
TEST_QUICK = test_quick.o
CXXFLAGS = -std=c++23 -pthread -O2 -Wall
LDLIBS = -pthread

# directorio includes
INCLUDE_PATH = include/

# Target principal
compilate: $(MAIN_EXECUTABLE)

execute: $(MAIN_EXECUTABLE)
	./$(MAIN_EXECUTABLE)

# Ejecución segura con menos recursos
execute-safe: $(MAIN_EXECUTABLE)
	@echo "Ejecutando experimento principal con configuración segura..."
	@echo "Esto puede tomar 15-30 minutos. Presiona Ctrl+C para cancelar."
	./$(MAIN_EXECUTABLE)

clean:
	rm -rf $(MAIN_EXECUTABLE) $(TEST_COMPREHENSIVE) $(TEST_EDGE_EXTRACTOR) $(TEST_QUICK) *.csv

clean-results:
	rm -rf resultados/*.csv resultados/*.txt

check:
	echo "Build, run and clean completed successfully!"

# Target principal para experimentos
run: compilate execute-safe check
	@echo "Experimento completado. Resultados en resultados/df.csv"

# Compilar programa principal
$(MAIN_EXECUTABLE): main.cpp src/kruskal.cpp
	$(CXX) $(CXXFLAGS) $^ -I$(INCLUDE_PATH) $(LDLIBS) -o $@

# Compilar test comprehensivo
$(TEST_COMPREHENSIVE): test/test_comprehensive.cpp src/kruskal.cpp
	$(CXX) $(CXXFLAGS) $^ -I$(INCLUDE_PATH) $(LDLIBS) -o $@

# Compilar test de edge extractor (validación)
$(TEST_EDGE_EXTRACTOR): test/edge_extractor_validation.cpp
	$(CXX) $(CXXFLAGS) $^ -I$(INCLUDE_PATH) $(LDLIBS) -o $@

# Compilar test rápido
$(TEST_QUICK): test/test_quick.cpp src/kruskal.cpp
	$(CXX) $(CXXFLAGS) $^ -I$(INCLUDE_PATH) $(LDLIBS) -o $@

# Targets para tests
test-comprehensive: $(TEST_COMPREHENSIVE)
	./$(TEST_COMPREHENSIVE)

test-edge-extractor: $(TEST_EDGE_EXTRACTOR)
	./$(TEST_EDGE_EXTRACTOR)

test-quick: $(TEST_QUICK)
	./$(TEST_QUICK)

# Ejecutar todos los tests
test-all: test-edge-extractor test-quick test-comprehensive

# Target para experimentos completos (sin limpiar resultados)
experiment: $(TEST_COMPREHENSIVE)
	./$(TEST_COMPREHENSIVE)
	@echo "Resultados guardados en kruskal_results.csv"

.PHONY: compilate execute execute-safe clean clean-results run check test-comprehensive test-edge-extractor test-quick test-all experiment
