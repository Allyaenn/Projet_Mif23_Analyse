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
	int x; //x coordinate of the pixel (column)
	int y; //y coordinate of the pixel (line)
	
	pixel(){} 
	pixel(int i, int j) : x(i), y(j) {} 
};

class Bloc{

	private : 
	
		 pixel p_hg; // top-left pixel of the bloc
		 pixel p_bd; // bottom-right pixel of the bloc
		 int nb_pixels; // number of pixels stored in the bloc (without background pixels)
		 double varB; // BLUE canal variance
		 double varG; // GREEN canal variance
		 double varR; // RED canal variance
		 std::list<Bloc*> voisins; // neighbours (8-connected)

	public :
	 
		/**
		* Default constructor
		*/
		Bloc();
		
		/**
		* Constructor with 3 parameters
		*/
		Bloc(pixel hg, pixel bd, std::list<Bloc*> v);
		
		/**
		* Constructor with 2 parameters
		*/
		Bloc(pixel hg, pixel bd);
		
		/**
		* Getter on neighbours list
		*/
		std::list<Bloc*> & getVoisins();
		
		/**
		* Getter on top-left pixel
		*/
		const pixel & getP_hg();
		
		/**
		* Getter on bottom-right pixel
		*/
		const pixel & getP_bd();
		
		/**
		* Getter on variances
		*/
		double * getVar();
		
		/**
		* Override of equality operator
		*/
		bool operator==(const Bloc & b);
		
		/**
		* Function which goal is to determine wether th bloc b and the current bloc are neighbours or not
		*/
		bool estVoisin(const Bloc & b);
		
		/**
		* Function which goal is to determine if the current bloc has to be splitted
		* The segementation criteria is the variance of the value (for each color) of each pixel 
		* in the picture
		*/
		bool hasToBeSplitted(const Mat & image, const unsigned short int tabCarres [], double seuil);

};

class Region {

	private : 
		std::list<Bloc*> blocs; // blocs composing the region
		double varB; // BLUE canal variance
		double varG; // GREEN canal variance
		double varR; // RED canal variance
		
		double moyB; // BLUE canal mean
		double moyG; // GREEN canal mean
		double moyR; // RED canal mean
		
	public : 
	
		/**
		* Default constructor
		*/
		Region();
		
		/**
		* Function adding a bloc in the region
		*/
		void addBloc(Bloc * b);
		
		/**
		* Getter on blocs list
		*/
		const std::list<Bloc*> & getBlocs();
		
		/**
		* Getter on the means
		*/
		double * getMoy(); 
		
		/**
		* Function which goal is to update the variances stored in the current region
		* according to the blocs composing it
		*/
		void updateVar(const Mat & image, const unsigned short int tabCarres []);
		
		/**
		* Function which goal is to determine if the union between the current region and 
		* the bloc b results in an homogeneous region.
		* The fusion criteria is the difference between the variance of the region and of the bloc 
		* (for each canal). It has to be beneath the "seuil" (threshold).
		*/
		bool isConsistent(Bloc & b, double seuil);
		
};

#endif
