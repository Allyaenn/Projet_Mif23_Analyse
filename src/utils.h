#ifndef _UTILS_H
#define _UTILS_H

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
#include <algorithm>
#include "Bloc_Region.h"

using namespace cv;
using namespace std;
using namespace chrono;

/**
 * Usage of the program
 */
void usage();

/**
 * Wrong size of a picture
 */
void wrongFormat();

/**
* Calcul of a spatial smoothing on a greyscale picture
* kernel = average
*/
Mat spatialSmoothingAvg(const Mat & image, double lambda);

/**
* Calcul of a spatial smoothing in a colored (3 canals) picture
* kernel = average
*/
Mat spatialSmoothingAvgColor(const Mat & image, double lambda);

/**
* Calcul of a spatial smoothing on a greyscale picture
* kernel = Gaussian
*/
Mat spatialSmoothingGauss(const Mat & image, double sigma);

/**
* Calcul of a spatial smoothing in a colored picture
* kernel = Gaussian
*/
Mat spatialSmoothingGaussColor(const Mat & image, double sigma);

/***
* Calcul of a spatial smoothing on a greyscale picture
* kernel = Exponential
*/
Mat spatialSmoothingExp(const Mat & image, double gamma);

/**
* Calcul of a spatial smoothing on a colored picture
* kernel = Exponential
*/
Mat spatialSmoothingExpColor(const Mat & image, double gamma);

/**
 * Temporal smoothing for a colored picture
 */
Mat temporalSmoothingColor(String filename);

/**
 * Temporal smoothing for a greyscale picture
 */
Mat temporalSmoothing(String filename);

/**
 * Extract the moving element of the @frame (colored picture)
 * by comparing it to the @background picture
 * @backgound picture of the background
 * @frame picture to analyse
 * @return a picture of the extracted foreground
 */
Mat extractForegroundColor(const Mat & background, const Mat & frame);

/**
 * Extract the moving element of the @frame (greyscale picture)
 * by comparing it to the @background picture
 * @backgound picture of the background
 * @frame picture to analyse
 * @return a picture of the extracted foreground
 */
Mat extractForeground(const Mat & background, const Mat & frame);

/**
 * Post extraction smoothing with counting of extracted neighbours pixels
 */
Mat preciseSmoothing(Mat image, int nbrVoisin, int requis);

/**
* Split the picture in different blocs (first part of the Split and Merge method)
*/
std::list<Bloc*> split(const Mat & image, unsigned short int tabCarres [], double seuil);

/**
* Merge the blocs in several regions (second part of the Split and Merge method)
*/
std::list<Region*> merge(const std::list<Bloc*> blocs, const Mat & image, const unsigned short int tabCarres [], double seuil);

/**
* Coloring of pixels depending on their color
* The aim of this fonction is to realize the bodypart segmentation
* This function is not usable, it was a test to see 
* if the body part segementation was possible with our split and merge results.
* We did not succeed because the region colors were to close from each other
*/
void detectBodyParts(const std::list<Region*> regions, const Mat & image);

#endif

