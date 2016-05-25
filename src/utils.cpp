#include "utils.h"

/**
 * Message d'erreur en cas de mauvais appel au programme
 */
void usage(){
	cout << "Error : expected usage" << endl << "./Analyse <video>" << endl;
}

/**
 * Message d'erreur en cas de mauvaises dimensions
 */
void wrongFormat(){
	std::cout<<"Error  : wrong size for used pictures, they have to be of equal sizes"<<std::endl;
}

/**
 * Calcul d'un lissage spatial
  noyau = moyenne*/
Mat spatialSmoothingAvg(Mat image, double lambda)
{
	Mat copie = image;
	double h = 1.0/(pow(lambda,2));
	int lignes = image.rows;
	int colonnes = image.cols;
	int res0, res1, res2; //des int pour eviter le dépassement de capacité
	int coordMin = -lambda/2;
	int coordMax = lambda/2;
	for(int i = 1; i<lignes-1; i++)
	{
		for(int j = 1; j<colonnes-1; j++)
		{
			//calcul de la convolution pour chaque composante couleur
			res0 = res1 = res2 = 0;
			for (int u = coordMin; u <= coordMax; u++)
			{
				for (int v = coordMin; v <= coordMax; v++)
				{	
					res0 += image.data[(i+u)*colonnes+(j+v)];
				}
			} 
			copie.data[i*colonnes+j] = (unsigned char)(res0 * h); //on repasse en char
		}
	}
	return copie;
}

Mat spatialSmoothingAvgColor(Mat image, double lambda)
{
	Mat copie = image;
	
	double h = 1.0/(pow(lambda,2));
	int lignes = image.rows;
	int colonnes = image.cols;
	int res0, res1, res2;
	int lambdasur2 = lambda/2;
	int coordMin = -lambda/2;
	int coordMax = lambda/2;
	for(int i = lambdasur2; i<lignes-lambdasur2; i++)
	{
		for(int j = lambdasur2; j<colonnes-lambdasur2; j++)
		{
			//calcul de la convolution pour chaque composante couleur
			res0 = res1 = res2 = 0;
			for (int u = coordMin; u <= coordMax; u++)
			{
				for (int v = coordMin; v <= coordMax; v++)
				{
					res0 += image.data[(i+u)*colonnes*3+(j+v)*3+0];
					res1 += image.data[(i+u)*colonnes*3+(j+v)*3+1];
					res2 += image.data[(i+u)*colonnes*3+(j+v)*3+2];
				}
			} 
			copie.data[i*colonnes*3+j*3+0] = res0 * h;
			copie.data[i*colonnes*3+j*3+1] = res1 * h;
			copie.data[i*colonnes*3+j*3+2] = res2 * h;
		}
	}
	return copie;
}


/**
 * Calcul d'un lissage spatial
  noyau = Gaussienne*/
Mat spatialSmoothingGauss(Mat image, double sigma)
{
	Mat copie = image;
	int lignes = image.rows;
	int colonnes = image.cols;
	double res;
	const double pi = 3.14159265358979323846;
	double coeff =  (1/(2*pi*pow(sigma,2)));
	double unsur2sig = 1/(2*sigma);
	double h;
	
	std::vector<double> refu_v;
	
	for(int i = 0; i<3; i++)
	{
		for(int j = 0; j < 3; j++)
		{
			refu_v.push_back(coeff *exp((-pow(i-1,2)-pow(j-1,2))*unsur2sig));
		}
	}
	
	for(int i = 1; i<lignes-1; i++)
	{
		for(int j = 1; j < colonnes-1; j++)
		{
				res = 0;
				for (int u = -1; u < 2; u++)
				{
					for (int v = -1; v < 2; v++)
					{	
						h = refu_v[(u+1)*3+(v+1)];
						res += h*(image.data[(i+u)*colonnes+(j+v)]);
					}
				} 
				copie.data[i*colonnes+j] = (unsigned char)(res);
		}
	}
	return copie;	
}

//BGR
Mat spatialSmoothingGaussColor(Mat image, double sigma)
{
	Mat copie = image;
	int lignes = image.rows;
	int colonnes = image.cols;
	double res0, res1, res2;
	const double pi = 3.14159265358979323846;
	double coeff =  (1/(2*pi*pow(sigma,2)));
	double unsur2sig = 1/(2*sigma);
	double h;
	
	std::vector<double> refu_v;
	
	for(int i = 0; i<3; i++)
	{
		for(int j = 0; j < 3; j++)
		{
			refu_v.push_back(coeff *exp((-pow(i-1,2)-pow(j-1,2))*unsur2sig));
		}
	}
	
	for(int i = 1; i<lignes-1; i++)
	{
		for(int j = 1; j < colonnes-1; j++)
		{
				res0 = res1 = res2 = 0;
				for (int u = -1; u < 2; u++)
				{
					for (int v = -1; v < 2; v++)
					{	
						h = refu_v[(u+1)*3+(v+1)];
						//res += h*(image.data[(i+u)*colonnes+(j+v)]);
						res0 += h * image.data[(i+u)*colonnes*3+(j+v)*3+0];
						res1 += h * image.data[(i+u)*colonnes*3+(j+v)*3+1];
						res2 += h * image.data[(i+u)*colonnes*3+(j+v)*3+2];
					}
				} 
				//copie.data[i*colonnes+j] = (unsigned char)(res);
				copie.data[i*colonnes*3+j*3+0] = (unsigned char) res0;
				copie.data[i*colonnes*3+j*3+1] = (unsigned char) res1;
				copie.data[i*colonnes*3+j*3+2] = (unsigned char) res2;
		}
	}
	return copie;	
}

//moyenne -> on a un carré donc tant pis pour le sinon !

/**
 * Calcul d'un lissage spatial
  noyau = fonction exponentielle*/
Mat spatialSmoothingExp(Mat image, double gamma)
{
	Mat copie = image;
	int lignes = image.rows;
	int colonnes = image.cols;
	double res;
	double h;
	double coeff = pow(gamma, 2)/4;
	
	std::vector<double> refu_v;
	
	for(int i = 0; i<3; i++)
	{
		for(int j = 0; j < 3; j++)
		{
			refu_v.push_back(coeff * exp(-gamma *(abs(i-1)+abs(j-1))));
		}
	}
	
	for(int i = 1; i<lignes-1; i++)
	{
		for(int j = 1; j < colonnes-1; j++)
		{
				res = 0;
				for (int u = -1; u < 2; u++)
				{
					for (int v = -1; v < 2; v++)
					{	
						h = refu_v[(u+1)*3+(v+1)];
						res += h*(image.data[(i+u)*colonnes+(j+v)]);
					}
				} 
				copie.data[i*colonnes+j] = (unsigned char)(res);
		}
	}
	return copie;	
}

Mat spatialSmoothingExpColor(Mat image, double gamma)
{
	Mat copie = image;
	int lignes = image.rows;
	int colonnes = image.cols;
	double res0, res1, res2;
	double h;
	double coeff = pow(gamma, 2)/4;
	
	std::vector<double> refu_v;
	
	for(int i = 0; i<3; i++)
	{
		for(int j = 0; j < 3; j++)
		{
			refu_v.push_back(coeff * exp(-gamma *(abs(i-1)+abs(j-1))));
		}
	}
	
	for(int i = 1; i<lignes-1; i++)
	{
		for(int j = 1; j < colonnes-1; j++)
		{
				res0 = res1 = res2 = 0;
				for (int u = -1; u < 2; u++)
				{
					for (int v = -1; v < 2; v++)
					{	
						h = refu_v[(u+1)*3+(v+1)];
						//res += h*(image.data[(i+u)*colonnes+(j+v)]);
						res0 += h * image.data[(i+u)*colonnes*3+(j+v)*3+0];
						res1 += h * image.data[(i+u)*colonnes*3+(j+v)*3+1];
						res2 += h * image.data[(i+u)*colonnes*3+(j+v)*3+2];
					}
				} 
				//copie.data[i*colonnes+j] = (unsigned char)(res);
				copie.data[i*colonnes*3+j*3+0] = (unsigned char) res0;
				copie.data[i*colonnes*3+j*3+1] = (unsigned char) res1;
				copie.data[i*colonnes*3+j*3+2] = (unsigned char) res2;
		}
	}
	return copie;	
}


/**
 * Retourne vrai si le pixel est situé à une certaine distance des bords de l'écran
 */
bool isScreenEge(int coordX, int coordY, int maxX, int maxY, double distance){

	if(distance < 0){
		distance = 0;
	}

	/* Le point est entre 0 et la distance selon X*/
	if(coordX <= distance ||
			/* Le point est entre 0 et la distance selon Y*/
			coordY <= distance ||
			/* Le point est situé à proximité de la valeur max pour X*/
			coordX >= (maxX - distance) ||
			/* Le point est situé à proximité de la valeur max de Y*/
			coordY >= (maxY - distance)){
		
		return true;
	}
	return false;
}

/**
 * Renvoi vrai si la différence entre les couleurs des deux pixels est supérieure au seuil
 */
bool colorPixelDifference(Vec3b pixA, Vec3b pixB, double seuil){

	if(abs(pixA[0] - pixB[0]) > seuil||
			abs(pixA[1] - pixB[1]) > seuil ||
			abs(pixA[2] - pixB[2]) >seuil){
		return true;
	}
	return false;

}
/**
 * Extraction des éléments mouvants d'une image
 * @background : image de comparaison
 * @image : image actuelle, dont les différences sont extraites
 * @seuil : valeur étalon pour déterminer une différence
 * @return : une image composé des pixels différents entre image et background
 */
Mat thresholdExtraction(Mat background, Mat image, double seuil){

	/* Création de l'image à retourner*/
	Mat retour(image.size(), image.type());

	/* Boucle d'extraction des différences */
	for(int x = 0; x < image.rows; x++){

		for(int y = 0; y < image.cols; y++){

			if(colorPixelDifference(
						background.at<Vec3b>(x, y),
						image.at<Vec3b>(x, y),
						seuil)){
				retour.at<Vec3b>(x, y) = image.at<Vec3b>(x, y);
			}else{
				retour.at<Vec3b>(x, y) = Vec3b(0, 0, 0);
			}
		}
	}

	return retour;


}

/**
 * Extrait les éléments mouvants de @frame
 * en comparant par rapport à  l'image de fond passée dans l'argument @Background
 * @backgound Matrice contenant l'image de fond
 * @frame Matrice contenant l'image complete
 * @return les éléments mouvants extraits de frame
 */
Mat extractForegroundColor(Mat background, Mat frame)
{
    Mat res;
    int seuil = 16;
	
    /*Vérification de la correspondance des dimensions*/
	if (background.rows == frame.rows && background.cols == frame.cols)
	{
        res = Mat(frame.size(), frame.type());

        /*comparaison de l'image avec l'arrière-plan
        et construction d'une image où les différences apparaissent*/
        for (int i = 0; i<frame.rows; i++)
        {
            for (int j = 0; j<frame.cols; j++)
            {

                if(abs(frame.at<Vec3b>(i,j)[0] - background.at<Vec3b>(i,j)[0]) > seuil
                || abs(frame.at<Vec3b>(i,j)[1] - background.at<Vec3b>(i,j)[1]) > seuil
                || abs(frame.at<Vec3b>(i,j)[2] - background.at<Vec3b>(i,j)[2]) > seuil){

                    //res.at<Vec3b>(i,j) = Vec3b(0,0,255);
                    res.at<Vec3b>(i,j) = frame.at<Vec3b>(i,j);

                }
                else{

                    res.at<Vec3b>(i,j) = Vec3b(0,100,0);
                }
            }
        }
	}
	else{
		wrongFormat();
	}
    return res;
}

Mat extractForeground(Mat background, Mat frame)
{
    Mat res;
    int seuil = 25;
	
    /*Vérification de la correspondance des dimensions*/
	if (background.rows == frame.rows && background.cols == frame.cols)
	{
        res = Mat(frame.size(), frame.type());

        /*comparaison de l'image avec l'arrière-plan
        et construction d'une image où les différences apparaissent*/
        for (int i = 0; i<frame.rows; i++)
        {
            for (int j = 0; j<frame.cols; j++)
            {

                if(abs(frame.at<unsigned char>(i,j) - background.at<unsigned char>(i,j)) > seuil){

                    //res.at<Vec3b>(i,j) = Vec3b(0,0,255);
                    res.at<unsigned char>(i,j) = frame.at<unsigned char>(i,j);

                }
                else{

                    res.at<unsigned char>(i,j) = 100;
                }
            }
        }
	}
	else{
		wrongFormat();
	}
    return res;
}

/**
 * Lissage temporel
 */
Mat temporalSmoothing(String filename){

    /*Résultat à renvoyer*/
    Mat background;
    /*Ensemble des 10 premières frames de la vidéos*/
    Mat stockage[10];
    /** Video*/
    VideoCapture vc = VideoCapture(filename);
    double h = 0.1;

    /*Récupération des dix premières images*/
    for(int i = 0; i< 10; i++){
        vc >> stockage[i];
    }
    background = Mat(stockage[0].size(), stockage[0].type());

    /*
     * Création d'une image dont chaque pixel correspond à la moyenne des 10 pixels
     * respectifs de chaque image
     */
    for(int x = 0; x < background.rows; x++){
        for(int y = 0; y < background.cols; y ++){
            for(int k = 0; k < 3; k++)
            {
                //calcul de la convolution pour chaque composante couleur
                int res = 0;
                for (int u = 0; u < 10; u++)
                {

                    res = res + (h*(stockage[u].at<Vec3b>(x, y)[k]));
                }
                background.at<Vec3b>(x,y)[k] = res;
            }

        }
    }
    vc.release();
    spatialSmoothingAvgColor(background, 1);
    return background;
}

void splitAndMerge(Mat & image)
{
	std::list<Bloc*> blocs;
	int lignes = image.rows;
	int colonnes = image.cols;
	int xmin, xmax, ymin, ymax;
	ymin = image.rows+1;
	ymax = -1;
	xmin = image.cols+1;
	xmax = -1;
	
	//extraction des pixels verts
	for(int i = 0; i<lignes; i++)
	{
		for(int j = 0; j < colonnes; j++)
		{
			if (!(image.data[i*colonnes*3+j*3+0] == 0 && image.data[i*colonnes*3+j*3+1] == 100 && image.data[i*colonnes*3+j*3+2] == 0))
			{
				if (j>xmax)
					xmax = j;
				if (j<xmin)
					xmin = j;
					
				if (i>ymax)
					ymax = i;
				if (i<ymin)
					ymin = i;
			}
		}
	}
	
	std::cout<<"xmin : "<<xmin<<" xmax : "<<xmax<<std::endl;
	std::cout<<"ymin : "<<ymin<<" ymax : "<<ymax<<std::endl;

	blocs.push_back(new Bloc(pixel (xmin, ymin), pixel (xmax, ymax)));
	
	blocs.front()->split(blocs, image);
	
	//affichage du résultat du split
	for(auto it = blocs.begin(); it != blocs.end(); it++)
	{
		rectangle(image, Point((*it)->p_hg.x,(*it)->p_hg.y), Point ((*it)->p_bd.x,(*it)->p_bd.y), 1);
	}
	
//	for(int i = ymin; i<ymax; i++)
//	{
//		for(int j = xmin; j < xmax; j++)
//		{
//			image.data[i*colonnes*3+j*3+1] = 255;
//		}
//	}
	
}
