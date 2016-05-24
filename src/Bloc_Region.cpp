#include "Bloc_Region.h"

/***********************************BLOC***********************************************/
Bloc::Bloc(){}

Bloc::Bloc(pixel hg, pixel bd, std::list<Bloc*> v) : p_hg(hg), p_bd(bd),voisins(v){}

Bloc::Bloc(pixel hg, pixel bd) : p_hg(hg), p_bd(bd){}

//double Bloc::calcule_valeur(Mat image){}
		
void Bloc::split(std::list<Bloc*> & bloc){
}

/***********************************REGION*********************************************/

Region::Region(){}
