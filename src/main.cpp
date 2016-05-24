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
#include <ctime>
#include <chrono>



using namespace cv;
using namespace std;

using namespace chrono;
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
	Mat frame_NB;
	/** Image de fond*/
    Mat background;// = temporalSmoothing(filename_bg);
    Mat bg_NB;
    /** Image extraite*/
    Mat perso;
	/** Video*/
    VideoCapture vc = VideoCapture(filename_bg);
    vc >> background;
    cvtColor(background, bg_NB, CV_BGR2GRAY);

    VideoCapture vcP = VideoCapture(filename_ps);
    vcP >> frame;

    namedWindow("Perso", 1);

	spatialSmoothingAvgColor(background, 3);
    char c;
    c = (char)waitKey(1);
    steady_clock::time_point start, end;
    while(c != 'q' && !frame.empty())
    {
    	start = steady_clock::now();
    	cvtColor(frame, frame_NB, CV_BGR2GRAY);
        spatialSmoothingAvgColor(frame, 3);
        perso = extractForegroundColor(background, frame);
        //perso = frame;
        imshow("Perso", perso);
        end = steady_clock::now();
        std::cout<<"time : "<< duration_cast<milliseconds>(end-start).count()<<std::endl;
        c = (char)waitKey(1);
        vcP >> frame;
    }
    
    splitAndMerge(perso);
		
	return EXIT_SUCCESS;
}
