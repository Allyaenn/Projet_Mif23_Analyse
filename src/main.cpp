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

    /*Nom des fichiers vidéos à exploiter*/
	string filename_bg = String(argv[1]);
	string filename_ps = String(argv[2]);

	/** Image actuelle*/
	Mat frame;
	/** Image de fond*/
    Mat background;// = temporalSmoothing(filename_bg);
    /** Image extraite*/
    Mat perso;
	/** Video*/
    VideoCapture vc = VideoCapture(filename_bg);
    vc >> background;

    VideoCapture vcP = VideoCapture(filename_ps);
    vcP >> frame;

    namedWindow("Perso", 1);

	spatialSmoothingAvgColor(background, 1);
    char c;
    c = (char)waitKey(30);
    while(c != 'q' && !frame.empty())
    {
        spatialSmoothingAvgColor(frame, 1);
        perso = extractForeground(background, frame);
        imshow("Perso", perso);
        c = (char)waitKey(30);
        vcP >> frame;
    }
		
	return EXIT_SUCCESS;
}
