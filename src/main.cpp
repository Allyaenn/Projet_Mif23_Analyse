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

#include "utils.h"

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
	/** Image de fond lissées temporellement*/
    Mat background = temporalSmoothingColor(filename_bg);
    Mat bg_NB = temporalSmoothing(filename_bg);
    
    /**Image de  fond non lissées*/
    Mat background2;
    Mat bg_NB2;
    /** Image extraite*/
    Mat perso;
    Mat perso_non_lissee;
	Mat lisse;
	/** Video*/
    VideoCapture vc = VideoCapture(filename_bg);
    vc >> background2;
    cvtColor(background2, bg_NB2, CV_BGR2GRAY);

    VideoCapture vcP = VideoCapture(filename_ps);
    vcP >> frame;

//    namedWindow("Perso", 1);
//	namedWindow("Background", 1);
//	namedWindow("Background_non_lissée", 1);

	spatialSmoothingGaussColor(background, 1);
    char c;
    c = (char)waitKey(1);
    steady_clock::time_point start, end;
    while(c != 'q' && !frame.empty())
    {
    	start = steady_clock::now();
//    	cvtColor(frame, frame_NB, CV_BGR2GRAY);
//    	perso = frame_NB;
        spatialSmoothingGaussColor(frame, 1);
       //	spatialSmoothingGauss(frame_NB,1);
        perso = extractForegroundColor(background, frame);
        //perso = extractForeground(bg_NB, frame_NB);
        //perso = frame;
        //perso = frame_NB;
        imshow("Perso", perso);
//        imshow("Background", background);
//        imshow("Background_non_lissée", background2);
		//lisse = lissageCouleur(perso, 4, 18);
		//imshow("Lisse", lisse);
        end = steady_clock::now();
        std::cout<<"time : "<< duration_cast<milliseconds>(end-start).count()<<std::endl;
        c = (char)waitKey(1);
        vcP >> frame;
    }

	//lisse = lissageCouleur(perso, 4, 18);
    splitAndMerge(perso);
    char d;
    d = (char)waitKey(1);
    namedWindow("Test", 1);
    while(d != 'q')
    {
   		 imshow("Test", perso);
   		 d = (char)waitKey(1);
   	}
   	
	return EXIT_SUCCESS;
}
