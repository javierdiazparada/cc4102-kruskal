#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <map>
#include <tuple>

#include "../include/utils.h"
#include "../include/edge_extractor.h"
#include "../include/kruskal.h"

/**
 * @brief Estructura para almacenar resultados de un experimento
 */
struct ExperimentResult {
    int n;                          // Número de nodos
    std::string extractor_type;     // "ArraySort" o "HeapMin"
    bool optimized;                 // Si usa optimización path compression
    double insertion_time_ms;       // Tiempo de inserción en milisegundos
    double kruskal_time_ms;         // Tiempo de Kruskal en milisegundos
    double total_time_ms;           // Tiempo total en milisegundos
    double mst_weight;              // Peso del MST
    int mst_edges;                  // Número de aristas en MST
    bool correctness;               // Si el resultado es correcto
};

/**
 * @brief Genera N puntos aleatorios en el plano [0,1] x [0,1]
 * @param n Número de puntos a generar
 * @param seed Semilla para el generador aleatorio
 * @return Vector de nodos generados
 */
std::vector<node> generate_random_points(int n, unsigned int seed) {
    std::vector<node> points;
    points.reserve(n);
    
    std::mt19937 gen(seed);
    std::uniform_real_distribution<double> distrib(0.0, 1.0);
    
    for (int i = 0; i < n; ++i) {
        points.push_back({distrib(gen), distrib(gen)});
    }
    
    return points;
}

/**
 * @brief Genera todas las aristas posibles entre N puntos
 * @param points Vector de puntos
 * @return Vector de aristas con pesos calculados
 */
std::vector<edge> generate_all_edges(const std::vector<node>& points) {
    std::vector<edge> edges;
    int n = points.size();
    edges.reserve(n * (n - 1) / 2);
    
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            double weight = distance(points[i], points[j]);
            edges.emplace_back(i, j, weight);
        }
    }
    
    return edges;
}

/**
 * @brief Ejecuta un experimento completo de Kruskal
 * @param edges Vector de aristas del grafo
 * @param use_heap Si usar HeapMin (true) o ArraySort (false)
 * @param use_optimization Si usar optimización path compression
 * @param n Número de nodos en el grafo
 * @return Resultado del experimento
 */
ExperimentResult run_experiment(const std::vector<edge>& edges, bool use_heap, bool use_optimization, int n) {
    ExperimentResult result;
    result.n = n;
    result.extractor_type = use_heap ? "HeapMin" : "ArraySort";
    result.optimized = use_optimization;
    
    // Crear extractor apropiado
    EdgeExtractor* extractor;
    if (use_heap) {
        extractor = new HeapMin();
    } else {
        extractor = new ArraySort();
    }
    
    // Medir tiempo de inserción
    auto start_insertion = std::chrono::high_resolution_clock::now();
    for (const auto& e : edges) {
        extractor->insert_edge(e);
    }
    auto end_insertion = std::chrono::high_resolution_clock::now();
    
    result.insertion_time_ms = std::chrono::duration<double, std::milli>(end_insertion - start_insertion).count();
    
    // Medir tiempo de Kruskal
    auto start_kruskal = std::chrono::high_resolution_clock::now();
    void* mst_result = kruskal(extractor, use_optimization);
    auto end_kruskal = std::chrono::high_resolution_clock::now();
    
    result.kruskal_time_ms = std::chrono::duration<double, std::milli>(end_kruskal - start_kruskal).count();
    result.total_time_ms = result.insertion_time_ms + result.kruskal_time_ms;
    
    // Obtener resultados del MST
    if (mst_result) {
        result.mst_weight = get_mst_weight(mst_result);
        result.mst_edges = get_mst_edge_count(mst_result);
        result.correctness = (result.mst_edges == n - 1);
        free_kruskal_result(mst_result);
    } else {
        result.mst_weight = 0.0;
        result.mst_edges = 0;
        result.correctness = false;
    }
    
    delete extractor;
    return result;
}

/**
 * @brief Verifica la correctitud comparando pesos de MST
 * @param results Vector de resultados a verificar
 * @return true si todos los pesos son iguales (dentro de tolerancia)
 */
bool verify_correctness(const std::vector<ExperimentResult>& results) {
    if (results.empty()) return false;
    
    double reference_weight = results[0].mst_weight;
    const double tolerance = 1e-10;
    
    for (const auto& result : results) {
        if (std::abs(result.mst_weight - reference_weight) > tolerance) {
            return false;
        }
        if (!result.correctness) {
            return false;
        }
    }
    
    return true;
}

/**
 * @brief Guarda resultados en archivo CSV
 * @param results Vector de resultados
 * @param filename Nombre del archivo
 */
void save_results_to_csv(const std::vector<ExperimentResult>& results, const std::string& filename) {
    std::ofstream file(filename);
    
    // Escribir encabezados
    file << "n,extractor_type,optimized,insertion_time_ms,kruskal_time_ms,total_time_ms,mst_weight,mst_edges,correctness\n";
    
    // Escribir datos
    for (const auto& result : results) {
        file << result.n << ","
             << result.extractor_type << ","
             << (result.optimized ? "true" : "false") << ","
             << std::fixed << std::setprecision(6) << result.insertion_time_ms << ","
             << result.kruskal_time_ms << ","
             << result.total_time_ms << ","
             << std::setprecision(10) << result.mst_weight << ","
             << result.mst_edges << ","
             << (result.correctness ? "true" : "false") << "\n";
    }
    
    file.close();
}

/**
 * @brief Imprime estadísticas resumidas
 * @param results Vector de resultados
 */
void print_summary_statistics(const std::vector<ExperimentResult>& results) {
    std::cout << "\n=== ESTADÍSTICAS RESUMIDAS ===" << std::endl;
    std::cout << std::setw(8) << "N" 
              << std::setw(12) << "Extractor" 
              << std::setw(8) << "Optim" 
              << std::setw(15) << "Tiempo Avg (ms)" 
              << std::setw(15) << "MST Weight" 
              << std::setw(10) << "Correcto" << std::endl;
    std::cout << std::string(70, '-') << std::endl;
    
    // Agrupar por configuración
    std::map<std::tuple<int, std::string, bool>, std::vector<ExperimentResult>> grouped;
    for (const auto& result : results) {
        auto key = std::make_tuple(result.n, result.extractor_type, result.optimized);
        grouped[key].push_back(result);
    }
    
    // Calcular promedios
    for (const auto& [key, group] : grouped) {
        double avg_time = 0.0;
        double avg_weight = 0.0;
        bool all_correct = true;
        
        for (const auto& result : group) {
            avg_time += result.total_time_ms;
            avg_weight += result.mst_weight;
            all_correct = all_correct && result.correctness;
        }
        
        avg_time /= group.size();
        avg_weight /= group.size();
        
        std::cout << std::setw(8) << std::get<0>(key)
                  << std::setw(12) << std::get<1>(key)
                  << std::setw(8) << (std::get<2>(key) ? "Sí" : "No")
                  << std::setw(15) << std::fixed << std::setprecision(3) << avg_time
                  << std::setw(15) << std::setprecision(6) << avg_weight
                  << std::setw(10) << (all_correct ? "✓" : "✗") << std::endl;
    }
}

int main() {
    std::cout << "=== PRUEBA COMPREHENSIVA ALGORITMO KRUSKAL ===" << std::endl;
    std::cout << "Probando desde 2^5 hasta 2^12 nodos con 5 secuencias por tamaño" << std::endl;
    
    // Configuración del experimento
    const std::vector<int> sizes = {32, 64, 128, 256, 512, 1024, 2048, 4096}; // 2^5 a 2^12
    const int num_sequences = 5;
    const std::vector<bool> heap_options = {false, true};  // ArraySort, HeapMin
    const std::vector<bool> optimization_options = {false, true};  // Sin/Con optimización
    
    std::vector<ExperimentResult> all_results;
    
    // Ejecutar experimentos
    for (int size : sizes) {
        std::cout << "\n--- Probando con N = " << size << " nodos ---" << std::endl;
        
        for (int seq = 0; seq < num_sequences; ++seq) {
            std::cout << "Secuencia " << (seq + 1) << "/" << num_sequences << "..." << std::endl;
            
            // Generar puntos y aristas para esta secuencia
            unsigned int seed = 1000 + seq * 100 + size;  // Semilla determinística pero única
            std::vector<node> points = generate_random_points(size, seed);
            std::vector<edge> edges = generate_all_edges(points);
            
            std::cout << "  Generadas " << edges.size() << " aristas" << std::endl;
            
            // Probar las 4 variantes
            std::vector<ExperimentResult> sequence_results;
            
            for (bool use_heap : heap_options) {
                for (bool use_optimization : optimization_options) {
                    ExperimentResult result = run_experiment(edges, use_heap, use_optimization, size);
                    sequence_results.push_back(result);
                    all_results.push_back(result);
                    
                    std::cout << "    " << result.extractor_type 
                              << " + " << (result.optimized ? "Optimizado" : "Básico")
                              << ": " << std::fixed << std::setprecision(3) << result.total_time_ms << " ms"
                              << " (MST: " << std::setprecision(6) << result.mst_weight << ")" << std::endl;
                }
            }
            
            // Verificar correctitud para esta secuencia
            if (!verify_correctness(sequence_results)) {
                std::cout << "  ⚠️  ADVERTENCIA: Inconsistencia en resultados para secuencia " << (seq + 1) << std::endl;
            }
        }
    }
    
    // Verificar correctitud global
    std::cout << "\n=== VERIFICACIÓN DE CORRECTITUD ===" << std::endl;
    
    // Agrupar por tamaño y secuencia para verificar que las 4 variantes den el mismo resultado
    std::map<std::pair<int, int>, std::vector<ExperimentResult>> by_size_and_sequence;
    int sequence_counter = 0;
    
    for (int size : sizes) {
        for (int seq = 0; seq < num_sequences; ++seq) {
            std::pair<int, int> key = {size, seq};
            // Tomar los 4 resultados correspondientes a esta secuencia
            for (int variant = 0; variant < 4; ++variant) {
                int index = sequence_counter * 4 + variant;
                if (index < all_results.size()) {
                    by_size_and_sequence[key].push_back(all_results[index]);
                }
            }
            sequence_counter++;
        }
    }
    
    bool all_correct = true;
    for (int size : sizes) {
        bool size_correct = true;
        for (int seq = 0; seq < num_sequences; ++seq) {
            std::pair<int, int> key = {size, seq};
            if (by_size_and_sequence[key].size() == 4) {
                bool seq_correct = verify_correctness(by_size_and_sequence[key]);
                if (!seq_correct) {
                    size_correct = false;
                }
            }
        }
        std::cout << "N = " << std::setw(4) << size << ": " 
                  << (size_correct ? "✅ CORRECTO" : "❌ ERROR") << std::endl;
        all_correct = all_correct && size_correct;
    }
    
    if (all_correct) {
        std::cout << "\n🎉 ÉXITO: Todas las variantes producen resultados consistentes" << std::endl;
    } else {
        std::cout << "\n💥 ERROR: Se encontraron inconsistencias en los resultados" << std::endl;
    }
    
    // Imprimir estadísticas
    print_summary_statistics(all_results);
    
    // Guardar resultados
    save_results_to_csv(all_results, "resultados/kruskal_results.csv");
    std::cout << "\n📊 Resultados guardados en 'resultados/kruskal_results.csv'" << std::endl;
    
    // Análisis de rendimiento
    std::cout << "\n=== ANÁLISIS DE RENDIMIENTO ===" << std::endl;
    std::cout << "Los resultados muestran el comportamiento de las 4 variantes:" << std::endl;
    std::cout << "1. ArraySort + UnionFind Básico" << std::endl;
    std::cout << "2. ArraySort + UnionFind Optimizado" << std::endl;
    std::cout << "3. HeapMin + UnionFind Básico" << std::endl;
    std::cout << "4. HeapMin + UnionFind Optimizado" << std::endl;
    std::cout << "\nUse los datos del CSV para generar gráficos de rendimiento." << std::endl;
    
    return all_correct ? 0 : 1;
}
