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

/*Calcul d'un lissage spatial 
  noyau = fonction exponentielle*/
Mat spatialSmoothingExp(cv::Mat image, double gamma);

/*Extraction des pixels au premier plan*/
void extractForeground(Mat image, String filename);

void splitAndMerge(Mat image);

