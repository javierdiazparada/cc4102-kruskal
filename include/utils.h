#ifndef UTILS_H
#define UTILS_H

#include <fstream>
#include <iostream>
#include <mutex>
#include <random>
#include <queue>
#include <cmath>

// Estructuras para el experimento
struct arg
{
    unsigned int n; // tamaño log2
};

struct node
{
    double x;
    double y;
};

struct edge
{
    int u, v;           // Índices de nodos
    double weight;      // Peso de arista (distancia euclidiana al cuadrado)
    
    // Constructor
    edge(int u, int v, double weight) : u(u), v(v), weight(weight) {}
    edge() : u(0), v(0), weight(0.0) {}
    
    // Comparison operators
    bool operator==(const edge& other) const {
        return u == other.u && v == other.v && std::abs(weight - other.weight) < 1e-10;
    }
    
    bool operator!=(const edge& other) const {
        return !(*this == other);
    }
    
    bool operator<(const edge& other) const {
        return weight < other.weight;
    }
};

struct datapoint
{
    unsigned long long n;
    std::string edge_extractor_name;
    double time_insertion;
    bool opti_path;
    double time_kruskal;
};

// Función de distancia
inline double distance(node a, node b)
{
    return std::pow(a.x - b.x, 2) + std::pow(a.y - b.y, 2);
}


// Function to compare two proirity queues
template <typename T, typename Container = std::vector<T>, typename Comparator>
bool are_priority_queues_equal(std::priority_queue<T, Container, Comparator> &q1,
                               std::priority_queue<T, Container, Comparator> &q2)
{
    if (q1.size() != q2.size())
    {
        return false;
    }

    std::vector<T> elements_q1, elements_q2;

    // Extraer elementos de la primera cola
    while (!q1.empty())
    {
        elements_q1.push_back(q1.top());
        q1.pop();
    }

    // Extraer elementos de la segunda cola
    while (!q2.empty())
    {
        elements_q2.push_back(q2.top());
        q2.pop();
    }

    // Comparar los vectores
    if (elements_q1 != elements_q2)
    {
        return false;
    }

    // Reinsertar elementos en las colas originales
    for (const auto &elem : elements_q1)
    {
        q1.push(elem);
    }
    for (const auto &elem : elements_q2)
    {
        q2.push(elem);
    }

    return true;
}

#endif
