#include "rrip.h"

//According to table
void update_rrpv(std::vector<int>& rrpv, std::size_t way, Classification cls, bool is_hit)
{
	//way is just the line number in set
	if (cls == Classification::CACHE_AVERSE) rrpv[way] = 7;
	else if (is_hit) rrpv[way] = 0;
	else {
		rrpv[way] = 0;
		/*Avoids the case where all new access are miss but predicted cache friendly,
		marking their RRIP 0. Eventually all lines will be full with RRIP 0 and 
		when a new miss occurs, to evict a line, all friendly lines will be 
		incremented to 7 marking them averse. So when a cache friendly line is a
		miss, increase all other lines. This mimics LRU. (useful to flush out
		old stale data and prioratizing new data during program phase change*/
		for (std::size_t i = 0; i < rrpv.size(); ++i) {
			if (i != way && rrpv[i] < 6) rrpv[i]++;
		}
	}
}

//Verify if we have to increase all the age if no rrpv == 7 or return the next largest rrpv value
std::size_t find_victim(std::vector<int>& rrpv)
{
	while (true) {
		for (std::size_t i = 0; i < rrpv.size(); ++i) {
			if (rrpv[i] == 7)
				return i;
		}

		for (std::size_t i = 0; i < rrpv.size(); ++i) {
			if (rrpv[i] < 7)
				rrpv[i]++;
		}
	}
	
}