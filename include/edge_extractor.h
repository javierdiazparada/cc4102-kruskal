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
    
    virtual void insert_edge(double const & edge) = 0;
    virtual double extract_min() = 0;
    virtual const std::string get_name() = 0;
    virtual unsigned int size() = 0;
    virtual EdgeExtractor* clone() = 0;
    virtual ~EdgeExtractor() {}
};

class HeapMin : public EdgeExtractor
{
private:
    std::priority_queue<double, std::vector<double>, std::greater<double>>* min_priority_queue;

public:
    HeapMin(){min_priority_queue = new std::priority_queue<double, std::vector<double>, std::greater<double>>();}

    HeapMin(const HeapMin &other)
    {
        min_priority_queue = new std::priority_queue<double, std::vector<double>, std::greater<double>>(*other.min_priority_queue);
    }

    void insert_edge(double const &edge) override { min_priority_queue->push(edge); }

    double extract_min() override
    {
        double min_value = min_priority_queue->top();
        min_priority_queue->pop();
        return min_value;
    }

    const std::string get_name() override { return "HeapMin"; }

    unsigned int size() override { return min_priority_queue->size(); }

    bool operator==(const HeapMin &other) const { return are_priority_queues_equal<double, std::vector<double>, std::greater<double>>(*min_priority_queue, *other.min_priority_queue); }

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
    std::vector<double>* array;
    bool is_sorted = false;

public:
    ArraySort() { array = new std::vector<double>(); }

    ArraySort(ArraySort &other)
    {
        array = new std::vector<double>(*other.array);
        is_sorted = false; // Reiniciar el flag al clonar
    }

    void insert_edge(double const &edge) override
    {
        array->push_back(edge);
        is_sorted = false;
    }

    double extract_min() override
    {
        if (!is_sorted)
        {
            sort(array->begin(), array->end(), std::greater<double>()); // Sort the array in descending order
            is_sorted = true;
        }
        double min_value = array->back();
        array->pop_back();
        return min_value;
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
        return std::equal(array->begin(), array->end(), other.array->begin()); // Comparar los vectores de manera eficiente
    }

    ArraySort *clone() override
    {
        ArraySort *new_array = new ArraySort(*this);
        return new_array;
    }

    ~ArraySort() { delete array; }
};

#endif