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

const std::string NAME_DF = "df.csv";
const std::string NAME_LOGS = "logs.txt";
constexpr unsigned int LIMIT_SUBITER = 5;
constexpr unsigned int SEED = 1234;
constexpr unsigned int WORST_CASE_THREAD = 135; // MB used (PREVENTIVE)
constexpr unsigned int MAX_N_THREADS = 1;
constexpr unsigned int LOG2_N_INIT = 5;
constexpr unsigned int LOG2_N_END = 12;

std::atomic<unsigned short> n_thread = 0;
std::atomic<double> process_ready = 0.;
bool ready = true;

std::ofstream df, logs;
std::mutex m_df, m_logs, m_ready;
std::condition_variable cv;

std::vector<arg> queue_gen;

// Distance function
double distance(node a, node b)
{
    return std::pow(a.x - b.x, 2) + std::pow(a.y - b.y, 2);
}

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
    logs << "Experiment (N:" << (1 >> data->n) << "; is_heap: " << data->edge_extractor_name << "; optimization_path: " << data->opti_path << ") " << txt << "\n";
    m_logs.unlock();
}

void safe_write_datapoint(const datapoint *data)
{
    m_df.lock();
    df << (1 >> data->n) << ", " << data->edge_extractor_name << ", " << data->time_insertion << ", " << data->opti_path << ", " << data->time_kruskal << ";\n";
    m_df.unlock();
}

void experiment(EdgeExtractor* edge_extractor, const double time_insertion, const bool is_opt)
{
    // Generate the data for the experiment
    datapoint* datapoint;

    datapoint->n = edge_extractor->size();
    datapoint->edge_extractor_name = edge_extractor->get_name();
    datapoint->opti_path = is_opt;
    datapoint->time_insertion = time_insertion;

    // Run Kruskal algorithm
    safe_write_data_logs(datapoint, "Run Kruskal algorithm...\n");

    auto start_exp = std::chrono::high_resolution_clock::now(); // time initial insert
    //kruskal::Kruskal kruskal(edge_extractor, is_opt);
    auto end_exp = std::chrono::high_resolution_clock::now(); // time end insert
    std::chrono::duration<double> delta_time_exp = end_exp - start_exp;

    datapoint->time_kruskal = delta_time_exp.count();

    safe_write_datapoint(datapoint);

    safe_write_data_logs(datapoint, "Ready!");
}


void main_experiment(EdgeExtractor* edge_extractor, std::vector<double> array_edges)
{
    // Create a new edge extractor object
    std::cout << "Experiment started with " << "edge_extractor->get_name()" << std::endl;
    // Insert all edges from array_edges to edge extractor object

    //std::string txt_init = std::format("Insertion Initialized...\nInsert {} edges en {}...\n", array_edges.size(), edge_extractor->get_name());
    //safe_write_data_logs(txt_init);

    auto start_insert = std::chrono::high_resolution_clock::now(); // time initial insert
    for (double edge : array_edges){edge_extractor->insert_edge(edge);}
    auto end_insert = std::chrono::high_resolution_clock::now(); // time end insert

    //std::string txt_end = std::format("Insertion Finished! \nInsert {} edges en {}...\n", array_edges.size(), edge_extractor->get_name());
    //safe_write_data_logs(txt_end);

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


void main_thread(const arg data, std::mt19937 gen)
{
    std::vector<node> array_nodes;
    std::uniform_real_distribution<double> distrib(0.0, 1.0);

    std::cout << "Iniciando el experimento N="<< (1<<data.n) << std::endl;

    std::cout << "Reservando memoria..." << std::endl;
    // Reserve space in memory
    array_nodes.reserve(1<<data.n);

    std::cout << "Generando nodos..." << std::endl;
    // Generate random nodes
    for (size_t i = 0; i < array_nodes.capacity(); ++i)
    {
        array_nodes.push_back({distrib(gen), distrib(gen)});
    }

    std::cout << "Calculando distancias..." << std::endl;
    // Calculate the distance between each pair of nodes
    // and save it in a vector
    std::vector<double> array_edges;
    for (size_t i = 0; i < array_nodes.size(); ++i)
    {
        for (size_t j = i + 1; j < array_nodes.size(); ++j)
        {
            array_edges.push_back(distance(array_nodes[i], array_nodes[j]));
        }
    }

    std::cout << "Limpieza de memoria..." << std::endl;
    // Delete the vector
    array_nodes.clear();

    
    std::cout << "Creando la estructura de datos..." << std::endl;
    ArraySort* array_sort = new ArraySort();
    HeapMin* heap_min = new HeapMin();
    
    // Run the experiment
    std::cout << "Ejecutando experimento..." << std::endl;
    main_experiment(array_sort, array_edges);
    main_experiment(heap_min,array_edges);
    //n_thread--;
    //ready = true;
    //cv.notify_one();
    //process_ready += 8. / (LIMIT_SUBITER * 4);
    std::cout << "\n Porcentaje de la tarea:" << process_ready << "%;\n";
}

int main()
{
    std::cout << "Iniciando el programa...\n";
    std::cin.tie(nullptr);
    std::ios_base::sync_with_stdio(false);

    df.open(NAME_DF, std::ios::app);
    logs.open(NAME_LOGS, std::ios::app);

    df << std::setprecision(10);
    std::cout << "Iniciando el csv...\n"<< std::endl;
    df << "N, EdgeExtractorName, TimeInsertion, KruskalOptiPath, TimeKruskal;\n";
    std::cout << "Inicializado el csv!\n"<< std::endl;

    std::mt19937 gen(SEED); // Generator

    // std::vector<std::jthread> list_threads;
    std::cout << "Iniciando los experimentos...\n"<< std::endl;
    for (unsigned int i = LOG2_N_INIT; i < (LOG2_N_END + 1); i++)
    {
        for (unsigned int j = LIMIT_SUBITER; j > 0; j--)
        {
            main_thread({i}, gen); //queue_gen.push_back({i});
        }
    }

    //while (!queue_gen.empty())
    //{
    //    if (n_thread < MAX_N_THREADS)
    //    {
    //        n_thread++;
    //        arg data_experiment = queue_gen.back();
    //        queue_gen.pop_back();
    //        std::jthread work(main_thread, data_experiment, gen);
    //        list_threads.push_back(std::move(work));
    //    }
    //    else
    //    {
    //        {
    //            ready = false;
    //            std::unique_lock lk(m_ready);
    //            cv.wait(lk, []
    //                    { return ready; });
    //        }
    //    }
    //}

    df.close();
    logs.close();

    return 0;
}