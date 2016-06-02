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
#include <ctime>
#include <chrono>

#include "constantes.h"

using namespace cv;
using namespace std;
using namespace chrono;

struct pixel {
	int x;
	int y;
	
	pixel(){} 
	pixel(int i, int j) : x(i), y(j) {} 
};

class Bloc{

	public : 
		 pixel p_hg; // représente le pixel en haut a gauche du bloc (comportant xmin et ymin)
		 pixel p_bd; // représente le pixel en bas a droite du bloc (comportant xmax et ymax)
		 int nb_pixels; // donne le nombre de pixels stockés dans la région (les pixels de l'arrière plan ne sont pas pris en compte)
		 double valeur; // la variance
		 std::list<Bloc*> voisins; // les voisins

		Bloc();
		
		Bloc(pixel hg, pixel bd, std::list<Bloc*> v);
		
		Bloc(pixel hg, pixel bd);
		
		const std::list<Bloc*> & getVoisins();
		
		bool operator==(const Bloc & b);
		
		bool estVoisin(const Bloc & b);
		
		//calcule la valeur (variance) associée à la région selon l'image passée en paramètres
		//double calcule_valeur(Mat image);
		
		//sépare le bloc en 4 sous-blocs et réparti les voisins en fonction de la disposition
		bool hasToBeSplitted(const Mat & image, const unsigned short int tabCarres []);
};

class Region {

	private : 
		std::list<Bloc*> blocs;
		
	public : 
	
		Region();
		
		void addBloc(Bloc * b);
		
};

#endif
