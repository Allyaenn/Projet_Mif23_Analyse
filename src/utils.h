#include <stdio.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <cstring>
#include <iostream>
#include <cmath>
#include <list>

#include "Bloc_Region.h"

using namespace cv;
using namespace std;

/**
 * Usage of the program
 */
void usage();

void wrongFormat();

/*Calcul d'un lissage spatial 
  noyau = moyenne*/
Mat spatialSmoothingAvg(Mat image, double lambda);

Mat spatialSmoothingAvgColor(Mat image, double lambda);

/*Calcul d'un lissage spatial 
  noyau = Gaussienne*/
Mat spatialSmoothingGauss(Mat image, double sigma);

Mat spatialSmoothingGaussColor(Mat image, double sigma);

/*Calcul d'un lissage spatial 
  noyau = fonction exponentielle*/
Mat spatialSmoothingExp(cv::Mat image, double gamma);

Mat spatialSmoothingExpColor(cv::Mat image, double gamma);

/** 
 * Retourne vrai sil le pixel est situé à une certaine distance des bords de l'image
 * */
bool isScreenEdge(int coordX, int coordY, int maxX, int maxY, double distance);

/**
 * Renvoi vrai si la différence entre les couleurs des deux pixels est supérieure au seuil
 */
bool pixelColorDifference(Vec3b pixA, Vec3b pixB, double seuil);

/**Extraction des pixels au premier plan*/
Mat extractForegroundColor(Mat background, Mat frame);

Mat extractForeground(Mat background, Mat frame);

/**
 * Lissage temporel
 */
Mat temporalSmoothing(String filename);
/**
* Segementation de l'image par la méthode Split and Merge
* Le critère de segmentation/fusion se base sur la couleur
*/
void splitAndMerge(Mat image);

/**
 * Lissage par rapport aux n voisins
 */
Mat lissageCouleur(Mat image, int nbrVoisin, int requis);

