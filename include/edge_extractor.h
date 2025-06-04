#ifndef EDGE_EXTRACTOR_H
#define EDGE_EXTRACTOR_H

#include <queue>
#include <vector>
#include <string>
#include <algorithm>


class EdgeExtractor
{
public:
    
    virtual void insert_edge(double const & edge) = 0;
    virtual double extract_min() = 0;
    virtual const std::string get_name() = 0;
    virtual unsigned int size() = 0;
    virtual ~EdgeExtractor() {}
};

class HeapMin : public EdgeExtractor
{
private:
    std::priority_queue<double, std::vector<double>, std::greater<double>>* min_priority_queue;

public:
    HeapMin() {}

    HeapMin(const HeapMin& other) : min_priority_queue(other.min_priority_queue) {}

    void insert_edge(double const & edge) override;

    double extract_min() override;

    const std::string get_name() override;

    unsigned int size() override;

    ~HeapMin();
};

class ArraySort : public EdgeExtractor
{
private:
    std::vector<double>* array;
    bool is_sorted = false;

public:
    ArraySort() {}

    ArraySort(ArraySort& other) : array(other.array), is_sorted(false) {}

    void insert_edge(double const & edge) override;

    double extract_min() override;

    const std::string get_name() override;

    unsigned int size() override;

    ~ArraySort();
};

#endif