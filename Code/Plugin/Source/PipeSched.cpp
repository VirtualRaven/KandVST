#include "PipeSched.h"
#include "Pipeline.h"
#include "PipelineManager.h"
#include <iostream>     // std::cout
#include <iterator>     // std::advance
#include <list>         // std::list

PipeSched::PipeSched() :

{

}

void PipeSched::assign(std::list<Pipeline> pipList) {
	std::list<Pipeline>::iterator it;
	for(it = pipList.begin(), it != pipList.end(), it++) {
		if (*it->isActive()) {
			continue;
		}
		else 
			if(it->mtx.try_lock()){
				//do stuff 
				it->mtx.unlock();
				break;
			}
			else {
				continue;	
			}
	}

}


PipeSched::~PipeSched()
{

}

