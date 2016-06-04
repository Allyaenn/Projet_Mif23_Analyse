#include "utils.h"

/**
 * Usage of the program
 */
void usage(){
	cout << "Error : expected usage" << endl << "./Analyse <video>" << endl;
}

/**
 * Wrong size of a picture
 */void wrongFormat(){
	std::cout<<"Error  : wrong size for used pictures, they have to be of equal sizes"<<std::endl;
}

/**
* Calcul of a spatial smoothing on a greyscale picture
* kernel = average
*/
Mat spatialSmoothingAvg(const Mat & image, double lambda)
{
	Mat copie = image.clone();
	double h = 1.0/(pow(lambda,2));
	int lignes = image.rows;
	int colonnes = image.cols;
	int pas = image.step;
	/*Integers are used to prevent overflow*/
	int res0, res1, res2;
	int lambdasur2 = lambda/2;
	int coordMin = -lambdasur2;
	int coordMax = lambdasur2;
	
	for(int i = lambdasur2; i<lignes-lambdasur2; i++)
	{
		for(int j = lambdasur2; j<colonnes-lambdasur2; j++)
		{
			/*Calculation of the convolution for each color element*/
			res0 = res1 = res2 = 0;
			for (int u = coordMin; u <= coordMax; u++)
			{
				for (int v = coordMin; v <= coordMax; v++)
				{	
					res0 += image.data[(i+u)*pas+(j+v)];
				}
			}
			/*Data is switched back to characters*/
			copie.data[i*pas+j] = (unsigned char)(res0 * h);
		}
	}
	return copie;
}

/**
* Calcul of a spatial smoothing in a colored (3 canals) picture
* kernel = average
*/
Mat spatialSmoothingAvgColor(const Mat & image, double lambda)
{
	Mat copie = image.clone();
	double h = 1.0/(pow(lambda,2));
	int lignes = image.rows;
	int colonnes = image.cols;
	int pas = image.step;
	int res0, res1, res2;
	int lambdasur2 = lambda/2;
	int coordMin = -lambdasur2;
	int coordMax = lambdasur2;
	
	for(int i = lambdasur2; i<lignes-lambdasur2; i++)
	{
		for(int j = lambdasur2; j<colonnes-lambdasur2; j++)
		{
			/*Calculation of the convolution for each color element*/
			res0 = res1 = res2 = 0;
			for (int u = coordMin; u <= coordMax; u++)
			{
				for (int v = coordMin; v <= coordMax; v++)
				{
					res0 += image.data[(i+u)*pas+(j+v)*3+0];
					res1 += image.data[(i+u)*pas+(j+v)*3+1];
					res2 += image.data[(i+u)*pas+(j+v)*3+2];
				}
			} 
			copie.data[i*pas+j*3+0] = res0 * h;
			copie.data[i*pas+j*3+1] = res1 * h;
			copie.data[i*pas+j*3+2] = res2 * h;
		}
	}
	return copie;
}


/**
* Calcul of a spatial smoothing on a greyscale picture
* kernel = Gaussian
*/
Mat spatialSmoothingGauss(const Mat & image, double sigma)
{
	Mat copie = image.clone();
	int lignes = image.rows;
	int colonnes = image.cols;
	int pas = image.step;
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
						res += h*(image.data[(i+u)*pas+(j+v)]);
					}
				} 
				copie.data[i*pas+j] = (unsigned char)(res);
		}
	}
	return copie;	
}

/**
* Calcul of a spatial smoothing in a colored picture (3 canals)
* kernel = Gaussian
*/
Mat spatialSmoothingGaussColor(const Mat & image, double sigma)
{
	Mat copie = image.clone();
	int lignes = image.rows;
	int colonnes = image.cols;
	int pas = image.step;
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
						res0 += h * image.data[(i+u)*pas+(j+v)*3+0];
						res1 += h * image.data[(i+u)*pas+(j+v)*3+1];
						res2 += h * image.data[(i+u)*pas+(j+v)*3+2];
					}
				} 
				//copie.data[i*colonnes+j] = (unsigned char)(res);
				copie.data[i*pas+j*3+0] = (unsigned char) res0;
				copie.data[i*pas+j*3+1] = (unsigned char) res1;
				copie.data[i*pas+j*3+2] = (unsigned char) res2;
		}
	}
	return copie;	
}

/***
* Calcul of a spatial smoothing on a greyscale picture
* kernel = Exponential
*/
Mat spatialSmoothingExp(const Mat & image, double gamma)
{
	Mat copie = image.clone();
	int lignes = image.rows;
	int colonnes = image.cols;
	int pas = image.step; 
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
						res += h*(image.data[(i+u)*pas+(j+v)]);
					}
				} 
				copie.data[i*pas+j] = (unsigned char)(res);
		}
	}
	return copie;	
}

/**
* Calcul of a spatial smoothing on a colored picture
* kernel = Exponential
*/
Mat spatialSmoothingExpColor(const Mat & image, double gamma)
{
	Mat copie = image.clone();
	int lignes = image.rows;
	int colonnes = image.cols;
	int pas = image.step;
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
						res0 += h * image.data[(i+u)*pas+(j+v)*3+0];
						res1 += h * image.data[(i+u)*pas+(j+v)*3+1];
						res2 += h * image.data[(i+u)*pas+(j+v)*3+2];
					}
				} 
				
				copie.data[i*pas+j*3+0] = (unsigned char) res0;
				copie.data[i*pas+j*3+1] = (unsigned char) res1;
				copie.data[i*pas+j*3+2] = (unsigned char) res2;
		}
	}
	return copie;	
}

/**
 * Temporal smoothing for a colored picture
 */
Mat temporalSmoothingColor(String filename){

    /*Results to to return*/
    Mat background;
    /*First ten pictures of the video*/
    Mat stockage[10];
    /** Video*/
    VideoCapture vc = VideoCapture(filename);

    /*Acquiring the ten pictures*/
    for(int i = 0; i< 10; i++){
        vc >> stockage[i];
    }
    background = Mat(stockage[0].size(), stockage[0].type());
	
	double h = 0.1;
    double res0, res1, res2;
    int lignes = background.rows;
    int colonnes = background.cols;
    int pas = background.step;
	
    /*Creation of a picture where each pixel is the average value of the matching pixel in the ten pictures*/ 
    for(int x = 0; x < lignes; x++){
        for(int y = 0; y < colonnes; y ++){

            res0 = res1 = res2 = 0;
            for (int u = 0; u < 10; u++)
            {
				//res = res + (h*(stockage[u].at<Vec3b>(x, y)[k]));
				res0 += stockage[u].data[x*pas+y*3+0];
				res1 += stockage[u].data[x*pas+y*3+1];
				res2 += stockage[u].data[x*pas+y*3+2];
            }
            background.data[x*pas+y*3+0] = (unsigned char)(res0 * h);
            background.data[x*pas+y*3+1] = (unsigned char)(res1 * h);
            background.data[x*pas+y*3+2] = (unsigned char)(res2 * h);

        }
    }
    vc.release();
    //spatialSmoothingAvgColor(background, 1);
    return background;
}

/**
 * Temporal smoothing for a greyscale picture
 */
Mat temporalSmoothing(String filename){

    /*Results to to return*/
    Mat background;
    /*First ten pictures of the video*/
    Mat stockage[10];
    
    Mat temp;
    /** Video*/
    VideoCapture vc = VideoCapture(filename);

    /*Acquiring the ten pictures*/
    for(int i = 0; i< 10; i++){
        vc >> temp;
		/*Switching from a colored picture to a greyscale one*/
        cvtColor(temp, stockage[i], CV_BGR2GRAY);
    }
    background = Mat(stockage[0].size(), stockage[0].type());
	
	double h = 0.1;
    double res;
    int lignes = background.rows;
    int colonnes = background.cols;
    int pas = background.step;
	
    /*Creation of a picture where each pixel is the average value of the matching pixel in the ten pictures*/ 
    for(int x = 0; x < lignes; x++){
        for(int y = 0; y < colonnes; y ++){

            res = 0;
            for (int u = 0; u < 10; u++)
            {
				res += stockage[u].data[x*pas+y];
            }
            background.data[x*pas+y] = (unsigned char)(res * h);

        }
    }
    vc.release();
    //spatialSmoothingAvgColor(background, 1);
    return background;
}


/**
 * Extract the moving element of the @frame (colored picture)
 * by comparing it to the @background picture
 * @backgound picture of the background
 * @frame picture to analyse
 * @return a picture of the extracted foreground
 */
Mat extractForegroundColor(const Mat & background, const Mat & frame){
    Mat res;
    int seuil = 14;
    int lignes = frame.rows;
    int colonnes = frame.cols;
    int pas = frame.step;
	
    /*Control of the similarity of the picture size*/
	if (background.rows == lignes && background.cols == colonnes){
        
        res = Mat(frame.size(), frame.type());

		/*Comparaison of the @frame with the @background and creation of the extract object in a picture*/
		for (int i = 0; i<lignes; i++){
            for (int j = 0; j<colonnes; j++){
            
            	if(abs(frame.data[i*pas+j*3+0] - background.data[i*pas+j*3+0]) > seuil
            	|| abs(frame.data[i*pas+j*3+1] - background.data[i*pas+j*3+1]) > seuil
            	|| abs(frame.data[i*pas+j*3+2] - background.data[i*pas+j*3+2]) > seuil){

                    res.data[i*pas+j*3+0] = frame.data[i*pas+j*3+0];
                    res.data[i*pas+j*3+1] = frame.data[i*pas+j*3+1];
                    res.data[i*pas+j*3+2] = frame.data[i*pas+j*3+2];
				}
                else{
                	res.data[i*pas+j*3+0] = BLUE;
                    res.data[i*pas+j*3+1] = GREEN;
                    res.data[i*pas+j*3+2] = RED;
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

/**
 * Extract the moving element of the @frame (greyscale picture)
 * by comparing it to the @background picture
 * @backgound picture of the background
 * @frame picture to analyse
 * @return a picture of the extracted foreground
 */
Mat extractForeground(const Mat & background, const Mat & frame)
{
    Mat res;
    int seuil = 25;
    int lignes = frame.rows;
    int colonnes = frame.cols;
    int pas  = frame.step;
	
    /*Control of the similarity of the picture size*/
	if (background.rows == lignes && background.cols == colonnes)
	{
        res = Mat(frame.size(), frame.type());

        /*Comparaison of the @frame with the @background and creation of the extract object in a picture*/
        for (int i = 0; i<lignes; i++)
        {
            for (int j = 0; j<colonnes; j++)
            {

                if(abs(frame.data[i*pas+j] - background.data[i*pas+j]) > seuil){

                    res.data[i*pas+j] = frame.data[i*pas+j];

                }
                else{	
                    res.data[i*pas+j] = GRAY;
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
 * Post extraction smoothing with counting of extracted neighbours pixels
 */
Mat preciseSmoothing(Mat image, int nbrVoisin, int requis){
	
	Mat retour(image.size(), image.type());
	int count, total;
	int colonnes = image.cols;
	int lignes = image.rows;
	int pas = image.step;
	int iMax, jMax, iMin, jMin;
	//Vec3b fond = Vec3b(BLUE, GREEN, RED);

	for(int x = 0; x < lignes; x++){
		for(int y = 0; y < colonnes; y++){
			retour.data[x*pas+y*3+0] = image.data[x*pas+y*3+0];
			retour.data[x*pas+y*3+1] = image.data[x*pas+y*3+1];
			retour.data[x*pas+y*3+2] = image.data[x*pas+y*3+2];
			
			if (!(image.data[x*pas+y*3+0] == BLUE 
		       && image.data[x*pas+y*3+1] == GREEN 
		       && image.data[x*pas+y*3+2] == RED))
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
						if (!(image.data[i*pas+j*3+0] == BLUE 
		                   && image.data[i*pas+j*3+1] == GREEN 
		                   && image.data[i*pas+j*3+2] == RED))
							count++;
						if(count > requis)
							break;
					}
					if(count > requis)
						break;
				}
				count--;
				if(count < requis){
					retour.data[x*pas+y*3+0] = BLUE;
					retour.data[x*pas+y*3+1] = GREEN;
					retour.data[x*pas+y*3+2] = RED;
				}
			}
		}
	}
	return retour;
}

/**
* Merge of the blocs in various regions
*/
std::list<Bloc*> split(const Mat & image, unsigned short int* tabCarres, double seuil )
{
	steady_clock::time_point start, end;
	start = steady_clock::now();
	
	int lignes = image.rows;
	int colonnes = image.cols;
	int pas = image.step;
	Mat copie = image.clone();
	std::list<Bloc*> blocsATraiter;
	std::list<Bloc*> blocsDefinitifs;
	Bloc* temp;
	
	int xmin, xmax, ymin, ymax;
	ymin = lignes+1;
	ymax = -1;
	xmin = colonnes+1;
	xmax = -1;

	for(int i = 0; i<lignes; i++)
	{
		for(int j = 0; j < colonnes; j++)
		{
			if (!(copie.data[i*pas+j*3+0] == BLUE 
			   && copie.data[i*pas+j*3+1] == GREEN 
			   && copie.data[i*pas+j*3+2] == RED))
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
				tabCarres[i*colonnes*3+j*3+0] = (double)copie.data[i*pas+j*3+0] * (double)copie.data[i*pas+j*3+0];
				tabCarres[i*colonnes*3+j*3+1] = (double)copie.data[i*pas+j*3+1] * (double)copie.data[i*pas+j*3+1];
				tabCarres[i*colonnes*3+j*3+2] = (double)copie.data[i*pas+j*3+2] * (double)copie.data[i*pas+j*3+2];	
			}
		}
	}

	blocsATraiter.push_back(new Bloc(pixel (xmin, ymin), pixel (xmax, ymax)));
	
	while (!blocsATraiter.empty())
  	{
  		temp = blocsATraiter.front();
		
		if (temp->hasToBeSplitted(image, tabCarres, seuil))
		{
			/*Split of the bloc in 4*/
			int nvX, nvY;
			pixel hg = temp->getP_hg();
			pixel bd = temp->getP_bd();
			nvX = (bd.x - hg.x)/2;
			nvY = (bd.y - hg.y)/2;
	
			Bloc* bloc1  = new Bloc(pixel(hg.x, hg.y), pixel(nvX+hg.x, nvY+hg.y)); //OK
	
			Bloc* bloc2  = new Bloc(pixel(nvX+hg.x+1, hg.y), pixel(bd.x, nvY+hg.y));
	
			Bloc* bloc3  = new Bloc(pixel(hg.x, nvY+hg.y+1), pixel(nvX+hg.x, bd.y));
	
			Bloc* bloc4  = new Bloc(pixel(nvX+hg.x+1, nvY+hg.y+1), pixel(bd.x, bd.y)); //OK
	
			/*Distribution of the neighbours of the bloc*/
	
			for (auto itv = temp->getVoisins().begin(); itv != temp->getVoisins().end(); itv++)
			{
				/*Removal of the main bloc as a neighbour of his own neighbours*/
				for (auto it2 = (*itv)->getVoisins().begin(); it2 != (*itv)->getVoisins().end(); it2++)
				{
					if (*temp == **it2)
					{
						(*itv)->getVoisins().erase(it2);
						break;
					}
				}	
			
				if (bloc1->estVoisin(**itv)){
					bloc1->getVoisins().push_back(*itv);
					(*itv)->getVoisins().push_back(bloc1);
				}
			
				if (bloc2->estVoisin(**itv)){
					bloc2->getVoisins().push_back(*itv);
					(*itv)->getVoisins().push_back(bloc2);
				}
			
				if (bloc3->estVoisin(**itv)){
					bloc3->getVoisins().push_back(*itv);
					(*itv)->getVoisins().push_back(bloc3);
				}
			
				if (bloc4->estVoisin(**itv)){
					bloc4->getVoisins().push_back(*itv);
					(*itv)->getVoisins().push_back(bloc4);
				}
			}
		
			/*Insertion of the new blocs in the neigbours list*/
			bloc1->getVoisins().push_back(bloc2);
			bloc1->getVoisins().push_back(bloc3);
			bloc1->getVoisins().push_back(bloc4);
		
			bloc2->getVoisins().push_back(bloc1);
			bloc2->getVoisins().push_back(bloc3);
			bloc2->getVoisins().push_back(bloc4);
		
			bloc3->getVoisins().push_back(bloc1);
			bloc3->getVoisins().push_back(bloc2);
			bloc3->getVoisins().push_back(bloc4);
		
			bloc4->getVoisins().push_back(bloc1);
			bloc4->getVoisins().push_back(bloc2);
			bloc4->getVoisins().push_back(bloc3);
			
			/*Removal of the old bloc*/
			blocsATraiter.pop_front();
		
			/*Insertion of the new bloc in the list*/
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
//	for(auto it = blocsDefinitifs.begin(); it != blocsDefinitifs.end(); it++)
//	{
//		pixel hg = (*it)->getP_hg();
//		pixel bd = (*it)->getP_bd();
//		rectangle(image, Point(hg.x,hg.y), Point (bd.x,bd.y), 1);
//	}

	end = steady_clock::now();
	std::cout<<"time split : "<< duration_cast<milliseconds>(end-start).count()<<std::endl;
	return blocsDefinitifs;
}

/**
* Merge of the blocs in various regions
*/
std::list<Region*> merge(const std::list<Bloc*> blocs, const Mat & image, const unsigned short int* tabCarres , double seuil)
{
	std::list<Bloc*> blocsLibres = blocs;
	steady_clock::time_point start, end;
	start = steady_clock::now();
	int lignes = image.rows;
	int colonnes = image.cols;
	int pas = image.step;
	std::list<Region*> regionsDef;
	std::list<Bloc*> voisins;
	Region* regTemp;
	Bloc* blocTemp;
	Bloc* blocVois;
	
	/*Blocs contain the "free" blocs, who don't belongs to a region*/
	while (!blocsLibres.empty())
	{
		/*Creation of a new region*/
		blocTemp = blocsLibres.front();
		blocsLibres.pop_front();
		regTemp = new Region();
		regTemp->addBloc(blocTemp);
		regTemp->updateVar(image, tabCarres);
		voisins.insert(voisins.end(), blocTemp->getVoisins().begin(), blocTemp->getVoisins().end());
		/*process of the neighbours of the region (the free neigbours who are in the region)*/

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
					voisins.insert(voisins.end(), blocVois->getVoisins().begin(), blocVois->getVoisins().end());
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
	
	//transformation de l'image de départ pour montrer les resultats du merge
	double * tabMoys;
	int i = 0;
	for (auto itReg = regionsDef.begin(); itReg != regionsDef.end(); itReg++)
	{
		tabMoys = (*itReg)->getMoy();
		for (auto itBloc = (*itReg)->getBlocs().begin(); itBloc != (*itReg)->getBlocs().end(); itBloc++)
		{
			//colorer chaque pixel avec la couleur moyenne de chaque région
			pixel hg = (*itBloc)->getP_hg();
			pixel bd = (*itBloc)->getP_bd();
			for (int i = hg.y; i < bd.y; i++)
			{
				for (int j = hg.x; j < bd.x; j++)
				{
					if (!(image.data[i*pas+j*3+0] == BLUE 
				       && image.data[i*pas+j*3+1] == GREEN 
				       && image.data[i*pas+j*3+2] == RED))
					{
						image.data[i*pas+j*3+0] = tabMoys[0];
						image.data[i*pas+j*3+1] = tabMoys[1];
						image.data[i*pas+j*3+2] = tabMoys[2];
					}
				}
			}
		}
	}
	end = steady_clock::now();
	std::cout<<"time merge : "<< duration_cast<milliseconds>(end-start).count()<<std::endl;
	return regionsDef;
}

/**
* Coloring the regions depending on their color on the image
* The aim of this fonction is to realize the bodypart segmentation
*/
void detectBodyParts(const std::list<Region*> regions, const Mat & image){
	
	int lignes = image.rows;
	int colonnes = image.cols;
	int pas = image.step;
	double * tabMoys;
	for (auto itReg = regions.begin(); itReg != regions.end(); itReg++)
	{
		tabMoys = (*itReg)->getMoy();
		if (tabMoys[0] >= 54 && tabMoys[0] <= 137 && tabMoys[1] >= 67 && tabMoys[1] <= 138 && tabMoys[2] >= 72 && tabMoys[2] <= 145){
			
			for (auto itBloc = (*itReg)->getBlocs().begin(); itBloc != (*itReg)->getBlocs().end(); itBloc++)
			{
				pixel hg = (*itBloc)->getP_hg();
				pixel bd = (*itBloc)->getP_bd();
				for (int i = hg.y; i < bd.y; i++)
				{
					for (int j = hg.x; j < bd.x; j++)
					{
						if (!(image.data[i*pas+j*3+0] == BLUE 
						   && image.data[i*pas+j*3+1] == GREEN 
						   && image.data[i*pas+j*3+2] == RED))
						{
							image.data[i*pas+j*3+0] = 255;
							image.data[i*pas+j*3+1] = 255;
							image.data[i*pas+j*3+2] = 255;
						}
					}
				}
			}
		}
		
		if (tabMoys[0] >= 12 && tabMoys[0] <= 54 && tabMoys[1] >= 26 && tabMoys[1] <= 79 && tabMoys[2] >= 29 && tabMoys[2] <= 86){
		
			for (auto itBloc = (*itReg)->getBlocs().begin(); itBloc != (*itReg)->getBlocs().end(); itBloc++)
			{
				pixel hg = (*itBloc)->getP_hg();
				pixel bd = (*itBloc)->getP_bd();
				for (int i = hg.y; i < bd.y; i++)
				{
					for (int j = hg.x; j < bd.x; j++)
					{
						if (!(image.data[i*pas+j*3+0] == BLUE 
						   && image.data[i*pas+j*3+1] == GREEN 
						   && image.data[i*pas+j*3+2] == RED))
						{
							image.data[i*pas+j*3+0] = 255;
							image.data[i*pas+j*3+1] = 0;
							image.data[i*pas+j*3+2] = 0;
						}
					}
				}
			}
		}
	}
}
