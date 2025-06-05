#ifndef EDGE_EXTRACTOR_H
#define EDGE_EXTRACTOR_H

#include <iostream>
#include <queue>
#include <vector>
#include <string>
#include <algorithm>

#include "utils.h"

class EdgeExtractor
{
public:
    
    virtual void insert_edge(const edge& e) = 0;
    virtual edge extract_min() = 0;
    virtual const std::string get_name() = 0;
    virtual unsigned int size() = 0;
    virtual EdgeExtractor* clone() = 0;
    virtual ~EdgeExtractor() {}
};

class HeapMin : public EdgeExtractor
{
private:
    // Comparator for edge priority queue (min-heap by weight)
    struct EdgeComparator {
        bool operator()(const edge& a, const edge& b) const {
            return a.weight > b.weight; // Min-heap: smaller weights have higher priority
        }
    };
    
    std::priority_queue<edge, std::vector<edge>, EdgeComparator>* min_priority_queue;

public:
    HeapMin(){min_priority_queue = new std::priority_queue<edge, std::vector<edge>, EdgeComparator>();}

    HeapMin(const HeapMin &other)
    {
        min_priority_queue = new std::priority_queue<edge, std::vector<edge>, EdgeComparator>(*other.min_priority_queue);
    }

    void insert_edge(const edge& e) override { min_priority_queue->push(e); }

    edge extract_min() override
    {
        edge min_edge = min_priority_queue->top();
        min_priority_queue->pop();
        return min_edge;
    }

    const std::string get_name() override { return "HeapMin"; }

    unsigned int size() override { return min_priority_queue->size(); }

    bool operator==(const HeapMin &other) const { 
        // Simple size comparison for now
        return min_priority_queue->size() == other.min_priority_queue->size();
    }

    HeapMin *clone() override
    {
        HeapMin *new_heap = new HeapMin(*this);
        return new_heap;
    }

    ~HeapMin() { delete min_priority_queue; }
};

class ArraySort : public EdgeExtractor
{
private:
    std::vector<edge>* array;
    bool is_sorted = false;

public:
    ArraySort() { array = new std::vector<edge>(); }

    ArraySort(const ArraySort &other)
    {
        array = new std::vector<edge>(*other.array);
        is_sorted = false; // Reiniciar el flag al clonar
    }

    void insert_edge(const edge& e) override
    {
        array->push_back(e);
        is_sorted = false;
    }

    edge extract_min() override
    {
        if (!is_sorted)
        {
            // Sort the array in descending order by weight (so we can pop from back to get minimum)
            sort(array->begin(), array->end(), [](const edge& a, const edge& b) {
                return a.weight > b.weight;
            });
            is_sorted = true;
        }
        edge min_edge = array->back();
        array->pop_back();
        return min_edge;
    }

    const std::string get_name() override { return "ArraySort"; }

    unsigned int size() override { return array->size(); }

    bool operator==(const ArraySort &other) const { 
        if (is_sorted != other.is_sorted){
            std::cout << "Error: Owner of the object is_sorted = " << is_sorted << " and other object is_sorted = " << other.is_sorted  << "." << std::endl;
            return false;
        }
       if (array->size() != other.array->size()){
            std::cout << "Error: Owner of the object size = " << array->size() << " and other object size = " << other.array->size()  << "." << std::endl;
            return false;
        }
        // Simple comparison for now - could be enhanced to compare edge contents
        return array->size() == other.array->size();
    }

    ArraySort *clone() override
    {
        ArraySort *new_array = new ArraySort(*this);
        return new_array;
    }

    ~ArraySort() { delete array; }
};

#endif
