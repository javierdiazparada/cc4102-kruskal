#include <iostream>
#include <vector>
#include <chrono>

#include "../include/kruskal.h"
#include "union_find.hpp"

/**
 * @brief Estructura de resultado para el algoritmo de Kruskal
 * Contiene las aristas del árbol cobertor mínimo y el peso total
 */
struct ResultadoKruskal {
    std::vector<edge> aristas_mst;    ///< Aristas en el árbol cobertor mínimo
    double peso_total;                ///< Peso total del MST
    int num_nodos;                    ///< Número de nodos en el grafo
    
    ResultadoKruskal(int n) : peso_total(0.0), num_nodos(n) {
        aristas_mst.reserve(n - 1);  // MST tiene exactamente n-1 aristas
    }
};

/**
 * @brief Implementación del algoritmo de Kruskal
 * @param edge_extractor Puntero a estructura de extracción de aristas (heap o arreglo ordenado)
 * @param opti_path Si usar optimización path compression en Union-Find
 * @return Puntero a ResultadoKruskal conteniendo el MST
 */
void* kruskal(EdgeExtractor *edge_extractor, const bool opti_path)
{
    if (!edge_extractor || edge_extractor->size() == 0) {
        return nullptr;
    }
    
    // Calcular número de nodos a partir del número de aristas
    // Para grafo completo: num_aristas = n*(n-1)/2, entonces n = (1 + sqrt(1 + 8*num_aristas))/2
    size_t num_aristas = edge_extractor->size();
    int num_nodos = static_cast<int>((1 + std::sqrt(1 + 8.0 * num_aristas)) / 2.0);
    
    // Crear estructura de resultado
    ResultadoKruskal* resultado = new ResultadoKruskal(num_nodos);
    
    // Crear estructura Union-Find basada en flag de optimización
    IUnionFind* uf = nullptr;
    if (opti_path) {
        uf = new UnionFindOptimized(num_nodos);
    } else {
        uf = new UnionFindBasic(num_nodos);
    }
    
    // Procesar aristas en orden de peso creciente
    int aristas_agregadas = 0;
    const int aristas_objetivo = num_nodos - 1;  // MST tiene exactamente n-1 aristas
    
    while (aristas_agregadas < aristas_objetivo && edge_extractor->size() > 0) {
        // Extraer arista de peso mínimo
        edge arista_actual = edge_extractor->extract_min();
        
        // Verificar si agregar esta arista crearía un ciclo
        if (!uf->connected(arista_actual.u, arista_actual.v)) {
            // Agregar arista al MST
            uf->unite(arista_actual.u, arista_actual.v);
            resultado->aristas_mst.push_back(arista_actual);
            resultado->peso_total += arista_actual.weight;
            aristas_agregadas++;
        }
    }
    
    // Limpiar estructura Union-Find
    delete uf;
    
    return static_cast<void*>(resultado);
}

/**
 * @brief Función auxiliar para liberar memoria de ResultadoKruskal
 * @param result Puntero a ResultadoKruskal a liberar
 */
void free_kruskal_result(void* result) {
    if (result) {
        delete static_cast<ResultadoKruskal*>(result);
    }
}

/**
 * @brief Función auxiliar para obtener peso total del resultado
 * @param result Puntero a ResultadoKruskal
 * @return Peso total del MST
 */
double get_mst_weight(void* result) {
    if (!result) return 0.0;
    return static_cast<ResultadoKruskal*>(result)->peso_total;
}

/**
 * @brief Función auxiliar para obtener número de aristas en MST
 * @param result Puntero a ResultadoKruskal
 * @return Número de aristas en el MST
 */
int get_mst_edge_count(void* result) {
    if (!result) return 0;
    return static_cast<ResultadoKruskal*>(result)->aristas_mst.size();
}
