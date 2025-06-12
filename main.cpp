#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <algorithm>
#include <atomic>
#include <thread>
#include <mutex>
#include <format>
#include <condition_variable>
#include <iomanip>
#include <type_traits>

#include "include/utils.h"
#include "include/edge_extractor.h"
#include "include/kruskal.h"

const std::string NAME_DF = "resultados/df.csv";
const std::string NAME_LOGS = "resultados/logs.txt";
constexpr unsigned int LIMIT_SUBITER = 5;
constexpr unsigned int SEED = 1234;
constexpr unsigned int WORST_CASE_THREAD = 135; // MB used (PREVENTIVE)
constexpr unsigned int MAX_N_THREADS = 5; // Increased to 5 threads for better performance
constexpr unsigned int LOG2_N_INIT = 5;
constexpr unsigned int LOG2_N_END = 12;

std::atomic<unsigned short> n_thread = 0;
std::atomic<double> process_ready = 0.;
bool ready = true;

std::ofstream df, logs;
std::mutex m_df, m_logs, m_ready;
std::condition_variable cv;

std::vector<arg> queue_gen;

// Write the results in a file
void safe_write_data_logs(const std::string txt)
{
    m_logs.lock();
    logs << txt << "\n";
    m_logs.unlock();
}

void safe_write_data_logs(const datapoint *data, const std::string txt)
{
    m_logs.lock();
    logs << "Experiment (N:" << data->n << "; EdgeExtractor: " << data->edge_extractor_name << "; optimization_path: " << data->opti_path << ") " << txt << "\n";
    m_logs.unlock();
}

void safe_write_datapoint(const datapoint *data)
{
    m_df.lock();
    df <<  data->n << ", " << data->edge_extractor_name << ", " << data->time_insertion << ", " << data->opti_path << ", " << data->time_kruskal << ";\n";
    m_df.unlock();
}

void experiment(EdgeExtractor* edge_extractor, const double time_insertion, const bool is_opt)
{
    // Generate the data for the experiment
    struct datapoint* datapoint = new struct datapoint;

    // Calculate number of nodes from number of edges (for complete graph: edges = n*(n-1)/2)
    size_t num_aristas = edge_extractor->size();
    int num_nodos = static_cast<int>((1 + std::sqrt(1 + 8.0 * num_aristas)) / 2.0);
    
    datapoint->n = num_nodos;  // Store number of nodes, not edges
    datapoint->edge_extractor_name = edge_extractor->get_name();
    datapoint->opti_path = is_opt;
    datapoint->time_insertion = time_insertion;

    // Run Kruskal algorithm
    safe_write_data_logs(datapoint, "Run Kruskal algorithm...\n");

    auto start_exp = std::chrono::high_resolution_clock::now(); // time initial insert
    void* kruskal_result = kruskal(edge_extractor, is_opt);
    auto end_exp = std::chrono::high_resolution_clock::now(); // time end insert
    std::chrono::duration<double> delta_time_exp = end_exp - start_exp;

    datapoint->time_kruskal = delta_time_exp.count();

    // Clean up the result
    free_kruskal_result(kruskal_result);

    safe_write_datapoint(datapoint);

    safe_write_data_logs(datapoint, "Ready!");
    
    // Clean up datapoint
    delete datapoint;

}


void main_experiment(EdgeExtractor* edge_extractor, const std::vector<edge>& array_edges)
{
    // Insert all edges from array_edges to edge extractor object
    std::string txt_init = std::format("Insertion Initialized...\nInsert {} edges en {}...\n", array_edges.size(), edge_extractor->get_name());
    safe_write_data_logs(txt_init);

    auto start_insert = std::chrono::high_resolution_clock::now(); // time initial insert
    for (const edge& e : array_edges){edge_extractor->insert_edge(e);}
    auto end_insert = std::chrono::high_resolution_clock::now(); // time end insert
    

    std::string txt_end = std::format("Insertion Finished! \nInsert {} edges en {}...\n", array_edges.size(), edge_extractor->get_name());
    safe_write_data_logs(txt_end);

    std::chrono::duration<double> delta_time_insert = end_insert - start_insert;
    // Create a new edge extractor object
    EdgeExtractor* edge_extractor2 = edge_extractor->clone(); // Duplicate edge extractor object

    // Run the Kruskal algorithm on both edge extractor objects
    experiment(edge_extractor, delta_time_insert.count(), true);
    experiment(edge_extractor2, delta_time_insert.count(), false);

    // Free memory
    delete edge_extractor;
    delete edge_extractor2;
}


void main_thread(const arg data, unsigned int thread_seed)
{
    std::vector<node> array_nodes;
    std::uniform_real_distribution<double> distrib(0.0, 1.0);
    
    // Create thread-specific random generator
    std::mt19937 gen(thread_seed);


    std::string txt =  std::format("Iniciando el experimento N={}", (1<<data.n));
    safe_write_data_logs(txt);

    // Reserve space in memory
    array_nodes.reserve(1 << data.n);

    // Generate random nodes
    for (size_t i = 0; i < array_nodes.capacity(); ++i)
    {
        array_nodes.push_back({distrib(gen), distrib(gen)});
    }

    // Calculate the distance between each pair of nodes
    // and save it in a vector of edges
    std::vector<edge> array_edges;
    array_edges.reserve(array_nodes.size() * (array_nodes.size() - 1) / 2);
    
    for (size_t i = 0; i < array_nodes.size(); ++i)
    {
        for (size_t j = i + 1; j < array_nodes.size(); ++j)
        {
            double weight = distance(array_nodes[i], array_nodes[j]);
            array_edges.emplace_back(static_cast<int>(i), static_cast<int>(j), weight);
        }
    }

    // Delete the vector
    array_nodes.clear();

    ArraySort* array_sort = new ArraySort();
    HeapMin* heap_min = new HeapMin();
    
    // Run the experiment
    main_experiment(array_sort, array_edges);
    main_experiment(heap_min, array_edges);
    n_thread--;
    ready = true;
    cv.notify_one();
    process_ready += 1. / (LIMIT_SUBITER * (LOG2_N_END - LOG2_N_INIT + 1));
    std::cout << "\n Porcentaje de la tarea:" << process_ready*100 << "%;\n";
}

int main()
{
    std::cout << "Iniciando el programa...\n";
    std::cin.tie(nullptr);
    std::ios_base::sync_with_stdio(false);

    df.open(NAME_DF, std::ios::out);
    logs.open(NAME_LOGS, std::ios::out);

    df << std::setprecision(10);
    std::cout << "Iniciando el csv..."<< std::endl;
    df << "N, EdgeExtractorName, TimeInsertion, KruskalOptiPath, TimeKruskal;\n";
    std::cout << "Inicializado el csv!"<< std::endl;

    std::mt19937 gen(SEED); // Generator

    std::vector<std::jthread> list_threads;
    std::cout << "Iniciando los experimentos..."<< std::endl;
    for (unsigned int i = LOG2_N_INIT; i < (LOG2_N_END + 1); i++)
    {
        for (unsigned int j = LIMIT_SUBITER; j > 0; j--)
        {
            queue_gen.push_back({i});
        }
    }

    unsigned int thread_counter = 0;
    while (!queue_gen.empty())
    {
        if (n_thread < MAX_N_THREADS)
        {
            n_thread++;
            arg data_experiment = queue_gen.back();
            queue_gen.pop_back();
            
            // Generate unique seed for each thread
            unsigned int thread_seed = SEED + thread_counter * 1000 + data_experiment.n;
            thread_counter++;
            
            std::jthread work(main_thread, data_experiment, thread_seed);
            list_threads.push_back(std::move(work));
        }
        else
        {
            {
                ready = false;
                std::unique_lock lk(m_ready);
                cv.wait(lk, []
                        { return ready; });
            }
        }
    }

    // Wait for all threads to complete before closing files
    std::cout << "Esperando que terminen todos los threads..." << std::endl;
    for (auto& thread : list_threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
    
    std::cout << "Todos los threads completados. Cerrando archivos..." << std::endl;
    df.close();
    logs.close();

    return 0;
}
