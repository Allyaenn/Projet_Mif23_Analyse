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
	string filename = String(argv[1]);

	return EXIT_SUCCESS;
}
