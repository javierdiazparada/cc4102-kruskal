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
 * @brief Implementación básica de Union-Find sin path compression
 * Usa únicamente la optimización union by size
 */
class UnionFindBasic : public IUnionFind {
private:
    std::vector<int> parent;  ///< Arreglo de padres para estructura de árbol
    std::vector<int> size;    ///< Arreglo de tamaños para union by size

public:
    /**
     * @brief Constructor - inicializa n conjuntos disjuntos
     * @param n Número de elementos (0 a n-1)
     */
    UnionFindBasic(int n) : parent(n), size(n, 1) {
        for (int i = 0; i < n; ++i) {
            parent[i] = i;  // Cada elemento es su propio padre inicialmente
        }
    }
    
    /**
     * @brief Encuentra la raíz sin path compression
     * @param x Elemento para encontrar su raíz
     * @return Raíz del conjunto que contiene x
     */
    int find(int x) override {
        while (parent[x] != x) {
            x = parent[x];
        }
        return x;
    }
    
    /**
     * @brief Une dos conjuntos usando union by size
     * @param a Primer elemento
     * @param b Segundo elemento
     */
    void unite(int a, int b) override {
        int rootA = find(a);
        int rootB = find(b);
        
        if (rootA == rootB) return;  // Ya están en el mismo conjunto
        
        // Union by size: adjuntar árbol más pequeño al más grande
        if (size[rootA] < size[rootB]) {
            parent[rootA] = rootB;
            size[rootB] += size[rootA];
        } else {
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

/**
 * @brief Union-Find optimizado con path compression y union by size
 * Proporciona complejidad temporal amortizada casi constante
 */
class UnionFindOptimized : public IUnionFind {
private:
    std::vector<int> parent;  ///< Arreglo de padres para estructura de árbol
    std::vector<int> size;    ///< Arreglo de tamaños para union by size

public:
    /**
     * @brief Constructor - inicializa n conjuntos disjuntos
     * @param n Número de elementos (0 a n-1)
     */
    UnionFindOptimized(int n) : parent(n), size(n, 1) {
        for (int i = 0; i < n; ++i) {
            parent[i] = i;  // Cada elemento es su propio padre inicialmente
        }
    }
    
    /**
     * @brief Encuentra la raíz con optimización path compression
     * @param x Elemento para encontrar su raíz
     * @return Raíz del conjunto que contiene x
     */
    int find(int x) override {
        if (parent[x] != x) {
            parent[x] = find(parent[x]);  // Path compression: aplanar árbol
        }
        return parent[x];
    }
    
    /**
     * @brief Une dos conjuntos usando union by size
     * @param a Primer elemento
     * @param b Segundo elemento
     */
    void unite(int a, int b) override {
        int rootA = find(a);
        int rootB = find(b);
        
        if (rootA == rootB) return;  // Ya están en el mismo conjunto
        
        // Union by size: adjuntar árbol más pequeño al más grande
        if (size[rootA] < size[rootB]) {
            parent[rootA] = rootB;
            size[rootB] += size[rootA];
        } else {
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

#endif // UNION_FIND_HPP
