#include "Bloc_Region.h"

/***********************************BLOC***********************************************/
Bloc::Bloc(){}

Bloc::Bloc(pixel hg, pixel bd, std::list<Bloc*> v) : p_hg(hg), p_bd(bd),voisins(v){}

Bloc::Bloc(pixel hg, pixel bd) : p_hg(hg), p_bd(bd){}

const std::list<Bloc*> & Bloc::getVoisins()
{
	return voisins;
}

bool Bloc::operator==(const Bloc & b)
{
	if (p_hg.x == b.p_hg.x && p_hg.y == b.p_hg.y && p_bd.x == b.p_bd.x && p_bd.y == b.p_bd.y)
		return true;
	
	return false;
}

//double Bloc::calcule_valeur(Mat image){}

bool Bloc::estVoisin(const Bloc & b){

//	std::cout<<"this : "<<std::endl;
//	std::cout<<"Xmin : "<<p_hg.x<<" - Ymin : "<<p_hg.y<<std::endl;
//	std::cout<<"Xmax : "<<p_bd.x<<" - Ymax : "<<p_bd.y<<std::endl;
//	std::cout<<"b : "<<std::endl;
//	std::cout<<"Xmin : "<<b.p_hg.x<<" - Ymin : "<<b.p_hg.y<<std::endl;
//	std::cout<<"Xmax : "<<b.p_bd.x<<" - Ymax : "<<b.p_bd.y<<std::endl;
	
	if (p_hg.x == b.p_bd.x+1)
	{
		//potentiel voisin gauche
		if (b.p_hg.y >= p_hg.y-1 && b.p_hg.y <= p_bd.y+1 || b.p_bd.y >= p_hg.y-1 && b.p_bd.y <= p_bd.y+1)
			return true;
		else
			return false;
	}
	else if (p_bd.x == b.p_hg.x-1)
	{
		//potentiel voisin droite
		if (b.p_hg.y >= p_hg.y-1 && b.p_hg.y <= p_bd.y+1 || b.p_bd.y >= p_hg.y-1 && b.p_bd.y <= p_bd.y+1)
		{
			//std::cout<<"VOISIN DROITE"<<std::endl;
			return true;
		}
		else
			return false;
	}
	else if (p_hg.y == b.p_bd.y-1)
	{
		//potentiel voisin haut
		if (b.p_hg.x >= p_hg.x-1 && b.p_hg.x <= p_bd.x+1 || b.p_bd.x >= p_hg.x-1 && b.p_bd.x <= p_bd.x+1)
			return true;
		else
			return false;
	}
	else if (p_bd.y == b.p_hg.y+1)
	{
		//potentiel voisin bas
		if (b.p_hg.x >= p_hg.x-1 && b.p_hg.x <= p_bd.x+1 || b.p_bd.x >= p_hg.x-1 && b.p_bd.x <= p_bd.x+1)
			return true;
		else
			return false;
	}
	else
		return false; //les deux blocs ne sont pas voisins

}
		
void Bloc::split(std::list<Bloc*> & blocs, const Mat & image, const Mat & imageCarree){
	std::cout<<"SPLIT"<<std::endl;
	int size = (p_bd.x - p_hg.x)*(p_bd.y-p_hg.y);
//	std::cout<<"pix_hg : "<<p_hg.x<<" - "<<p_hg.y<<std::endl;
//	std::cout<<"pix_bd : "<<p_bd.x<<" - "<<p_bd.y<<std::endl;
//	std::cout<<"SIZE : "<<size<<std::endl;
	if(size>25)
	{
		//std::cout<<"I'm in"<<std::endl;
		int n = 0;
		int m0,m1,m2;
		double sum1,sum2,sum0;
		m0 = m1 = m2 = 0;
		sum0 = sum1 = sum2 = 0;
		double var0, var1, var2;
		var0 = var1 = var2 = 0;
		int lignes = image.rows;
		int colonnes = image.cols;
		
	
		for (int i = p_hg.y; i < p_bd.y; i++)
		{
			for (int j = p_hg.x; j < p_bd.x; j++)
			{
				if (!(image.data[i*colonnes*3+j*3+0] == BLUE && image.data[i*colonnes*3+j*3+1] == GREEN && image.data[i*colonnes*3+j*3+2] == RED))
				{
					n++;
					//std::cout<<"J'ai trouvé un pixel";
//					sum0 += imageCarree.data[i*colonnes*3+j*3+0];
//					sum1 += imageCarree.data[i*colonnes*3+j*3+1];
//					sum2 += imageCarree.data[i*colonnes*3+j*3+2];
					
					sum0 += pow(image.data[i*colonnes*3+j*3+0],2);
					std::cout<<"Carré SPLIT  = "<<pow(image.data[i*colonnes*3+j*3+0],2)<<std::endl;
					sum1 += pow(image.data[i*colonnes*3+j*3+1],2);
					sum2 += pow(image.data[i*colonnes*3+j*3+2],2);
				
					m0 += image.data[i*colonnes*3+j*3+0];
					m1 += image.data[i*colonnes*3+j*3+1];
					m2 += image.data[i*colonnes*3+j*3+2];
				}
			}
		}
	
		if (n != 0)
		{
			m0 = m0/n;
			m1 = m1/n;
			m2 = m2/n;
	
			var0 = abs((sum0/n) - pow(m0,2));
			var1 = abs((sum1/n) - pow(m1,2));
			var2 = abs((sum2/n) - pow(m2,2));
		}
		else
		{
			var0 = 0;
			var1 = 0;
			var2 = 0;
		}
		
	
		std::cout<<"nb_pixels : "<<n<<std::endl;
		std::cout<<"vars :"<<var0<<" - "<<var1<<" - "<<var2<<std::endl;
	
		if (var0>100 || var1>100 || var2>100) // a faire varier
		{
			//séparation du bloc en 4
			int nvX, nvY;
			nvX = (p_bd.x - p_hg.x)/2;
			nvY = (p_bd.y - p_hg.y)/2;
//			std::cout<<"nvX : "<<nvX<<" - nvY : "<<nvY<<std::endl;
//			std::cout<<std::endl;
			
			Bloc* bloc1  = new Bloc(pixel(p_hg.x, p_hg.y), pixel(nvX+p_hg.x, nvY+p_hg.y)); //OK
			
			Bloc* bloc2  = new Bloc(pixel(nvX+p_hg.x+1, p_hg.y), pixel(p_bd.x, nvY+p_hg.y));
			
			Bloc* bloc3  = new Bloc(pixel(p_hg.x, nvY+p_hg.y+1), pixel(nvX+p_hg.x, p_bd.y));
			
			Bloc* bloc4  = new Bloc(pixel(nvX+p_hg.x+1, nvY+p_hg.y+1), pixel(p_bd.x, p_bd.y)); //OK
			
			//répartion des vosins du bloc d'origine
			
			for (auto it = voisins.begin(); it != voisins.end(); it++)
			{
				//supression du bloc principal en tant que voisin chez ses propres  voisins
				for (auto it2 = (*it)->voisins.begin(); it2 != (*it)->voisins.end(); it2++)
				{
					if (*this == **it2)
					{
						(*it)->voisins.erase(it2);
						break;
					}
				}	
				
				if (bloc1->estVoisin(**it)){
					//std::cout<<"Le bloc 1 a un voisin"<<std::endl;
					bloc1->voisins.push_back(*it);
					(*it)->voisins.push_back(bloc1);
				}
				
				if (bloc2->estVoisin(**it)){
					//std::cout<<"Le bloc 2 a un voisin"<<std::endl;
					bloc2->voisins.push_back(*it);
					(*it)->voisins.push_back(bloc2);
				}
				
				if (bloc3->estVoisin(**it)){
					//std::cout<<"Le bloc 3 a un voisin"<<std::endl;
					bloc3->voisins.push_back(*it);
					(*it)->voisins.push_back(bloc3);
				}
				
				if (bloc4->estVoisin(**it)){
					//std::cout<<"Le bloc 4 a un voisin"<<std::endl;
					bloc4->voisins.push_back(*it);
					(*it)->voisins.push_back(bloc4);
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
			bloc1->split(blocs, image, imageCarree);
			bloc2->split(blocs, image, imageCarree);
			bloc3->split(blocs, image, imageCarree);
			bloc4->split(blocs, image, imageCarree);
			
		
		}
	}
}

/***********************************REGION*********************************************/

Region::Region(){}
