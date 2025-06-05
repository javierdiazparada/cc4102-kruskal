#ifndef KRUSKAL_H
#define KRUSKAL_H

#include "edge_extractor.h"

/**
 * @brief Implementación del algoritmo de Kruskal
 * @param extractor Puntero a estructura de extracción de aristas (heap o arreglo ordenado)
 * @param opti_path Si usar optimización path compression en Union-Find
 * @return Puntero a ResultadoKruskal conteniendo el MST
 */
void* kruskal(EdgeExtractor *extractor, const bool opti_path);

/**
 * @brief Función auxiliar para liberar memoria de ResultadoKruskal
 * @param result Puntero a ResultadoKruskal a liberar
 */
void free_kruskal_result(void* result);

/**
 * @brief Función auxiliar para obtener peso total del resultado
 * @param result Puntero a ResultadoKruskal
 * @return Peso total del MST
 */
double get_mst_weight(void* result);

/**
 * @brief Función auxiliar para obtener número de aristas en MST
 * @param result Puntero a ResultadoKruskal
 * @return Número de aristas en el MST
 */
int get_mst_edge_count(void* result);

#endif
