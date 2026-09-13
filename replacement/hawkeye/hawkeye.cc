// hawkeye.cc
#include "hawkeye.h"
#include <algorithm>

/* TODO: implement find_victim / replacement_cache_fill /
update_replacement_state here, delegating to optgen / predictor / rrip.h as
described in hawkeye.h above. */

/*Note all this is to update rrpv and train predictor after knowing if its actual hit/miss.
Purpose if to know which line to evict mimicing OPT. 
On every access, pass line and set to OPTgen. It checks if hit or miss under OPT.
Then train precictor using OPTgen result using the ip(PC). 
If the line is actual hit, we need to update the rrpv for that line by passing what the 
predictor predicted(classification) for that pc and the actual result (hit) to rrpv updater. 
If miss, champ may want to replace. Finding victim is simple, and we need to update 
rrpv for the newly inserted line based on precictor classification */

//For replacement find most cache averse line
long hawkeye::find_victim (uint32_t triggering_cpu, uint64_t instr_id, long set, const champsim::cache_block* current_set, 
				champsim::address ip, champsim::address full_addr, access_type type) 
{
	return static_cast<long> (::find_victim(rrpv[static_cast<std::size_t>(set)]));
}

void hawkeye::replacement_cache_fill (uint32_t triggering_cpu, long set, long way, champsim::address full_addr, champsim::address ip, 
							champsim::address victim_addr, access_type type)
{
	//ip is pc. 
	/* Actual miss so update rrpv accordingly. Needs what predictor classifies current
	PC and actual hit/miss */
	/*Notice that current PC would be trained when it was a "prev reference of some line.
	OPTgen would know whether it caused hit or miss and was trained accordingly. 
	And now we are using that prediction.*/
	/*From the table notice that we mostly re */
	Classification cls;
	if (predictor.predict(ip.to<uint64_t>())) cls = Classification::CACHE_FRIENDLY;
	else cls = Classification::CACHE_AVERSE;

	update_rrpv(rrpv[static_cast<std::size_t>(set)], static_cast<std::size_t>(way), cls, false);
}


void hawkeye::update_replacement_state (uint32_t triggering_cpu, long set, long way, champsim::address full_addr, champsim::address ip,
                            champsim::address victim_addr, access_type type, uint8_t hit) 
{
	//find line address/number from full address
	uint64_t line_addr = full_addr.to<uint64_t>() >> 6; //64B lines

	//optgen returns whether this line access will be hit/miss under OPT
	bool opt_hit = optgen.access(static_cast<std::size_t>(set), line_addr);

	//Track last pc which accessed current line
	if (last_pc[static_cast<std::size_t>(set)].find(line_addr) != last_pc[static_cast<std::size_t>(set)].end()) {
		//some pc previously accessed the line.
        uint64_t prev_pc = last_pc[static_cast<std::size_t>(set)][line_addr];
        /*Optgen says whether current access was hit/miss under OPT. Train previous
        pc that accessed line accordingly.ex: OPTgen says hit then prev was friendly*/
        predictor.train(prev_pc, opt_hit);
    }

    //Update last pc that accessed current line
    last_pc[static_cast<std::size_t>(set)][line_addr] = ip.to<uint64_t>();

    /*Took care of previous reference and trained predictor. but now need to mark
    current line averse or friendly to help in eviction. (forget about last reference)*/
    /*If current access to line was actual hit, need to update rrpv for the line.
    For that we use: 1. How did predictor classified the pc and 2. the fact that
    its an actual hit. Update using table in paper.*/
	if (hit) {
		//Find predictor classification for the pc
		Classification cls;
		if (predictor.predict(ip.to<uint64_t>())) cls = Classification::CACHE_FRIENDLY;
		else cls = Classification::CACHE_AVERSE;

		update_rrpv(rrpv[static_cast<std::size_t>(set)], static_cast<std::size_t>(way), cls, true);
	}
}