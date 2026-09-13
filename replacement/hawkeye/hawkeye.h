// hawkeye.h
#ifndef HAWKEYE_H
#define HAWKEYE_H
#include <vector>
#include "modules.h"
#include "optgen.h"
#include "predictor.h"
#include "rrip.h"
#include "cache.h"
#include <map>

struct hawkeye : public champsim::modules::replacement {
	// TODO: instantiate different modules
	// TODO: Add any new data structures or functions to connect each of
	//the modules

	OPTgen optgen;
  	HawkeyePredictor predictor;
  	// rrpv[set][way]: Array of rrpv for each line in each set (Champ does not maintain rrpv)
  	std::vector<std::vector<int>> rrpv;
  	//Need to track last pc that accessed X to train that pc instead of ip(current pc)
  	std::vector<std::map<uint64_t, uint64_t>> last_pc; //one map per set (line -> pc)

  	explicit hawkeye(CACHE* cache)
      : replacement(cache), optgen(cache->NUM_SET, cache->NUM_WAY), predictor(), 
      rrpv(cache->NUM_SET, std::vector<int>(cache->NUM_WAY, 0)), last_pc(cache->NUM_SET)
  	{
  	}
	

	// TODO: Complete the definitions for the following functions that are
	//required across all replacement policies. You can use the other replacement
	//policies as a reference. Each should be implemented primarily by calling
	//optgen.access(...), predictor.train(...)/predict(...), and
	//update_rrpv(...)/find_victim(...) from rrip.h, do not re-implement
	//OPTgen/predictor/RRIP logic here.

	long find_victim (uint32_t triggering_cpu, uint64_t instr_id, long set, const champsim::cache_block* current_set, champsim::address ip,
                   champsim::address full_addr, access_type type);

	void replacement_cache_fill (uint32_t triggering_cpu, long set, long way, champsim::address full_addr, champsim::address ip, champsim::address victim_addr,
                            access_type type);

	void update_replacement_state (uint32_t triggering_cpu, long set, long way, champsim::address full_addr, champsim::address ip,
                              champsim::address victim_addr, access_type type, uint8_t hit);
};
#endif