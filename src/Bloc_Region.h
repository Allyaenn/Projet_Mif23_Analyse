#ifndef _BLOC_REGION_H
#define _BLOC_REGION_H

#include <stdio.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <cstring>
#include <iostream>
#include <cmath>
#include <list>

struct pixel {
	int x;
	int y;
};

class Bloc{

	private :
		 pixel p_hg;
		 pixel p_bd;
		 double valeur;
		 std::list<Bloc*> voisins;
	
	public : 
	
		Bloc();
		
		Bloc(pixel hg, pixel bd, std::list<Bloc*> v);
		
		Bloc(pixel hg, pixel bd);
		
		//calcule la valeur (variance) associée à la région selon l'image passée en paramètres
		//double calcule_valeur(Mat image);
		
		//sépare le bloc en 4 sous-blocs et réparti les voisins en fonction de la disposition
		void split(std::list<Bloc*> & bloc);
};

class Region {

	private : 
		std::list<Bloc*> blocs;
		
	public : 
	
		Region();
		
};

#endif
