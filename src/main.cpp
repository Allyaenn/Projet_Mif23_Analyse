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
	
    /*name of the video file we will work on*/
	string filename_bg = String(argv[1]);
	string filename_ps = String(argv[2]);

	/** Frame extracted from the video*/
	Mat frame;
	
	/** Frame used to work on */
	Mat image;
	
	/** Background image temporally smoothed*/
    Mat background = temporalSmoothingColor(filename_bg);
    
	/** Video*/
    VideoCapture vcP = VideoCapture(filename_ps);
    vcP >> frame;

	// spatial smoothing of the background
	background = spatialSmoothingExpColor(background, 1);
	
    char c;
    c = (char)waitKey(1);
    while(c != 'q' && !frame.empty())
    {
       	image = spatialSmoothingExpColor(frame, 1);
       	image = extractForegroundColor(background, image);
		image = preciseSmoothing(image, 4, 28);
		imshow("Extracted Forgeround", image);
        c = (char)waitKey(1);
        //vcP >> frame;
    }
    
	unsigned short int tabCarres [image.rows*image.cols*3];
	for (int i = 0; i<image.rows*image.cols*3; i++)
		tabCarres[i] = 0;
    std::list<Bloc*> blocs = split(image, tabCarres, 400);
    std::list<Region*> regions =  merge(blocs, image, tabCarres, 1000);
    c = (char)waitKey(1);
    namedWindow("Segmented Image", 1);
    while(c != 'q')
    {
   		 imshow("Segmented Image", image);
   		 //detectBodyParts(regions, image);
   		 c = (char)waitKey(1);
   	}
   	
	return EXIT_SUCCESS;
}
