#include "utils.h"

void usage(){
	cout << "Error : expected usage" << endl << "./Analyse <video>" << endl;
}

/*Calcul d'un lissage spatial 
  noyau = moyenne*/
Mat spatialSmoothingAvg(Mat image, double lambda)
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

/*Calcul d'un lissage spatial 
  noyau = Gaussienne*/
Mat spatialSmoothingGauss(Mat image, double sigma)
{

	
	const double pi = 3.14159265358979323846;
	
	for(int i = 1; i<image.rows; i++)
	{
		for(int j = 1; j < image.cols; j++)
		{
			//calcul du noyau
			//h = (1/(2*pi*pow(sigma,2)))*exp(-(pow(i,2)-pow(j,2))/sigma);
			for(int k = 0; k < 3; k++)
			{
				//calcul de la convolution pour chaque composante couleur
				int res = 0;
				for (int u = -1; u < 2; u++)
				{
					for (int v = -1; v < 2; v++)
					{	
						//double h = (1/(2*pi*pow(sigma,2)))*exp((-pow(u,2)-pow(v,2))/(2*sigma));
						//cout<<"h : "<<h<<" u : "<<u<<" v :"<<v<<std::endl;
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
