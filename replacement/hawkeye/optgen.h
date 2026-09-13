#ifndef OPTGEN_H
#define OPTGEN_H

#include <cstddef>
#include <cstdint>
#include <vector>
#include <deque>

class OPTgen {
public:
    // num_sets: number of cache sets tracked independently
    // associativity: W, the cache associativity
    // history_multiplier: history length multiplier (default 8)
    OPTgen(std::size_t num_sets,
           std::size_t associativity,
           std::size_t history_multiplier = 8);

    // Processes one access to address, mapped to set set_idx,
    //Return true if OPT determines hit, false if miss
    bool access(std::size_t set_idx, uint64_t address);

private:
    //data members and helper fns
    bool check_occupancy(std::size_t set_idx, std::size_t previous_index);
    void increment_occupancy(std::size_t set_idx, std::size_t previous_index);

    std::vector<std::deque<int>> occupancy_vectors;    //Each entry has occupancy vector(1 per set), stores "current" capacity of the set
    std::vector<std::deque<uint64_t>> access_history;  //Each entry has deque, stores access history or each set

    //For access()
    std::size_t associativity;
    std::size_t history_multiplier;
};

#endif