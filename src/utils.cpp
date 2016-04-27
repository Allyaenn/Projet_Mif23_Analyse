#include "utils.h"

void usage(){
	cout << "Error : expected usage" << endl << "./Analyse <video>" << endl;
}

/*Calcul d'un lissage spatial 
  noyau = moyenne*/
Mat spatialSmoothingAvg(Mat image, double lambda)
{
	double h;
	for(int i = 1; i<image.rows; i++)
	{
		for(int j = 1; j<image.cols; j++)
		{
			//calcul du noyau
			unsigned char res = 0;
			for (int u = -1; u < 2; u++)
			{
				for (int v = -1; v < 2; v++)
				{	
					double h = 1.0/9.0;
					//cout<<"h : "<<h<<" u : "<<u<<" v :"<<v<<std::endl;
					res = res + (h*image.at<unsigned char>(i+u,j+v));
				}
			} 
			image.at<unsigned char>(i,j) = res;
		}
	}
	
	return image;
}

Mat spatialSmoothingAvgColor(Mat image, double lambda)
{
	double h;
	for(int i = 1; i<image.rows; i++)
	{
		for(int j = 1; j<image.cols; j++)
		{
			for(int k = 0; k < 3; k++)
			{
				//calcul de la convolution pour chaque composante couleur
				int res = 0;
				for (int u = -1; u < 2; u++)
				{
					for (int v = -1; v < 2; v++)
					{	
						double h = 1.0/9.0;
						res = res + (h*(image.at<Vec3b>(i+u,j+v)[k]));
					}
				} 
				image.at<Vec3b>(i,j)[k] = res;
			}
		}
	}
	
	return image;
}


/*Calcul d'un lissage spatial 
  noyau = Gaussienne*/
Mat spatialSmoothingGauss(Mat image, double sigma)
{
	const double pi = 3.14159265358979323846;
	
	for(int i = 1; i<image.rows; i++)
	{
		for(int j = 1; j < image.cols; j++)
		{
				unsigned char res = 0;
				for (int u = -1; u < 2; u++)
				{
					for (int v = -1; v < 2; v++)
					{	
						double h = (1/(2*pi*pow(sigma,2)))*exp((-pow(u,2)-pow(v,2))/(2*sigma));
						//cout<<"h : "<<h<<" u : "<<u<<" v :"<<v<<std::endl;
						res = res + (h*(image.at<unsigned char>(i+u,j+v)));
					}
				} 
				image.at<unsigned char>(i,j) = res;
		}
	}
	return image;	
}


//BGR

//moyenne -> on a un carré donc tant pis pour le sinon !

/*Calcul d'un lissage spatial 
  noyau = fonction exponentielle*/
Mat spatialSmoothingExp(Mat image, double gamma)
{
	double h;
	for(int i = 0; i<image.rows; i++)
	{
		for(int i = 0; i<image.rows; i++)
		{
			//calcul du noyau 

			//calcul de la convolution
		}
	}
	
	return image;
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
