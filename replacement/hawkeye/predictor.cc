#include "predictor.h"

HawkeyePredictor::HawkeyePredictor(
    std::size_t num_entries,
    int counter_bits) : num_entries(num_entries),
    counter_bits(counter_bits),
    predictor_table(num_entries, 4)
    
{
}

/*OPTgen says whether current line hit/miss under OPT. If hit the last access 
was cache friendly else averse. So train last pc accordingly. */
void HawkeyePredictor::train(uint64_t pc, bool opt_hit)
{
    std::size_t hashed_pc = hash_pc(pc);
    if (opt_hit) {
        if (predictor_table[hashed_pc] < 7) predictor_table[hashed_pc]++;
    }  
    else {
        if (predictor_table[hashed_pc] > 0) predictor_table[hashed_pc]--;
    }
}

//Return trained value for any pc
bool HawkeyePredictor::predict(uint64_t pc) const
{
    std::size_t hashed_pc = hash_pc(pc);
    int counter_value = predictor_table[hashed_pc];
    return counter_value & 0x4;
}

//Return the counter bits
int HawkeyePredictor::get_counter(uint64_t pc) const
{
    std::size_t hashed_pc = hash_pc(pc);
    return predictor_table[hashed_pc];
}

std::size_t HawkeyePredictor::hash_pc(uint64_t pc) const
{
    return (pc ^ (pc >> 12)) & 0x1FFF; //Return lower 13 bits of hashed pc
}