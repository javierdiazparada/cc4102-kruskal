#include "../include/edge_extractor.h"
#include <vector>
#include <iostream>
#include <random>
#include <algorithm>

int main() {
    // Create an instance of the EdgeExtractor class
    HeapMin heap_min = HeapMin();
    ArraySort array_sorter = ArraySort();
    std::vector<double> edges;
    // Generate random edge values for testing purposes
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 10.0);
    // Insert random edge values into the extractor 
    for (int i = 0; i < 5; ++i) {
        double edge_value = dis(gen);
        heap_min.insert_edge(edge_value);
        array_sorter.insert_edge(edge_value);
        edges.push_back(edge_value);
        std::cout << "Inserted edge: " << edge_value << std::endl;
    }

    // Sort the edges using vector sort
    std::sort(edges.begin(), edges.end());

    // Extract edges from the graph and store them in a vector
    for (int i = 0; i < 5; ++i) {
        double extracted_edge_heap = heap_min.extract_min();
        double extracted_edge_array_sort = array_sorter.extract_min();
        std::cout << "Extracted edge HeapMin: " << extracted_edge_heap << std::endl;
        std::cout << "Extracted edge ArraySort: " << extracted_edge_array_sort << std::endl;
        std::cout << "Extracted edge vector sort: " << edges[i] << std::endl;
    }

    return 0;
}