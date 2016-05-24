#include "Bloc_Region.h"

/***********************************BLOC***********************************************/
Bloc::Bloc(pixel hg, pixel bd, std::list<Bloc*> v) : p_hg(hg), p_bd(bd),voisins(v){}

Bloc::Bloc(pixel hg, pixel bd) : p_hg(hg), p_bd(bd){}
		
void Bloc::split(std::list<Bloc*> & bloc){
}

/***********************************REGION*********************************************/

Region::Region(){}
