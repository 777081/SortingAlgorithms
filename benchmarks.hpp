#include <fstream>
#include <vector>
#include <random>
#include <climits>
#include <string>
#include <iostream>
#include <chrono>

#include "intro_sort.hpp"
#include "merge_sort.hpp"
#include "quick_sort.hpp"

const vector<int> MEASUREMENT_POINTS = {500000, 1000000};
const vector<double> TEST_FRACTIONS = {0.25, 0.5, 0.75, 0.95, 0.99, 0.997};
const int SEEDS = 100;

template <typename T> std::vector<T> populate_structure(int seed, int size){
    std::mt19937 rng(seed);
    std::uniform_int_distribution<int> dist(INT_MIN, INT_MAX); 

    std::vector<T> structure;

    for(int i = 0; i < size; ++i){
        structure.push_back(dist(rng));
    }

    return structure;
}

template <typename T> std::vector<T> partially_sort(std::vector<T> structure, double sort_fraction, int shuffle_seed){

    std::mt19937 rng(shuffle_seed);
    std::vector<T> temp(structure);
    int cutoff = static_cast<int>(temp.size()*sort_fraction);

    
    std::sort(temp.begin(), temp.end());

    std::shuffle(temp.begin() + cutoff, temp.end(), rng);

    return temp;
}

template <typename T, typename Func>
double run_test(std::vector<std::vector<T>> measurement_point, Func sorting_function, bool pre_sort = false, double sort_fraction = 1, bool reverse = false){
    std::vector<std::vector<T>> work_copy;

    if(pre_sort){
        for(int i = 0; i < SEEDS; ++i){
            work_copy.push_back(partially_sort(measurement_point.at(i), sort_fraction, i));
        }
    }
    else if(reverse){
        work_copy = measurement_point;
        for(int i = 0; i < SEEDS ; ++i){
            std::sort(work_copy[i].begin(), work_copy[i].end(), std::greater<>());
        }
    }
    else{
        work_copy = measurement_point;
    }
    
    auto start = std::chrono::high_resolution_clock::now();
    for(int i = 0; i < SEEDS; ++i){
        sorting_function(work_copy.at(i).data(), 0, work_copy.at(i).size()-1);
    }
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double>(end - start).count() / SEEDS;
}


template <typename T> void run_test_suite(){
    std::cout << "Generating test data\n";
    int max_mpoint = *max_element(MEASUREMENT_POINTS.begin(), MEASUREMENT_POINTS.end());
    std::vector<std::vector<T>> measurement_data;
    for(int i = 0; i < SEEDS; ++i){
        measurement_data.push_back(populate_structure<T>(i, max_mpoint));
    }
    std::cout << "Test data generated\n";
    std::vector<std::vector<T>> current_measurement(SEEDS);

    for(int i = 0; i < MEASUREMENT_POINTS.size(); ++i){
        std::cout << "\n====== Starting tests for measurement size " << MEASUREMENT_POINTS.at(i) << " =====\n\n"; 
        std::fstream measurement_file("size" + std::to_string(MEASUREMENT_POINTS[i]) + ".csv", std::ios::out);
        measurement_file << "alg, full_random, ";
        for(int i = 0; i < TEST_FRACTIONS.size(); ++i){
            measurement_file << TEST_FRACTIONS[i] << ", ";
        }
        measurement_file << "reverse\n";

        std::cout << "Truncating test data to measurement size\n";
        for(int j = 0; j < SEEDS; ++j){
            current_measurement[j] = std::vector<T>(measurement_data.at(j).begin(), measurement_data.at(j).begin() + MEASUREMENT_POINTS.at(i));
        }
        std::cout << "Data truncated\n";


        //merge sort tests
        std::cout << "== Starting merge sort ==\n";


        std::cout << "unsorted ";

        measurement_file << "merge_sort, " << run_test(
            current_measurement,
            merge_sort<T>
        ) << ", ";
        std::cout << "done\n";


        for(int i = 0; i < TEST_FRACTIONS.size(); ++i){
            std::cout << TEST_FRACTIONS.at(i);

            measurement_file << run_test(
                current_measurement,
                merge_sort<T>,
                true,
                TEST_FRACTIONS.at(i)
            ) << ", ";

            std::cout << " done\n";

        }

        std::cout << "reverse ";
        measurement_file << run_test(current_measurement, merge_sort<T>, false, 1, true) << "\n";
        std::cout << "done\n";



        //quick sort tests
        std::cout << "== Starting quick sort ==\n";

        std::cout << "unsorted ";

        measurement_file << "quick_sort, " << run_test(
            current_measurement,
            quick_sort<T>
        ) << ", ";
        std::cout << "done\n";


        for(int i = 0; i < TEST_FRACTIONS.size(); ++i){
            std::cout << TEST_FRACTIONS.at(i);

            measurement_file << run_test(
                current_measurement,
                quick_sort<T>,
                true,
                TEST_FRACTIONS.at(i)
            ) << ", ";

            std::cout << " done\n";

        }

        std::cout << "reverse ";
        measurement_file << run_test(current_measurement, quick_sort<T>, false, 1, true) << "\n";
        std::cout << "done\n";



        //intro sort tests
        std::cout << "== Starting intro sort ==\n";

        std::cout << "unsorted ";

        measurement_file << "intro_sort, " << run_test(
            current_measurement,
            intro_sort<T>
        ) << ", ";
        std::cout << "done\n";


        for(int i = 0; i < TEST_FRACTIONS.size(); ++i){
            std::cout << TEST_FRACTIONS.at(i);

            measurement_file << run_test(
                current_measurement,
                intro_sort<T>,
                true,
                TEST_FRACTIONS.at(i)
            ) << ", ";

            std::cout << " done\n";

        }

        std::cout << "reverse ";
        measurement_file << run_test(current_measurement, intro_sort<T>, false, 1, true) << "\n";
        std::cout << "done\n";
        measurement_file.close();
    }
}