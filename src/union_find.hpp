#ifndef UNION_FIND_HPP
#define UNION_FIND_HPP

#include <vector>

/**
 * @brief Interfaz para la estructura de datos Union-Find
 * Proporciona operaciones básicas para manejo de conjuntos disjuntos
 */
class IUnionFind {
public:
    /**
     * @brief Encuentra la raíz del conjunto que contiene el elemento x
     * @param x Elemento para encontrar su raíz
     * @return Elemento raíz del conjunto que contiene x
     */
    virtual int find(int x) = 0;
    
    /**
     * @brief Une los conjuntos que contienen los elementos a y b
     * @param a Primer elemento
     * @param b Segundo elemento
     */
    virtual void unite(int a, int b) = 0;
    
    /**
     * @brief Verifica si dos elementos están en el mismo conjunto
     * @param a Primer elemento
     * @param b Segundo elemento
     * @return true si a y b están en el mismo conjunto, false en caso contrario
     */
    virtual bool connected(int a, int b) = 0;
    
    virtual ~IUnionFind() {}
};

/**
 * @brief Implementación unificada de Union-Find con opción de path compression
 * Usa union by size y opcionalmente path compression según el parámetro del constructor
 */
class UnionFind : public IUnionFind {
private:
    std::vector<int> parent;  ///< Arreglo de padres para estructura de árbol
    std::vector<int> size;    ///< Arreglo de tamaños para union by size
    bool use_path_compression; ///< Flag para activar/desactivar path compression

    int find_optimized(int x) {
        // Versión con path compression
        if (parent[x] != x) {
            parent[x] = find_optimized(parent[x]); // Path compression: aplanar árbol
        }
        return parent[x];
    }

public:
    /**
     * @brief Constructor - inicializa n conjuntos disjuntos
     * @param n Número de elementos (0 a n-1)
     * @param path_compression Si usar optimización path compression
     */
    UnionFind(int n, bool path_compression = false) 
        : parent(n), size(n, 1), use_path_compression(path_compression) {
        for (int i = 0; i < n; ++i) {
            parent[i] = i;  // Cada elemento es su propio padre inicialmente
        }
    }
    
    /**
     * @brief Encuentra la raíz con o sin path compression según configuración
     * @param x Elemento para encontrar su raíz
     * @return Raíz del conjunto que contiene x
     */
    int find(int x) override {
        if (use_path_compression) {
            return find_optimized(x);
        } else {
            // Versión básica sin path compression
            while (parent[x] != x) {
                x = parent[x];
            }
            return x;
        }
    }

    /**
     * @brief Une dos conjuntos usando union by size
     * @param a Primer elemento
     * @param b Segundo elemento
     */
    void unite(int a, int b) override {
        int rootA = find(a);
        int rootB = find(b);
        if (rootA == rootB)
            return; // Ya están en el mismo conjunto
        // Union by size: adjuntar árbol más pequeño al más grande
        if (size[rootA] < size[rootB]) {
            parent[rootA] = rootB;
            size[rootB] += size[rootA];
        }
        else {
            parent[rootB] = rootA;
            size[rootA] += size[rootB];
        }
    }
    
    /**
     * @brief Verifica si dos elementos están conectados
     * @param a Primer elemento
     * @param b Segundo elemento
     * @return true si están conectados, false en caso contrario
     */
    bool connected(int a, int b) override {
        return find(a) == find(b);
    }
};

// Alias para mantener compatibilidad con código existente
using UnionFindBasic = UnionFind;
class UnionFindOptimized : public UnionFind {
public:
    UnionFindOptimized(int n) : UnionFind(n, true) {}
};

#endif // UNION_FIND_HPP
