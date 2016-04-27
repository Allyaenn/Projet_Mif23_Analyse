/**
 * GONNOT Alix
 * PICARD Colas
 *
 * Main of projet Analyse
 * 
 * Step 1 : Foreground extraction
 * Step 2 : Body part Segmentation
 *
*/
#include "utils.h"
#include <stdio.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <string.h>
#include <iostream>
#include <math.h>
#include <list>

using namespace cv;
using namespace std;


/**
 * Main Function
 * @argc number of arg
 * @argv array of arg
 * 
 */
int main(int argc, char ** argv){

	/**
	 * Argument recuperation
	 */
	if(argc < 2){
		usage();
		return EXIT_FAILURE;
	}

	/**Nom du fichier vidéo à exploiter*/
	string filename = String(argv[1]);
	/** Image actuelle*/
	Mat frame;
	/** Image de fond*/
	Mat background;
	/**	 Images transformées*/
	Mat filterdFrame1, filterdFrame2;
	/** Video*/
	VideoCapture vc = VideoCapture(filename);
	vc >> frame;

	/** Passage de l'image en niveau de gris */
	cvtColor(frame, filterdFrame1, CV_RGB2GRAY);
	/** Filtre (Moyenne)*/
	spatialSmoothingAvg(filterdFrame1, 1);
	filterdFrame2 = frame.clone();
	spatialSmoothingAvgColor(filterdFrame2, 1);


	/** Fenetres d'affichages*/
	namedWindow("Originale", 1);
	namedWindow("Filtree gris", 1);
	namedWindow("Filtree couleur", 1);
	
	/** Caractère de récupération */
	char c;
	c = (char)waitKey(30);

	/**
	 * Boucle d'éxécution
	 * Application à la vidéo entière
	 */
	while(c != 'q' && !frame.empty())
	{
		imshow("Filtree gris", filterdFrame1);
		imshow("Filtree couleur", filterdFrame2);
		imshow("Originale", frame);
		vc >> frame;
		cvtColor(frame, filterdFrame1, CV_RGB2GRAY);
		spatialSmoothingAvg(filterdFrame1, 1);
		filterdFrame2 = frame.clone();
		spatialSmoothingAvgColor(filterdFrame2, 1);
		
		c = (char)waitKey(30);

	}
	vc.release();
		
	return EXIT_SUCCESS;
}
