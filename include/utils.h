#ifndef UTILS_H
#define UTILS_H

#include <fstream>
#include <iostream>
#include <mutex>
#include <random>
#include <queue>
#include <cmath>

/**
 * @brief Estructura de argumentos para experimentos
 */
struct arg
{
    unsigned int n; ///< Tamaño como potencia de 2 (log2)
};

/**
 * @brief Representa un nodo en el plano bidimensional
 */
struct node
{
    double x; ///< Coordenada x en el rango [0,1]
    double y; ///< Coordenada y en el rango [0,1]
};

/**
 * @brief Representa una arista entre dos nodos
 */
struct edge
{
    int u, v;           ///< Índices de nodos conectados
    double weight;      ///< Peso de arista (distancia euclidiana al cuadrado)
    
    /**
     * @brief Constructor con parámetros
     * @param u Índice del primer nodo
     * @param v Índice del segundo nodo
     * @param weight Peso de la arista
     */
    edge(int u, int v, double weight) : u(u), v(v), weight(weight) {}
    
    /**
     * @brief Constructor por defecto
     */
    edge() : u(0), v(0), weight(0.0) {}
    
    /**
     * @brief Operador de igualdad
     * @param other Otra arista para comparar
     * @return true si las aristas son iguales
     */
    bool operator==(const edge& other) const {
        return u == other.u && v == other.v && std::abs(weight - other.weight) < 1e-10;
    }
    
    /**
     * @brief Operador de desigualdad
     * @param other Otra arista para comparar
     * @return true si las aristas son diferentes
     */
    bool operator!=(const edge& other) const {
        return !(*this == other);
    }
    
    /**
     * @brief Operador menor que (por peso)
     * @param other Otra arista para comparar
     * @return true si esta arista tiene menor peso
     */
    bool operator<(const edge& other) const {
        return weight < other.weight;
    }
};

/**
 * @brief Estructura para almacenar datos de un experimento
 */
struct datapoint
{
    unsigned long long n;           ///< Número de nodos en el experimento
    std::string edge_extractor_name; ///< Nombre del extractor usado ("ArraySort" o "HeapMin")
    double time_insertion;          ///< Tiempo de inserción de aristas (segundos)
    bool opti_path;                 ///< Si se usó optimización path compression
    double time_kruskal;            ///< Tiempo de ejecución de Kruskal (segundos)
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
