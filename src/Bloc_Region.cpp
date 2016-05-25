#include "Bloc_Region.h"

/***********************************BLOC***********************************************/
Bloc::Bloc(){}

Bloc::Bloc(pixel hg, pixel bd, std::list<Bloc*> v) : p_hg(hg), p_bd(bd),voisins(v){}

Bloc::Bloc(pixel hg, pixel bd) : p_hg(hg), p_bd(bd){}

bool Bloc::operator==(const Bloc & b)
{
	if (p_hg.x == b.p_hg.x && p_hg.y == b.p_hg.y && p_bd.x == b.p_bd.x && p_bd.y == b.p_bd.y)
		return true;
	
	return false;
}

//double Bloc::calcule_valeur(Mat image){}

bool Bloc::estVoisin(const Bloc & b){

	
	return false;

}
		
void Bloc::split(std::list<Bloc*> & blocs, const Mat & image){

	int size = (p_bd.x - p_hg.x)*(p_bd.y-p_hg.y);
	std::cout<<"SIZE : "<<size<<std::endl;
	if(size>10)
	{
		std::cout<<"I'm in"<<std::endl;
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
	
		if (n != 0)
		{
			m0 = m0/n;
			m1 = m1/n;
			m2 = m2/n;
	
			var0 = (sum0/n) - pow(m0,2);
			var1 = (sum1/n) - pow(m1,2);
			var2 = (sum2/n) - pow(m2,2);
		}
		
	
		std::cout<<"nb_pixels : "<<n<<std::endl;
		std::cout<<"vars :"<<var0<<" - "<<var1<<" - "<<var2<<std::endl;
	
		if (var0>0.001 && var1>0.001 && var2>0.001)
		{
			//séparation du bloc en 4
			int nvX, nvY;
			nvX = (p_bd.x - p_hg.x)/2;
			nvY = (p_bd.y - p_hg.y)/2;
			std::cout<<"nvX : "<<nvX<<" - nvY : "<<nvY<<std::endl;
		
			Bloc* bloc1  = new Bloc(pixel(p_hg.x, p_hg.y), pixel(nvX+p_hg.x, nvY+p_hg.y)); //OK
			
			Bloc* bloc2  = new Bloc(pixel(nvX+p_hg.x, p_hg.y), pixel(p_bd.x, nvY+p_hg.y));
			
			Bloc* bloc3  = new Bloc(pixel(p_hg.x, nvY+p_hg.y), pixel(nvX+p_hg.x, p_bd.y));
			
			Bloc* bloc4  = new Bloc(pixel(nvX+p_hg.x, nvY+p_hg.y), pixel(p_bd.x, p_bd.y)); //OK
			
			//répartion des vosins du bloc d'origine
			
			for (auto it = voisins.begin(); it != voisins.end(); it++)
			{
				if (bloc1->estVoisin(**it))
					bloc1->voisins.push_back(*it);
				
				if (bloc2->estVoisin(**it))
					bloc2->voisins.push_back(*it);
				
				if (bloc3->estVoisin(**it))
					bloc3->voisins.push_back(*it);
				
				if (bloc3->estVoisin(**it))
					bloc3->voisins.push_back(*it);
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
			
			//insertion des nouveaux blocs dans la listes
			blocs.push_back(bloc1);
			blocs.push_back(bloc2);
			blocs.push_back(bloc3);
			blocs.push_back(bloc4);
			
			//suppression de l'ancien bloc
			for (auto it = blocs.begin(); it != blocs.end(); it++)
			{
				if (**it == *this){
					blocs.erase(it);
					break;
				}
			}
			delete this;
			//https://isocpp.org/wiki/faq/freestore-mgmt#delete-this
			
			
			//on split les nouveau blocs
//			bloc1->split(blocs, image);
//			bloc2->split(blocs, image);
//			bloc3->split(blocs, image);
//			bloc4->split(blocs, image);
			
		
		}
	}
}

/***********************************REGION*********************************************/

Region::Region(){}
