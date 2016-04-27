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
	if(argc < 3){
		usage();
		return EXIT_FAILURE;
	}

	/**Nom des fichiers vidéos à exploiter*/
	string filename_bg = String(argv[1]);
	string filename_ps = String(argv[2]);

	/** Image actuelle*/
	Mat frame;
	/** Image de fond*/
	Mat background;
	/** Video*/
	VideoCapture vc = VideoCapture(filename);
	vc >> frame;

	
	VideoCapture vc = VideoCapture(filename_bg);
	vc >> background;
	spatialSmoothingAvgColor(background, 1);
	
	extractForeground(background, filename_ps);
	
	vc.release();
		
	return EXIT_SUCCESS;
}
