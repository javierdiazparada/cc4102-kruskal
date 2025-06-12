#ifndef EDGE_EXTRACTOR_H
#define EDGE_EXTRACTOR_H

#include <iostream>
#include <queue>
#include <vector>
#include <string>
#include <algorithm>

#include "utils.h"

/**
 * @brief Interfaz abstracta para estructuras de extracción de aristas
 * Permite intercambiar entre diferentes implementaciones (heap, array ordenado)
 */
class EdgeExtractor
{
public:
    /**
     * @brief Inserta una arista en la estructura
     * @param e Arista a insertar
     */
    virtual void insert_edge(const edge& e) = 0;
    
    /**
     * @brief Extrae y retorna la arista de peso mínimo
     * @return Arista con el menor peso
     */
    virtual edge extract_min() = 0;
    
    /**
     * @brief Obtiene el nombre de la implementación
     * @return Nombre del extractor ("HeapMin" o "ArraySort")
     */
    virtual const std::string get_name() = 0;
    
    /**
     * @brief Obtiene el número de aristas en la estructura
     * @return Número de aristas almacenadas
     */
    virtual unsigned int size() = 0;
    
    /**
     * @brief Crea una copia profunda del extractor
     * @return Puntero a nueva instancia clonada
     */
    virtual EdgeExtractor* clone() = 0;
    
    virtual ~EdgeExtractor() {}
};

/**
 * @brief Implementación usando min-heap (priority queue)
 * Mantiene las aristas en un heap mínimo para extracción eficiente
 */
class HeapMin : public EdgeExtractor
{
private:
    /**
     * @brief Comparador para el min-heap por peso de arista
     */
    struct EdgeComparator {
        bool operator()(const edge& a, const edge& b) const {
            return a.weight > b.weight; // Min-heap: menor peso = mayor prioridad
        }
    };
    
    std::priority_queue<edge, std::vector<edge>, EdgeComparator>* min_priority_queue;

public:
    /**
     * @brief Constructor por defecto
     */
    HeapMin(){min_priority_queue = new std::priority_queue<edge, std::vector<edge>, EdgeComparator>();}

    /**
     * @brief Constructor de copia
     * @param other Instancia a copiar
     */
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
        // Comparación simple por tamaño
        return min_priority_queue->size() == other.min_priority_queue->size();
    }

    HeapMin *clone() override
    {
        HeapMin *new_heap = new HeapMin(*this);
        return new_heap;
    }

    ~HeapMin() { delete min_priority_queue; }
};

/**
 * @brief Implementación usando array que se ordena al primer extract_min()
 * Almacena aristas en vector y las ordena cuando se necesita extraer el mínimo
 */
class ArraySort : public EdgeExtractor
{
private:
    std::vector<edge>* array;  ///< Vector que almacena las aristas
    bool is_sorted = false;    ///< Flag que indica si el array está ordenado

public:
    /**
     * @brief Constructor por defecto
     */
    ArraySort() { array = new std::vector<edge>(); }

    /**
     * @brief Constructor de copia
     * @param other Instancia a copiar
     */
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
            // Ordenar en orden descendente para extraer mínimos desde el final
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
        // Comparación simple por tamaño
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
