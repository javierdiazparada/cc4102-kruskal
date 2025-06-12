#include <iostream>
#include <vector>
#include <random>
#include <chrono>

#include "../include/utils.h"
#include "../include/edge_extractor.h"
#include "../include/kruskal.h"

int main()
{
    std::cout << "=== PRUEBA RÁPIDA DEL SISTEMA KRUSKAL ===" << std::endl;
    
    // Generar un grafo pequeño de prueba (N=8 nodos)
    const int N = 4096;
    std::vector<node> nodes;
    std::mt19937 gen(1234);
    std::uniform_real_distribution<double> distrib(0.0, 1.0);
    
    // Generar nodos aleatorios
    for (int i = 0; i < N; ++i) {
        nodes.push_back({distrib(gen), distrib(gen)});
    }
    
    // Calcular todas las aristas
    std::vector<edge> edges;
    for (int i = 0; i < N; ++i) {
        for (int j = i + 1; j < N; ++j) {
            double weight = distance(nodes[i], nodes[j]);
            edges.emplace_back(i, j, weight);
        }
    }
    
    std::cout << "Generado grafo con " << N << " nodos y " << edges.size() << " aristas" << std::endl;
    
    // Probar las 4 variantes
    std::vector<std::string> variantes = {
        "ArraySort + UnionFind Básico",
        "ArraySort + UnionFind Optimizado", 
        "HeapMin + UnionFind Básico",
        "HeapMin + UnionFind Optimizado"
    };
    
    std::vector<double> pesos_mst;
    
    for (int variante = 0; variante < 4; ++variante) {
        EdgeExtractor* extractor;
        bool usar_heap = (variante >= 2);
        bool usar_optimizacion = (variante % 2 == 1);
        
        if (usar_heap) {
            extractor = new HeapMin();
        } else {
            extractor = new ArraySort();
        }
        
        // Insertar todas las aristas
        for (const auto& e : edges) {
            extractor->insert_edge(e);
        }
        
        // Ejecutar Kruskal
        auto start = std::chrono::high_resolution_clock::now();
        void* resultado = kruskal(extractor, usar_optimizacion);
        auto end = std::chrono::high_resolution_clock::now();
        
        auto duracion = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        double peso_total = get_mst_weight(resultado);
        int num_aristas = get_mst_edge_count(resultado);
        
        std::cout << "Variante " << (variante + 1) << " (" << variantes[variante] << "):" << std::endl;
        std::cout << "  - Peso total MST: " << peso_total << std::endl;
        std::cout << "  - Número de aristas: " << num_aristas << std::endl;
        std::cout << "  - Tiempo: " << duracion.count() << " microsegundos" << std::endl;
        
        pesos_mst.push_back(peso_total);
        
        // Limpiar memoria
        free_kruskal_result(resultado);
        delete extractor;
        
        std::cout << std::endl;
    }
    
    // Verificar que todas las variantes producen el mismo MST
    bool todas_iguales = true;
    for (size_t i = 1; i < pesos_mst.size(); ++i) {
        if (std::abs(pesos_mst[i] - pesos_mst[0]) > 1e-10) {
            todas_iguales = false;
            break;
        }
    }
    
    if (todas_iguales) {
        std::cout << "✅ ÉXITO: Todas las variantes producen el mismo MST" << std::endl;
        std::cout << "✅ ÉXITO: El sistema está funcionando correctamente" << std::endl;
    } else {
        std::cout << "❌ ERROR: Las variantes producen MSTs diferentes" << std::endl;
        return 1;
    }
    
    return 0;
}
