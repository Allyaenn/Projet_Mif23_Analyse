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

/**
 * Lissage à utiliser uniquement sur une image traitée avec foregroundextraction
 */
Mat preciseSmoothing(Mat image, int nbrVoisin, int requis, Mat orig){
	
	Mat retour(image.size(), image.type());
	int count, total;
	int colonnes = image.cols;
	int lignes = image.rows;
	int iMax, jMax, iMin, jMin;
	//Vec3b fond = Vec3b(BLUE, GREEN, RED);

	for(int x = 0; x < lignes; x++){
		for(int y = 0; y < colonnes; y++){
			retour.data[x*colonnes*3+y*3+0] = image.data[x*colonnes*3+y*3+0];
			retour.data[x*colonnes*3+y*3+1] = image.data[x*colonnes*3+y*3+1];
			retour.data[x*colonnes*3+y*3+2] = image.data[x*colonnes*3+y*3+2];
			
			if (!(image.data[x*colonnes*3+y*3+0] == BLUE 
		       && image.data[x*colonnes*3+y*3+1] == GREEN 
		       && image.data[x*colonnes*3+y*3+2] == RED))
			{
				count = 0;
				total = 0;
				iMax = x + nbrVoisin;
				iMin = x - nbrVoisin;
				jMax = y + nbrVoisin;
				jMin = y - nbrVoisin;

				if(iMin < 0)
					iMin = nbrVoisin - x - nbrVoisin;
				if(iMax >= lignes)
					iMax = lignes - x;
				if(jMin < 0)
					jMin = nbrVoisin - y - nbrVoisin;
				if(jMax >= colonnes)
					jMax = colonnes - y;

				for(int i = iMin; i < iMax; i++){
					for(int j = jMin; j < jMax; j++){
						if (!(image.data[i*colonnes*3+j*3+0] == BLUE 
		                   && image.data[i*colonnes*3+j*3+1] == GREEN 
		                   && image.data[i*colonnes*3+j*3+2] == RED))
							count++;
						if(count > requis)
							break;
					}
					if(count > requis)
						break;
				}
				count--;
				if(count < requis){
					retour.data[x*colonnes*3+y*3+0] = BLUE;
					retour.data[x*colonnes*3+y*3+1] = GREEN;
					retour.data[x*colonnes*3+y*3+2] = RED;
				}
			}
		}
	}
	return retour;
}

//http://stackoverflow.com/questions/23468537/differences-of-using-const-cvmat-cvmat-cvmat-or-const-cvmat
std::list<Bloc*> split(const Mat & image, unsigned short int tabCarres [], double seuil )
{
	steady_clock::time_point start, end;
	start = steady_clock::now();
	
	int lignes = image.rows;
	int colonnes = image.cols;
	Mat copie = image.clone();
	std::list<Bloc*> blocsATraiter;
	std::list<Bloc*> blocsDefinitifs;
	Bloc* temp;
	
	int xmin, xmax, ymin, ymax;
	ymin = lignes+1;
	ymax = -1;
	xmin = colonnes+1;
	xmax = -1;
	
	// SPLIT
	//extraction des pixels verts
	for(int i = 0; i<lignes; i++)
	{
		for(int j = 0; j < colonnes; j++)
		{
			if (!(copie.data[i*colonnes*3+j*3+0] == BLUE 
			   && copie.data[i*colonnes*3+j*3+1] == GREEN 
			   && copie.data[i*colonnes*3+j*3+2] == RED))
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
				tabCarres[i*colonnes*3+j*3+0] = (double)copie.data[i*colonnes*3+j*3+0] * (double)copie.data[i*colonnes*3+j*3+0];
				tabCarres[i*colonnes*3+j*3+1] = (double)copie.data[i*colonnes*3+j*3+1] * (double)copie.data[i*colonnes*3+j*3+1];
				tabCarres[i*colonnes*3+j*3+2] = (double)copie.data[i*colonnes*3+j*3+2] * (double)copie.data[i*colonnes*3+j*3+2];
				
			}
		}
	}
	
//	std::cout<<"xmin : "<<xmin<<" xmax : "<<xmax<<std::endl;

	blocsATraiter.push_back(new Bloc(pixel (xmin, ymin), pixel (xmax, ymax)));
	
	while (!blocsATraiter.empty())
  	{
  		temp = blocsATraiter.front();
		
		if (temp->hasToBeSplitted(image, tabCarres, seuil))
		{
			//séparation du bloc en 4
			int nvX, nvY;
			nvX = (temp->p_bd.x - temp->p_hg.x)/2;
			nvY = (temp->p_bd.y - temp->p_hg.y)/2;
			// std::cout<<std::endl;
	
			Bloc* bloc1  = new Bloc(pixel(temp->p_hg.x, temp->p_hg.y), pixel(nvX+temp->p_hg.x, nvY+temp->p_hg.y)); //OK
	
			Bloc* bloc2  = new Bloc(pixel(nvX+temp->p_hg.x+1, temp->p_hg.y), pixel(temp->p_bd.x, nvY+temp->p_hg.y));
	
			Bloc* bloc3  = new Bloc(pixel(temp->p_hg.x, nvY+temp->p_hg.y+1), pixel(nvX+temp->p_hg.x, temp->p_bd.y));
	
			Bloc* bloc4  = new Bloc(pixel(nvX+temp->p_hg.x+1, nvY+temp->p_hg.y+1), pixel(temp->p_bd.x, temp->p_bd.y)); //OK
	
			//répartion des vosins du bloc d'origine
	
			for (auto itv = temp->voisins.begin(); itv != temp->voisins.end(); itv++)
			{
				//supression du bloc principal en tant que voisin chez ses propres  voisins
				for (auto it2 = (*itv)->voisins.begin(); it2 != (*itv)->voisins.end(); it2++)
				{
					if (*temp == **it2)
					{
						(*itv)->voisins.erase(it2);
						break;
					}
				}	
			
				if (bloc1->estVoisin(**itv)){
					//std::cout<<"Le bloc 1 a un voisin"<<std::endl;
					bloc1->voisins.push_back(*itv);
					(*itv)->voisins.push_back(bloc1);
				}
			
				if (bloc2->estVoisin(**itv)){
					//std::cout<<"Le bloc 2 a un voisin"<<std::endl;
					bloc2->voisins.push_back(*itv);
					(*itv)->voisins.push_back(bloc2);
				}
			
				if (bloc3->estVoisin(**itv)){
					//std::cout<<"Le bloc 3 a un voisin"<<std::endl;
					bloc3->voisins.push_back(*itv);
					(*itv)->voisins.push_back(bloc3);
				}
			
				if (bloc4->estVoisin(**itv)){
					//std::cout<<"Le bloc 4 a un voisin"<<std::endl;
					bloc4->voisins.push_back(*itv);
					(*itv)->voisins.push_back(bloc4);
				}
			}
		
			//insertion des nouveaux blocs dans les listes de voisins
			bloc1->voisins.push_back(bloc2);
			bloc1->voisins.push_back(bloc3);
			bloc1->voisins.push_back(bloc4);
		
			bloc2->voisins.push_back(bloc1);
			bloc2->voisins.push_back(bloc3);
			bloc2->voisins.push_back(bloc4);
		
			bloc3->voisins.push_back(bloc1);
			bloc3->voisins.push_back(bloc2);
			bloc3->voisins.push_back(bloc4);
		
			bloc4->voisins.push_back(bloc1);
			bloc4->voisins.push_back(bloc2);
			bloc4->voisins.push_back(bloc3);
			
			//suppression de l'ancien bloc
			blocsATraiter.pop_front();
		
			//insertion des nouveaux blocs dans la listes
			blocsATraiter.push_back(bloc1);
			blocsATraiter.push_back(bloc2);
			blocsATraiter.push_back(bloc3);
			blocsATraiter.push_back(bloc4);

		}
		else
		{
			blocsATraiter.pop_front();
			blocsDefinitifs.push_back(temp);
		}
	}
	
	//affichage du résultat du split (destiné à disparaitre)
	
//	auto itBloc = blocsDefinitifs.begin();
//	for (int i = 0; i<57; i++)
//		itBloc++;
//	rectangle(image, Point((*itBloc)->p_hg.x,(*itBloc)->p_hg.y), Point ((*itBloc)->p_bd.x,(*itBloc)->p_bd.y), CV_RGB(255,0,0),CV_FILLED);
//	for(auto it = (*itBloc)->getVoisins().begin(); it != (*itBloc)->getVoisins().end(); it++)
//	{
//		rectangle(image, Point((*it)->p_hg.x,(*it)->p_hg.y), Point ((*it)->p_bd.x,(*it)->p_bd.y), CV_RGB(0,0,255),CV_FILLED);
//	}
//	
//	for(auto it = blocsDefinitifs.begin(); it != blocsDefinitifs.end(); it++)
//	{
//		rectangle(image, Point((*it)->p_hg.x,(*it)->p_hg.y), Point ((*it)->p_bd.x,(*it)->p_bd.y), 1);
//	}

	end = steady_clock::now();
	std::cout<<"time split : "<< duration_cast<milliseconds>(end-start).count()<<std::endl;
	return blocsDefinitifs;
}

std::list<Region*> merge(const std::list<Bloc*> blocs, const Mat & image, const unsigned short int tabCarres [], double seuil)
{
	std::list<Bloc*> blocsLibres = blocs;
	steady_clock::time_point start, end;
	start = steady_clock::now();
	int lignes = image.rows;
	int colonnes = image.cols;
	std::list<Region*> regionsDef;
	std::list<Bloc*> voisins;
	Region* regTemp;
	Bloc* blocTemp;
	Bloc* blocVois;
	
	//blocs contient les blocs "libres" (qui n'appartiennent pas déjà à une région)
	while (!blocsLibres.empty())
	{
		//création d'une nouvelle région
		blocTemp = blocsLibres.front();
		blocsLibres.pop_front();
		regTemp = new Region();
		regTemp->addBloc(blocTemp);
		regTemp->updateVar(image, tabCarres);
		voisins.insert(voisins.end(), blocTemp->voisins.begin(), blocTemp->voisins.end());
		//parcours des voisins de la région (c'est à dire les voisins libres des blocs constituant la région)

		while(!voisins.empty())
		{
			//depop du premier voisin
			blocVois = voisins.front();
			voisins.pop_front();
			std::list<Bloc*>::iterator it;
  			it = find (blocsLibres.begin(), blocsLibres.end(), blocVois);
  			if (it != blocsLibres.end()){ //si il est dans les blocs libres
  			  	//std::cout<<"L'un de mes voisins est libre"<<std::endl;
				// si valeur ok -> on fusionnne	(ajout du bloc dans la région + ajout des voisins dans les voisins)
				if (regTemp->isConsistent(*blocVois, seuil)){
					//fusion
					regTemp->addBloc(blocVois);
					regTemp->updateVar(image, tabCarres);
					//ajout des voisins dans les voisins
					voisins.insert(voisins.end(), blocVois->voisins.begin(), blocVois->voisins.end());
					//on le retire des blocs libres
					blocsLibres.erase(it);
				}
				//else le voisin ne nous interesse pas
				// sinon rien
  			}
  			//else le voisin n'est pas libre, on ne le considère pas
		} // il n'y a plus de voisins à explorer
		
		//ajout de la région dans la liste des régions definitives
		voisins.clear();
		regionsDef.push_back(regTemp);
	}
	
	double * tabMoys;
	int i = 0;
	for (auto itReg = regionsDef.begin(); itReg != regionsDef.end(); itReg++)
	{
		tabMoys = (*itReg)->getMoy();
		for (auto itBloc = (*itReg)->getBlocs().begin(); itBloc != (*itReg)->getBlocs().end(); itBloc++)
		{
			//colorer chaque pixel avec la couleur moyenne de chaque région
			for (int i = (*itBloc)->p_hg.y; i < (*itBloc)->p_bd.y; i++)
			{
				for (int j = (*itBloc)->p_hg.x; j < (*itBloc)->p_bd.x; j++)
				{
					if (!(image.data[i*colonnes*3+j*3+0] == BLUE 
				       && image.data[i*colonnes*3+j*3+1] == GREEN 
				       && image.data[i*colonnes*3+j*3+2] == RED))
					{
						image.data[i*colonnes*3+j*3+0] = tabMoys[0];
						image.data[i*colonnes*3+j*3+1] = tabMoys[1];
						image.data[i*colonnes*3+j*3+2] = tabMoys[2];
					}
				}
			}
		}
	}
	
	end = steady_clock::now();
	std::cout<<"time merge : "<< duration_cast<milliseconds>(end-start).count()<<std::endl;
	return regionsDef;
}
