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
	int x; //coordonnées x du pixel
	int y; //coordonnées y du pixel
	
	pixel(){} 
	pixel(int i, int j) : x(i), y(j) {} 
};

class Bloc{

	private : 
	
		 pixel p_hg; // représente le pixel en haut a gauche du bloc (comportant xmin et ymin)
		 pixel p_bd; // représente le pixel en bas a droite du bloc (comportant xmax et ymax)
		 int nb_pixels; // donne le nombre de pixels stockés dans la région (les pixels de l'arrière plan ne sont pas pris en compte)
		 double varB; // la variance pour le canal BLUE
		 double varG; // la variance pour le canal GREEN
		 double varR; // la variance pour le canal RED
		 std::list<Bloc*> voisins; // les voisins

	public :
	 
		/**
		* Constructeur par défaut
		*/
		Bloc();
		
		/**
		* Constructeur avec 3 paramètres
		*/
		Bloc(pixel hg, pixel bd, std::list<Bloc*> v);
		
		/**
		* Constructeur avec 2 paramètres
		*/
		Bloc(pixel hg, pixel bd);
		
		/**
		* Getter sur la liste des voisins
		*/
		std::list<Bloc*> & getVoisins();
		
		const pixel & getP_hg();
		
		const pixel & getP_bd();
		
		double * getVar();
		
		/**
		* Surcharge de l'opérateur d'égalité 
		*/
		bool operator==(const Bloc & b);
		
		/**
		* Fonction permettant de determiner si le bloc b et le bloc courant sont voisins 
		* vis à vis de leur coordonnées
		*/
		bool estVoisin(const Bloc & b);
		
		/**
		* Fonction qui détermine si le bloc doit être divisé en 4 ou non.
		* Le critère de segmentation est la variance de la valeur pour chaque pixel du bloc, 
		* elle est calculée pour chaque canal de l'image.
		*/
		bool hasToBeSplitted(const Mat & image, const unsigned short int tabCarres [], double seuil);

};

class Region {

	private : 
		std::list<Bloc*> blocs; // les blocs composant la région
		double varB; // la variance pour le canal BLUE
		double varG; // la variance pour le canal GREEN
		double varR; // la variance pour le canal RED
		
		double moyB; // la moyenne pour le canal BLUE
		double moyG; // la moyenne pour le canal GREEN
		double moyR; // la moyenne pour le canal RED
		
	public : 
	
		/**
		* Constructeur par défaut
		*/
		Region();
		
		/**
		* Fonction permettant d'ajouter un bloc dans une région
		*/
		void addBloc(Bloc * b);
		
		/**
		* Getter sur la liste des blocs composant une région
		*/
		const std::list<Bloc*> & getBlocs();
		
		/**
		* Getter sur les moyennes des valeur des pixels de la région
		*/
		double * getMoy(); 
		
		/**
		* Fonction permettant de mettre à jour les variances et moyennes de la région
		*/
		void updateVar(const Mat & image, const unsigned short int tabCarres []);
		
		/**
		* Fonction permettant de determiner si l'union d'un bloc et d'une région
		* résulte en une nouvelle région homogène ou non
		*/
		bool isConsistent(Bloc & b, double seuil);
		
};

#endif
