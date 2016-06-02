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

void wrongFormat();

/**
* Calcul d'un lissage spatial sur une image en niveaux de gris (1 canal)
* noyau = moyenne
*/
Mat spatialSmoothingAvg(const Mat & image, double lambda);

/**
* Calcul d'un lissage spatial sur une image couleur (3 canaux)
* noyau = moyenne
*/
Mat spatialSmoothingAvgColor(const Mat & image, double lambda);

/**
* Calcul d'un lissage spatial sur une image en niveaux de gris (1 canal)
* noyau = Gaussienne
*/
Mat spatialSmoothingGauss(const Mat & image, double sigma);

/**
* Calcul d'un lissage spatial sur une image couleur (3 canaux)
* noyau = Gaussienne
*/
Mat spatialSmoothingGaussColor(const Mat & image, double sigma);

/**
* Calcul d'un lissage spatial sur une image en niveaux de gris (1 canal)
* noyau = Exponentielle
*/
Mat spatialSmoothingExp(const Mat & image, double gamma);

/**
* Calcul d'un lissage spatial sur une image couleur (3 canaux)
* noyau = Exponentielle
*/
Mat spatialSmoothingExpColor(const Mat & image, double gamma);

/**
 * Lissage temporel pour image couleur
 */
Mat temporalSmoothingColor(String filename);

/**
 * Lissage temporel pour image en niveau de gris
 */
Mat temporalSmoothing(String filename);

/**
 * Extrait les éléments mouvants de @frame (image en couleurs)
 * en comparant par rapport à  l'image de fond passée dans l'argument @Background
 * @backgound Matrice contenant l'image de fond
 * @frame Matrice contenant l'image complete
 * @return les éléments mouvants extraits de frame
 */
Mat extractForegroundColor(const Mat & background, const Mat & frame);

/**
 * Extrait les éléments mouvants de @frame (image en niveaux de gris)
 * en comparant par rapport à  l'image de fond passée dans l'argument @Background
 * @backgound Matrice contenant l'image de fond
 * @frame Matrice contenant l'image complete
 * @return les éléments mouvants extraits de frame
 */
Mat extractForeground(const Mat & background, const Mat & frame);

/**
 * Lissage post-extraction par rapport aux voisins du pixel considéré
 */
Mat preciseSmoothing(Mat image, int nbrVoisin, int requis, Mat orig);

/**
* Split de l'image en différents blocs
*/
std::list<Bloc*> split(const Mat & image, unsigned short int tabCarres [], double seuil);

/**
* Merge des blocs en différentes régions
*/
std::list<Region*> merge(const std::list<Bloc*> blocs, const Mat & image, const unsigned short int tabCarres [], double seuil);
#endif

