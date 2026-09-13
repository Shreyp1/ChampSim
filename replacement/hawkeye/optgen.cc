#include "optgen.h"

OPTgen::OPTgen(std::size_t num_sets, std::size_t associativity,
    std::size_t history_multiplier) : occupancy_vectors(num_sets), 
    access_history(num_sets), associativity(associativity), 
    history_multiplier(history_multiplier)
{
}
   
//returns whether the current line X be hit or miss under OPT  
bool OPTgen::access(std::size_t set_idx, uint64_t address) {
    /* For each access, check the last access to the same address
    if not found, access is new : update accordingly 
    If found, check if some entry = associativity.
    If yes then update it normally, if no, increment occupancy vector
    */
    
   //Check is last access exists or some entry = associativity
   bool hit = false;
    for (std::size_t i = access_history[set_idx].size(); i > 0; --i) {
        if (access_history[set_idx][i-1] == address) {
            //Found last reference, check occupancy vector(hit/miss)
            std::size_t previous_ref_index = i - 1;
            hit = check_occupancy(set_idx, previous_ref_index);
            if (hit) increment_occupancy(set_idx, previous_ref_index);
            break;
        }
    }
    if (access_history[set_idx].size() == history_multiplier * associativity) {
        //history is full
        access_history[set_idx].pop_front();
        occupancy_vectors[set_idx].pop_front();
    }

    access_history[set_idx].push_back(address);
    occupancy_vectors[set_idx].push_back(0);

    return hit;

}

bool OPTgen::check_occupancy(std::size_t set_idx, std::size_t previous_index)
{
    for (std::size_t i = occupancy_vectors[set_idx].size(); i > previous_index; --i) {
        if (occupancy_vectors[set_idx][i-1] >= associativity) return false;
    }
    return true;
}

void OPTgen::increment_occupancy(std::size_t set_idx, std::size_t previous_index)
{
    for (std::size_t i = occupancy_vectors[set_idx].size(); i > previous_index; i--) {
        occupancy_vectors[set_idx][i-1]++;
    }
}
