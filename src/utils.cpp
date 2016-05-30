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
 * Calcul d'un lissage spatial pour une image en niveau de gris
  noyau = moyenne*/
Mat spatialSmoothingAvg(const Mat & image, double lambda)
{
	Mat copie = image.clone();
	double h = 1.0/(pow(lambda,2));
	int lignes = image.rows;
	int colonnes = image.cols;
	int res0, res1, res2; //des int pour eviter le dépassement de capacité
	int lambdasur2 = lambda/2;
	int coordMin = -lambdasur2;
	int coordMax = lambdasur2;
	
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
					res0 += image.data[(i+u)*colonnes+(j+v)];
				}
			} 
			copie.data[i*colonnes+j] = (unsigned char)(res0 * h); //on repasse en char
		}
	}
	return copie;
}

/**
 * Calcul d'un lissage spatial pour une image en couleurs
  noyau = moyenne*/
Mat spatialSmoothingAvgColor(const Mat & image, double lambda)
{
	Mat copie = image.clone();
	double h = 1.0/(pow(lambda,2));
	int lignes = image.rows;
	int colonnes = image.cols;
	int res0, res1, res2;
	int lambdasur2 = lambda/2;
	int coordMin = -lambdasur2;
	int coordMax = lambdasur2;
	
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
 * Calcul d'un lissage spatial pour une image en niveau de gris
  noyau = Gaussienne*/
Mat spatialSmoothingGauss(const Mat & image, double sigma)
{
	Mat copie = image.clone();
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

/**
 * Calcul d'un lissage spatial pour une image en couleur
  noyau = Gaussienne*/
Mat spatialSmoothingGaussColor(const Mat & image, double sigma)
{
	Mat copie = image.clone();
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

/**
 * Calcul d'un lissage spatial pour une image en niveau de gris
  noyau = Exponentielle*/
Mat spatialSmoothingExp(const Mat & image, double gamma)
{
	Mat copie = image.clone();
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

/**
 * Calcul d'un lissage spatial pour une image en niveau de gris
  noyau = Exponentielle*/
Mat spatialSmoothingExpColor(const Mat & image, double gamma)
{
	Mat copie = image.clone();
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
						res0 += h * image.data[(i+u)*colonnes*3+(j+v)*3+0];
						res1 += h * image.data[(i+u)*colonnes*3+(j+v)*3+1];
						res2 += h * image.data[(i+u)*colonnes*3+(j+v)*3+2];
					}
				} 
				
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
 * Extrait les éléments mouvants de @frame
 * en comparant par rapport à  l'image de fond passée dans l'argument @Background
 * @backgound Matrice contenant l'image de fond
 * @frame Matrice contenant l'image complete
 * @return les éléments mouvants extraits de frame
 */
Mat extractForegroundColor(const Mat & background, const Mat & frame){
    Mat res;
    int seuil = 14;
    int lignes = frame.rows;
    int colonnes = frame.cols;
	
    /*Vérification de la correspondance des dimensions*/
	if (background.rows == lignes && background.cols == colonnes){
        
        res = Mat(frame.size(), frame.type());

        /*comparaison de l'image avec l'arrière-plan
        et construction d'une image où les différences apparaissent*/
        for (int i = 0; i<lignes; i++){
            for (int j = 0; j<colonnes; j++){
            
            	if(abs(frame.data[i*colonnes*3+j*3+0] - background.data[i*colonnes*3+j*3+0]) > seuil
            	|| abs(frame.data[i*colonnes*3+j*3+1] - background.data[i*colonnes*3+j*3+1]) > seuil
            	|| abs(frame.data[i*colonnes*3+j*3+2] - background.data[i*colonnes*3+j*3+2]) > seuil){

                    res.data[i*colonnes*3+j*3+0] = frame.data[i*colonnes*3+j*3+0];
                    res.data[i*colonnes*3+j*3+1] = frame.data[i*colonnes*3+j*3+1];
                    res.data[i*colonnes*3+j*3+2] = frame.data[i*colonnes*3+j*3+2];
				}
                else{
                	res.data[i*colonnes*3+j*3+0] = BLUE;
                    res.data[i*colonnes*3+j*3+1] = GREEN;
                    res.data[i*colonnes*3+j*3+2] = RED;
                    //opencv fonctionne en BGR et non en RGB
                }
            }
        }
	}
	else{
		wrongFormat();
	}
    return res;
}

Mat extractForeground(const Mat & background, const Mat & frame)
{
    Mat res;
    int seuil = 25;
    int lignes = frame.rows;
    int colonnes = frame.cols;
	
    /*Vérification de la correspondance des dimensions*/
	if (background.rows == lignes && background.cols == colonnes)
	{
        res = Mat(frame.size(), frame.type());

        /*comparaison de l'image avec l'arrière-plan
        et construction d'une image où les différences apparaissent*/
        for (int i = 0; i<lignes; i++)
        {
            for (int j = 0; j<colonnes; j++)
            {

                if(abs(frame.data[i*colonnes+j] - background.data[i*colonnes+j]) > seuil){

                    res.data[i*colonnes+j] = frame.data[i*colonnes+j];

                }
                else{	
                    res.data[i*colonnes+j] = GRAY;
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
 * Lissage temporel pour image couleur
 */
Mat temporalSmoothingColor(String filename){

    /*Résultat à renvoyer*/
    Mat background;
    /*Ensemble des 10 premières frames de la vidéos*/
    Mat stockage[10];
    /** Video*/
    VideoCapture vc = VideoCapture(filename);

    /*Récupération des dix premières images*/
    for(int i = 0; i< 10; i++){
        vc >> stockage[i];
    }
    background = Mat(stockage[0].size(), stockage[0].type());
	
	double h = 0.1;
    double res0, res1, res2;
    int lignes = background.rows;
    int colonnes = background.cols;
	
    /*
     * Création d'une image dont chaque pixel correspond à la moyenne des 10 pixels
     * respectifs de chaque image
     */
    for(int x = 0; x < lignes; x++){
        for(int y = 0; y < colonnes; y ++){

            res0 = res1 = res2 = 0;
            for (int u = 0; u < 10; u++)
            {
				//res = res + (h*(stockage[u].at<Vec3b>(x, y)[k]));
				res0 += stockage[u].data[x*colonnes*3+y*3+0];
				res1 += stockage[u].data[x*colonnes*3+y*3+1];
				res2 += stockage[u].data[x*colonnes*3+y*3+2];
            }
            background.data[x*colonnes*3+y*3+0] = (unsigned char)(res0 * h);
            background.data[x*colonnes*3+y*3+1] = (unsigned char)(res1 * h);
            background.data[x*colonnes*3+y*3+2] = (unsigned char)(res2 * h);

        }
    }
    vc.release();
    //spatialSmoothingAvgColor(background, 1);
    return background;
}

/**
 * Lissage temporel pour image en niveau de gris
 */
Mat temporalSmoothing(String filename){

    /*Résultat à renvoyer*/
    Mat background;
    /*Ensemble des 10 premières frames de la vidéos*/
    Mat stockage[10];
    
    Mat temp;
    /** Video*/
    VideoCapture vc = VideoCapture(filename);

    /*Récupération des dix premières images*/
    for(int i = 0; i< 10; i++){
        vc >> temp;
        cvtColor(temp, stockage[i], CV_BGR2GRAY); //passage de couleur en niveau de gris
    }
    background = Mat(stockage[0].size(), stockage[0].type());
	
	double h = 0.1;
    double res;
    int lignes = background.rows;
    int colonnes = background.cols;
	
    /*
     * Création d'une image dont chaque pixel correspond à la moyenne des 10 pixels
     * respectifs de chaque image
     */
    for(int x = 0; x < lignes; x++){
        for(int y = 0; y < colonnes; y ++){

            res = 0;
            for (int u = 0; u < 10; u++)
            {
				res += stockage[u].data[x*colonnes+y];
            }
            background.data[x*colonnes+y] = (unsigned char)(res * h);

        }
    }
    vc.release();
    //spatialSmoothingAvgColor(background, 1);
    return background;
}

//http://stackoverflow.com/questions/23468537/differences-of-using-const-cvmat-cvmat-cvmat-or-const-cvmat
void splitAndMerge(const Mat & image)
{
	Mat copieCarree = Mat(image.rows, image.cols, CV_64FC3,Scalar::all(0));
	Mat copie = image.clone();
	steady_clock::time_point start, end;
	start = steady_clock::now();
	std::list<Bloc*> blocs;
	int lignes = copie.rows;
	int colonnes = copie.cols;
	int xmin, xmax, ymin, ymax;
	ymin = lignes+1;
	ymax = -1;
	xmin = colonnes+1;
	xmax = -1;
	
	//extraction des pixels verts
	for(int i = 0; i<lignes; i++)
	{
		for(int j = 0; j < colonnes; j++)
		{
			if (!(copie.data[i*colonnes*3+j*3+0] == BLUE && copie.data[i*colonnes*3+j*3+1] == GREEN && copie.data[i*colonnes*3+j*3+2] == RED))
			{
				if (j>xmax)
					xmax = j;
				if (j<xmin)
					xmin = j;
					
				if (i>ymax)
					ymax = i;
				if (i<ymin)
					ymin = i;
				
				//on fait la modification préalable
				copieCarree.data[i*colonnes*3+j*3+0] = (double)pow(copie.data[i*colonnes*3+j*3+0],2);
				std::cout<<"Carré  = "<<copieCarree.data[i*colonnes*3+j*3+0]<<std::endl;
				std::cout<<"Carré_pow  = "<<pow(copie.data[i*colonnes*3+j*3+0],2);
				copieCarree.data[i*colonnes*3+j*3+1] = (double)pow(copie.data[i*colonnes*3+j*3+1],2);
				copieCarree.data[i*colonnes*3+j*3+2] = (double) pow(copie.data[i*colonnes*3+j*3+2],2);
			}
		}
	}
	
	std::cout<<"xmin : "<<xmin<<" xmax : "<<xmax<<std::endl;
	std::cout<<"ymin : "<<ymin<<" ymax : "<<ymax<<std::endl;

	blocs.push_back(new Bloc(pixel (xmin, ymin), pixel (xmax, ymax)));
	
	 cout << "M = " << endl << " " << copieCarree<< endl << endl;
	
	blocs.front()->split(blocs, copie, copieCarree);
	
	//affichage du résultat du split (destiné à disparaitre)
	
	auto itBloc = blocs.begin();
	itBloc++; itBloc++;itBloc++;itBloc++;
	rectangle(image, Point((*itBloc)->p_hg.x,(*itBloc)->p_hg.y), Point ((*itBloc)->p_bd.x,(*itBloc)->p_bd.y), CV_RGB(255,0,0),CV_FILLED);
	for(auto it = (*itBloc)->getVoisins().begin(); it != (*itBloc)->getVoisins().end(); it++)
	{
		rectangle(image, Point((*it)->p_hg.x,(*it)->p_hg.y), Point ((*it)->p_bd.x,(*it)->p_bd.y), CV_RGB(0,0,255),CV_FILLED);
	}
	
	for(auto it = blocs.begin(); it != blocs.end(); it++)
	{
		rectangle(image, Point((*it)->p_hg.x,(*it)->p_hg.y), Point ((*it)->p_bd.x,(*it)->p_bd.y), 1);
	}
	
	end = steady_clock::now();
	 std::cout<<"time : "<< duration_cast<milliseconds>(end-start).count()<<std::endl;
	
//	for(int i = ymin; i<ymax; i++)
//	{
//		for(int j = xmin; j < xmax; j++)
//		{
//			image.data[i*colonnes*3+j*3+1] = 255;
//		}
//	}
	
}

/**
 * Lissage à utiliser uniquement sur une image traitée avec foregroundextraction
 */
Mat lissageCouleur(const Mat & image, int nbrVoisin, int requis){
	
	Mat retour(image.size(), image.type());
	int count;
	int iMax, jMax, iMin, jMin;

	for(int x = 0; x < image.rows - 1; x++){
		for(int y = 0; y < image.cols - 1; y++){
			if(retour.at<Vec3b>(x,y) != Vec3b(255, 255, 255)){

				count = 0;
				iMax = x + nbrVoisin;
				iMin = x - nbrVoisin;
				jMax = y + nbrVoisin;
				jMin = y - nbrVoisin;

				if(x -  nbrVoisin < 0)
					iMin = nbrVoisin - x - nbrVoisin;
				if(x + nbrVoisin > image.rows - 1)
					iMax = image.rows -1 - x;
				if(y - nbrVoisin < 0)
					jMin = nbrVoisin - y - nbrVoisin;
				if(y + nbrVoisin > image.cols - 1)
					jMax = image.cols - 1 - y;

				for(int i = iMin; i < iMax; i++){
					for(int j = jMin; j < jMax; j++){
						if(retour.at<Vec3b>(x + i,y + j) != Vec3b(255, 255, 255))
							count++;
						if(count > requis)
							break;
					}
					if(count > requis)
						break;
				}
				count--;
				if(count < requis){
					retour.at<Vec3b>(x,y) = Vec3b(255, 255, 255);
				}
			}
		}
	}
	return retour;

}
