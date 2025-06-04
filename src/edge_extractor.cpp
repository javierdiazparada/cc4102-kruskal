#include "../include/edge_extractor.h"

// HeapMin Implementation
void HeapMin::insert_edge(double const &edge){ min_priority_queue->push(edge); }

double HeapMin::extract_min() 
{
    double min_value = min_priority_queue->top();
    min_priority_queue->pop();
    return min_value;
}

const std::string HeapMin::get_name() {return "HeapMin";}

unsigned int HeapMin::size() { return min_priority_queue->size(); }

HeapMin::~HeapMin() { delete min_priority_queue; }


// ArraySort Implementation
void ArraySort::insert_edge(double const &edge)
{
    array->push_back(edge);
    is_sorted = false;
}

double ArraySort::extract_min()
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

const std::string ArraySort::get_name(){ return "ArraySort"; }

unsigned int ArraySort::size() { return array->size(); }

ArraySort::~ArraySort() { delete array; }
