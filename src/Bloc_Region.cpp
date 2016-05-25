#include "Bloc_Region.h"

/***********************************BLOC***********************************************/
Bloc::Bloc(){}

Bloc::Bloc(pixel hg, pixel bd, std::list<Bloc*> v) : p_hg(hg), p_bd(bd),voisins(v){}

Bloc::Bloc(pixel hg, pixel bd) : p_hg(hg), p_bd(bd){}

//double Bloc::calcule_valeur(Mat image){}
		
void Bloc::split(std::list<Bloc*> & bloc, const Mat & image){

	int n = 0;
	int m0,m1,m2;
	double sum1,sum2,sum0;
	m0 = m1 = m2 = 0;
	sum0 = sum1 = sum2 = 0;
	double var0, var1, var2;
	var0 = var1 = var2 = 0;
	int lignes = p_bd.y - p_hg.y;
	int colonnes = p_bd.x - p_hg.x;
	
	for (int i = p_hg.y; i < p_bd.y; i++)
	{
		for (int j = p_hg.x; j < p_bd.x; j++)
		{
			if (!(image.data[i*colonnes*3+j*3+0] == 0 && image.data[i*colonnes*3+j*3+1] == 100 && image.data[i*colonnes*3+j*3+2] == 0))
			{
				n++;
				sum0 = pow(image.data[i*colonnes*3+j*3+0],2);
				sum1 = pow(image.data[i*colonnes*3+j*3+1],2);
				sum2 = pow(image.data[i*colonnes*3+j*3+2],2);
				
				m0 = image.data[i*colonnes*3+j*3+0];
				m1 = image.data[i*colonnes*3+j*3+1];
				m2 = image.data[i*colonnes*3+j*3+2];
			}
		}
	}
	
	m0 = m0/n;
	m1 = m1/n;
	m2 = m2/n;
	
	var0 = (sum0/n) - pow(m0,2);
	var1 = (sum1/n) - pow(m1,2);
	var2 = (sum2/n) - pow(m2,2);
	
	std::cout<<"nb_pixels : "<<n<<std::endl;
	std::cout<<"vars :"<<var0<<" - "<<var1<<" - "<<var2<<std::endl;
	
	if (var0>0.01 && var1>0.01 && var2>0.01)
	{
		//séparation du bloc en 4
		
	}

}

/***********************************REGION*********************************************/

Region::Region(){}
