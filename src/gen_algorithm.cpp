#include "gen_algorithm.hpp"

gen_algorithm::gen_algorithm() = default;

gen_algorithm::~gen_algorithm() = default;

gen_algorithm::gen_algorithm(unsigned p_size, float m_probability, float c_probability, unsigned t, unsigned length_of_vector, unsigned iter_count)
{

    population_size = p_size;
    mutation_probability = m_probability;
    cross_probability = c_probability;
    parm_t = t;
    lengthOfVector = 66; //length_of_vector;
    iteration_count = iter_count;
    load_data();
}

/*
*   Metoda wykonujaca krzyzowanie. Wybiera losowo ktore z pierwszych a elementow, potem kolejnych, bedzie pochodzic od ktorego rodzica.
*/

void gen_algorithm::crossMethod(int method_number, int changed_element_numberP, int crossed_element_numberP, std::vector<std::vector<individual>> &vec, bool isNotSingle)
{
    int a = generate_number() % lengthOfVector;

    std::vector<individual> tmp;
    tmp.reserve(lengthOfVector);
    std::vector<individual> tmp2;
    tmp2.reserve(lengthOfVector);
    std::vector<individual> changed_element_number = population_chromosome[changed_element_numberP].get_chromosome();
    std::vector<individual> crossed_element_number = population_chromosome[crossed_element_numberP].get_chromosome();

    switch (method_number)
    {
    case 0: // Ab

        for (int i = 0; i < a; i++)
        {
            tmp.push_back(crossed_element_number[i]);
            if (isNotSingle)
                tmp2.push_back(crossed_element_number[i]);
        }
        for (unsigned i = a; i < lengthOfVector; i++)
        {
            tmp.push_back(changed_element_number[i]);
            if (isNotSingle)
                tmp2.push_back(changed_element_number[i]);
        }
        break;

    case 1: // aB

        for (int i = 0; i < a; i++)
        {
            tmp.push_back(changed_element_number[i]);
            if (isNotSingle)
                tmp2.push_back(changed_element_number[i]);
        }
        for (unsigned i = a; i < lengthOfVector; i++)
        {
            tmp.push_back(crossed_element_number[i]);
            if (isNotSingle)
                tmp2.push_back(crossed_element_number[i]);
        }
        break;

    default:
        break;
    }

    vec.push_back(tmp);
    if (isNotSingle)
        vec.push_back(tmp2);
}

/*
*   Krzyzowanie jendopunktowe. Na podstawie prawdopodobienstwa tworzone sa 2 grupy, nastepnie elementy z grupy pierwszej lacza
*   sie z elementami z grupy drugiej. Z dwoch rodzicow powstaje dwojka dzieci. 
*/

void gen_algorithm::crossChromosome()
{
    std::vector<std::vector<individual>> vec;
    vec.reserve(population_chromosome.size());

    std::vector<int> first_half;
    std::vector<int> second_half;

    first_half.reserve(population_chromosome.size() / 2 + 1);
    second_half.reserve(population_chromosome.size() / 2);

    bool first = true;

    for (unsigned i = 0; i < population_chromosome.size(); i++)
    {
        if (generate_number() % 100 < cross_probability)
        {
            if (first)
            {
                first_half.push_back(i);
                first = false;
            }
            else
            {
                second_half.push_back(i);
                first = true;
            }
        }
        else
        {
            vec.push_back(population_chromosome[i].get_chromosome());
        }
    }

    if (second_half.size() == 0)                            // gdy prawdopodobienstwo jest tak male, ze zostal wylosowany tylko jeden osobnik do 
    {                                                       // skrzyzowania, jest on skrzyzowany z losowym innym osobnikiem, natomiast ten drugi
        int i = generate_number() % population_chromosome.size();      // nie zostaje nadpisany przez dziecko
        while (i == 0)
        {
            i = generate_number() % population_chromosome.size();
        }
        crossMethod(generate_number() % 2, first_half[0], i, vec, false);
    }
    else
    {
        for (unsigned i = 0; i < first_half.size(); i++)
        {
            if (second_half.size() == 0)
            {
                int i = generate_number() % population_chromosome.size();
                while (i == first_half[0])  
                {
                    i = generate_number() % population_chromosome.size();
                }
                crossMethod(generate_number() % 2, first_half[0], i, vec, false);
            }
            else
            {
                unsigned x = generate_number() % second_half.size();

                crossMethod(generate_number() % 2, first_half[i], second_half[x], vec, true);

                second_half.erase(second_half.begin() + x);
            }
        }
    }

    for (unsigned i = 0; i < vec.size(); i++)
    {
        population_chromosome[i].set_chromosome(vec[i]);
    }
}
/*
*   pierwsza wersja mutacji. Przechodzi po kazdym bicie kazdego elementu i wylicza prawdopodobienstwo
*/
void gen_algorithm::mutate_long()
{
    for(unsigned i = 0; i < population.size(); i++)
    {
        std::vector<unsigned> tmp = population[i].get_gene();
        for (unsigned x = 0; x < lengthOfVector; x++)
        {
            if (generate_number() % 100 < mutation_probability)
            {
                tmp[x] = (tmp[x] + 1) % 2;
            }
        }
        population[i].set_gene(tmp);
    }
}

void gen_algorithm::mutate()
{
    int numberOfElementsToChange = mutation_probability * lengthOfVector * population.size() / 1000;    // liczenie ilosci bitow do zmiany
    std::vector<std::vector<bool>> tmpCheckVec;
    std::vector<bool> tmpCheck;

    for (unsigned i = 0; i < population_size; i++)      // tworzenie tablicy przechowywującej zmnienione bity
    {
        for (unsigned j = 0; j < lengthOfVector; j++)
        {
            tmpCheck.push_back(false);
        }
        tmpCheckVec.push_back(tmpCheck);
    }

    for (int i = 0; i < numberOfElementsToChange; i++)      // zmiana kolejnych bitow
    {
        int x = generate_number() % population.size();
        int y = generate_number() % lengthOfVector;

        while (tmpCheckVec[x][y] == true)                   // sprawdzenie czy bit zostal juz zmieniony i ewentualne wylosowanie nowego
        {
            x = generate_number() % population.size();
            y = generate_number() % lengthOfVector;
        }
        tmpCheckVec[x][y] = true;

        std::vector<unsigned> tmpVec = population[x].get_gene();
        tmpVec[y] = (tmpVec[y] + 1) % 2;
        population[x].set_gene(tmpVec);
    }
}

void gen_algorithm::gen_function()
{
}

void gen_algorithm::initPopulation()
{
    for (unsigned i = 0; i < population_size; i++)
    {
        std::vector<unsigned> tmp;
        for (unsigned x = 0; x < lengthOfVector; x++)
        {
            tmp.push_back(generate_number() % 2);
        }
        individual ind;
        ind.set_gene(tmp);
        population.push_back(ind);
    }
}

void gen_algorithm::init_population_chromosome(unsigned path_count) {
    
    for (unsigned i = 0; i < population_size; i++) {
        
        std::vector<individual> temp;

        for(unsigned j = 0; j < 66; ++j) {

            std::vector<unsigned> tmp, tmp2;

            tmp.push_back(0);

            for(unsigned a = 1; a < path_count; a++) {
                
                tmp.push_back(generate_number() % 101);
            }

            tmp.push_back(100);
        
            std::sort(tmp.begin(), tmp.end());

            for(unsigned a = 0; a < path_count; a++) {
                tmp2[a] = tmp[a+1] - tmp[a];
            }

            individual ind;
            ind.set_gene(tmp2);
            temp.push_back(ind);
        }

        Chromosome ch;
        ch.set_chromosome(temp);
        population_chromosome.push_back(ch);
    }
}

unsigned gen_algorithm::generate_number()
{

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 generator(seed);
    return generator();
}

void gen_algorithm::fintess_calc_chromosome(unsigned path_count, unsigned modularity) {
    
    unsigned module_count;

    for(auto &ch : population_chromosome) {

        module_count = 0;

        for(int i = 0; i < 18; i++)
            for(int j = 0; j < 18; j++)
                edges[i][j] = 0;

        for(int i = 0; i < 66; i++)
            for(int j = 0; j < path_count; j++)
                for(auto &link : demand_paths[i][j])
                    edges[link[0]][link[1]] += ch.get_chromosome()[i].get_gene()[j] * demands[i];
        
        for(int i = 0; i < 18; i++)
            for(int j = 0; j < 18; j++)
                module_count += (int)std::ceil((double)edges[i][j] / (modularity * 100));

        ch.set_fitness(module_count);
    }
}

void gen_algorithm::fintess_calc() {

    unsigned ones_score = 0;
    unsigned zeros_score = 0;
    unsigned score = 0;
    std::vector<unsigned> a;

    for (unsigned i = 0; i < lengthOfVector; ++i)
        a.push_back(0);

    l_best_so_far.set_fitness(0);
    r_best_so_far.set_fitness(0);
    l_best_so_far.set_gene(a);
    r_best_so_far.set_gene(a);

    for (auto &i : population) {

        ones_score = 0;
        zeros_score = 0;
        score = 0;

        for (auto x = i.get_gene().begin(); x != i.get_gene().end(); ++x) { //
                                                                            //
            if (*x == 0)                                                    //
                break;                                                      //
            else                                                            //
                ++ones_score;                                               //
        }                                                                   // zliczanie jedynek od początku genu.

        for (auto j = i.get_gene().rbegin(); j != i.get_gene().rend(); ++j) {   //
                                                                                //
            if (*j == 1)                                                        //
                break;                                                          //
            else                                                                //
                ++zeros_score;                                                  //
        }                                                                       // zliczanie zer od końca genu.

        if (zeros_score >= parm_t && ones_score >= parm_t)  //
            score += 100;                                   // wartość super.

        score += std::max(ones_score, zeros_score); // dodajemy maksymalną wartośc pochodzącą od zer lub jedynek.

        i.set_fitness(score);   // ustawiamy funkcję celu dla danego osobnika.

        if (l_best_so_far < i && ones_score > zeros_score)  //
            l_best_so_far = i;                              //
        if (r_best_so_far < i && zeros_score > ones_score)  //
            r_best_so_far = i;                              //
        if (best_so_far < i)                                //
            best_so_far = i;                                // funkcje potrzebne do realizacji wyświetlania najlepszych osobników z zerami i jedynkami dla danej generacji.
    }
}

void gen_algorithm::selection() {

    long unsigned number;
    long unsigned sum;
    int count;
    long unsigned fitness_sum = 0;
    std::vector<individual> temp;

    for (auto i : population)               //
        fitness_sum += i.get_fitness();     // liczymy sumę wartości funkcji celu całej populacji.

    for (unsigned x = 0; x < population_size; ++x) { // powtarzamy tyle razy ile wynosi wielkość populacji.

        number = generate_number() % fitness_sum; // losujemy liczbę z przedziału < 0, suma funkcji celu >
        sum = 0;
        count = 0;

        for (auto &i : population) { // iterujemy po elementach populacji
        
            if ((sum += i.get_fitness()) > number)  {//jeśli dotychczasowa suma funkcji celu jest większa niż zadana liczba
    
                temp.push_back(population[count]);  // wybierz osobnika
                break;
            }
            count++;
        }
    }

    population.clear();

    for (auto &i : temp)            //
        population.push_back(i);    // ładujemy nową populację. 
}

void gen_algorithm::selection_tournament() {

    std::vector<individual> temp;

    for( unsigned x = 0; x < population_size; ++x ) { // powtarzamy tyle razy ile wynosi wielkość populacji.

        unsigned temp1 = generate_number() % population_size; //
        unsigned temp2 = generate_number() % population_size; // losujemy 2 osobników z populacji.

        if( population[temp1] < population[temp2] ) // 
            temp.push_back(population[temp2]);      //
        else                                        //
            temp.push_back(population[temp1]);      // wybieramy lepszego.
    }

    population.clear(); // czyścimy dotychczasową populację.

    for( auto & i : temp )          //
        population.push_back(i);    // ładujemy nową populację.                              
}

void gen_algorithm::selection_tournament_chromosome() {

    std::vector<Chromosome> temp;

    for(unsigned x = 0; x < population_size; ++x) { // powtarzamy tyle razy ile wynosi wielkość populacji.

        unsigned temp1 = generate_number() % population_size; //
        unsigned temp2 = generate_number() % population_size; // losujemy 2 osobników z populacji.

        if( population_chromosome[temp1] < population_chromosome[temp2] ) // 
            temp.push_back(population_chromosome[temp2]);      //
        else                                        //
            temp.push_back(population_chromosome[temp1]);      // wybieramy lepszego.
    }

    population_chromosome.clear(); // czyścimy dotychczasową populację.

    for( auto & i : temp )          //
        population_chromosome.push_back(i);    // ładujemy nową populację.                              
}

individual gen_algorithm::start() {

    initPopulation();
    fintess_calc();
    for (unsigned i = 0; i < iteration_count; ++i)
    {
        selection();
        crossChromosome();
        mutate();
        fintess_calc();
        //std::cout << "lewy: " << l_best_so_far;
        //std::cout << "prawy: " << r_best_so_far;
        //std::cout << best_so_far;
    }
    return best_so_far;
}

void gen_algorithm::load_data() {

    pugi::xml_document doc;
    pugi::xml_node tools = doc.child("network").child("demands");
    int element_counter = 0;
    int path_counter = 0;
    int link_counter = 0;

    for (pugi::xml_node_iterator it = tools.begin(); it != tools.end(); ++it) {
        
        demands.push_back(std::stoi(it->child("demandValue").child_value()));
        pugi::xml_node path = it->child("admissiblePaths");
        std::vector<std::vector<std::vector<unsigned>>> tmp_path;

        for (pugi::xml_node_iterator pit = path.begin(); pit != path.end(); ++pit) {
            
            std::vector<std::vector<unsigned>> tmp_links;

            for(pugi::xml_node_iterator lit = pit->children().begin(); lit != pit->children().end(); ++lit) {

                unsigned a;
                unsigned b;
                bool pierwsze = true;
                std::string a_char;
                std::string b_char;
                int pomin = 5;
                std::string tmp_string = lit->child_value();

                for(auto &it : tmp_string) {
                    
                    if(pomin <= 0) {
                        
                        if(it == '_')
                            pierwsze = false;

                        if(it != '_') {
                            
                            if(pierwsze)
                                a_char += it;
                            
                            else
                                b_char += it;
                        }
                    }
                    pomin--;
                }

                a = std::stoi(a_char);
                b = std::stoi(b_char);
                std::vector<unsigned> tmp_link;

                if(a < b) {
                    
                    tmp_link.push_back(a);
                    tmp_link.push_back(b);
                }

                else {
                    
                    tmp_link.push_back(b);
                    tmp_link.push_back(a);
                }

                tmp_links.push_back(tmp_link);
            }
            tmp_path.push_back(tmp_links);
        }
        demand_paths.push_back(tmp_path);
    }
}