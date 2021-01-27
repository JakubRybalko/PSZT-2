#pragma once
#include <random>
#include <chrono>
#include <vector>
#include <iostream>
#include <algorithm>
#include "individual.hpp"
#include "Chromosome.hpp"
#include "pugixml.hpp"
#include <set>
//
class gen_algorithm {
private:
    unsigned population_size;
    unsigned mutation_probability;
    unsigned cross_probability;
    unsigned parm_t;
    unsigned lengthOfVector;
    unsigned iteration_count;
    individual l_best_so_far;
    individual r_best_so_far;
    individual best_so_far;
    std::vector<individual> population;
    std::vector<Chromosome> population_chromosome;
    std::vector<std::vector<std::vector<std::vector<unsigned>>>> demand_paths;
    std::vector<unsigned> demands;
    unsigned edges[18][18];

    void crossChromosome();
    void crossMethod(int method_number, int changed_element_number, int crossed_element_number, std::vector<std::vector<individual>> &vec, bool isNotSingle);
    void mutate();
    unsigned generate_number();
    void gen_function();
    void fintess_calc();
    void fintess_calc_chromosome(unsigned path_count, unsigned modularity);
    void selection();
    void initPopulation();
    void init_population_chromosome(unsigned path_count);
    void prepare_next_gen();
    void selection_tournament();
    void selection_tournament_chromosome();
    void mutate_long();
    void load_data();

public:

    gen_algorithm();

    ~gen_algorithm();

    gen_algorithm(unsigned p_size, float m_probability, float c_probability, unsigned t, unsigned length_of_vector, unsigned iter_count);

    individual start();
};