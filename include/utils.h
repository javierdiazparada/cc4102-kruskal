#ifndef UTILS_H
#define UTILS_H

#include <fstream>
#include <iostream>
#include <mutex>
#include <random>

// Structs for the experiment
struct arg
{
    unsigned int n; // log2 size
};

struct node
{
    double x;
    double y;
};

struct datapoint
{
    unsigned int n; // log2 size
    std::string edge_extractor_name;
    double time_insertion;
    bool opti_path;
    double time_kruskal;
};

#endif