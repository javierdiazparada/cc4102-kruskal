#include "../include/edge_extractor.h"
#include <vector>
#include <iostream>
#include <random>
#include <algorithm>

size_t SIZE_TEST = 1000;

int main() {

    std::cout << "Test edges extractors..." << std::endl;
    // Create an instance of the EdgeExtractor class
    HeapMin heap_min = HeapMin();
    ArraySort array_sorter = ArraySort();
    std::vector<edge> edges;
    // Generate random edge values for testing purposes
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 10.0);
    std::uniform_int_distribution<> node_dis(0, 99);

    size_t contador_heap = 0;
    size_t contador_array = 0;

    // Insert random edge values into the extractor
    std::cout << "Test: Insertion extractors" << std::endl;
    for (int i = 0; i < SIZE_TEST; ++i)
    {
        double weight = dis(gen);
        int u = node_dis(gen);
        int v = node_dis(gen);
        edge e(u, v, weight);
        heap_min.insert_edge(e);
        array_sorter.insert_edge(e);
        edges.push_back(e);
    }

    if (heap_min.size() != edges.size())
    {
        std::cout << "Error HeapMin: The number of inserted edges does not match the size of the extractor." << std::endl;
    }
    if (array_sorter.size() != edges.size())
    {
        std::cout << "Error ArraySort: The number of inserted edges does not match the size of the extractor." << std::endl;
    }
    std::cout << "Test passed!" << std::endl;

    std::cout << "Test: Clone extractors" << std::endl;
    
    // Clone extractors
    HeapMin* heap_min_clone = heap_min.clone();
    ArraySort* array_sorter_clone = array_sorter.clone();

    if (!(*heap_min_clone == heap_min)){
        std::cout << "Error: Cloning HeapMin failed." << std::endl;
        std::cout << "HeapMin clone size: " << heap_min_clone->size() << std::endl;
        std::cout << "HeapMin size: " << heap_min.size() << std::endl;
        for (int i = 0; i < heap_min_clone->size(); ++i){
            if(heap_min_clone->extract_min() != heap_min.extract_min()){
                std::cout << "Error: Cloning HeapMin failed." << std::endl;
                break;
            }
        }
    }
    if (!(*array_sorter_clone == array_sorter)){
        std::cout << "Error: Cloning ArraySort failed." << std::endl;
        std::cout << "ArraySort clone size: " << array_sorter_clone->size() << std::endl;
        std::cout << "ArraySort size: " << array_sorter.size() << std::endl;
        for (int i = 0; i < array_sorter_clone->size(); ++i){
            if(array_sorter_clone->extract_min() != array_sorter.extract_min()){
                std::cout << "Error: Cloning ArraySort failed." << std::endl;
                break;
            }
        }
    }

    // Delete the cloned objects to avoid memory leaks
    delete heap_min_clone;
    delete array_sorter_clone;

    std::cout << "Test passed!" << std::endl;

    // Sort the edges using vector sort by weight
    std::sort(edges.begin(), edges.end(), [](const edge& a, const edge& b) {
        return a.weight < b.weight;
    });

    std::cout << "Test: Extracting edges" << std::endl;

    // Extract edges from the graph and store them in a vector
    for (int i = 0; i < SIZE_TEST; ++i)
    {
        edge extracted_edge_heap = heap_min.extract_min();
        edge extracted_edge_array_sort = array_sorter.extract_min();

        // Check if the extracted edges match the sorted order by weight
        if (std::abs(extracted_edge_heap.weight - edges[i].weight) < 1e-10)
        {
            contador_heap++;
        }
        if (std::abs(extracted_edge_array_sort.weight - edges[i].weight) < 1e-10)
        {
            contador_array++;
        }
    }
    std::cout << "Extraction correctly from HeapMin: " << (contador_heap / SIZE_TEST) * 100.0 << "%" << std::endl;
    std::cout << "Extraction correctly from ArraySort: " << (contador_array / SIZE_TEST) * 100.0 << "%" << std::endl;

    if ((contador_heap == contador_array) && (contador_heap == SIZE_TEST))
    {
        std::cout << "Test passed!" << std::endl;
    }

    return 0;
}
