#include "Bloc_Region.h"

/***********************************BLOC***********************************************/
Bloc::Bloc() : varB(0),varG(0),varR(0) {}

Bloc::Bloc(pixel hg, pixel bd, std::list<Bloc*> v) : p_hg(hg), p_bd(bd),voisins(v), varB(0),varG(0),varR(0){}

Bloc::Bloc(pixel hg, pixel bd) : p_hg(hg), p_bd(bd), varB(0),varG(0),varR(0){}

std::list<Bloc*> & Bloc::getVoisins()
{
	return voisins;
}

const pixel & Bloc::getP_hg(){
	return p_hg;
}
		
const pixel & Bloc::getP_bd(){
	return p_bd;
}

double * Bloc::getVar()
{
	double * vars = new double[3];
	vars[0] = varB;
	vars[1] = varG;
	vars[2] = varR;
	return vars;
}

bool Bloc::operator==(const Bloc & b)
{
	//two blocs are equals if the pixel describing them are the same
	if (p_hg.x == b.p_hg.x && p_hg.y == b.p_hg.y && p_bd.x == b.p_bd.x && p_bd.y == b.p_bd.y)
		return true;
	
	return false;
}


bool Bloc::estVoisin(const Bloc & b){
	
	if (p_hg.x == b.p_bd.x+1)
	{
		//the blocs are side by side (right side of the current bloc)
		if (((b.p_hg.y >= p_hg.y-1 && b.p_hg.y <= p_bd.y+1) || (b.p_bd.y >= p_hg.y-1 && b.p_bd.y <= p_bd.y+1))
			//the ymin and the ymax of bloc b are included between the ymin and the ymax of current bloc
		 || ((p_hg.y >= b.p_hg.y-1 && p_hg.y <= b.p_bd.y+1) || (p_bd.y >= b.p_hg.y-1 && p_bd.y <= b.p_bd.y+1)))
		 	//the ymin and the ymax of current bloc are included between the ymin and the ymax of bloc b
			return true;
		else
			return false;
	}
	else if (p_bd.x == b.p_hg.x-1)
	{
		//the blocs are side by side (left side of the current bloc)
		if (((b.p_hg.y >= p_hg.y-1 && b.p_hg.y <= p_bd.y+1) || (b.p_bd.y >= p_hg.y-1 && b.p_bd.y <= p_bd.y+1))
			//the ymin and the ymax of bloc b are included between the ymin and the ymax of current bloc
		 || ((p_hg.y >= b.p_hg.y-1 && p_hg.y <= b.p_bd.y+1) || (p_bd.y >= b.p_hg.y-1 && p_bd.y <= b.p_bd.y+1)))
		 	//the ymin and the ymax of current bloc are included between the ymin and the ymax of bloc b
		{
			return true;
		}
		else
			return false;
	}
	else if (p_hg.y == b.p_bd.y+1)
	{
		//the blocs are side by side (top of the current bloc)
		if (((b.p_hg.x >= p_hg.x-1 && b.p_hg.x <= p_bd.x+1) || (b.p_bd.x >= p_hg.x-1 && b.p_bd.x <= p_bd.x+1))
			//the xmin and the xmax of bloc b are included between the xmin and the xmax of current bloc
		||  ((p_hg.x >= b.p_hg.x-1 && p_hg.x <= b.p_bd.x+1) || (p_bd.x >= b.p_hg.x-1 && p_bd.x <= b.p_bd.x+1)))
			//the xmin and the xmax of current bloc are included between the xmin and the xmax of bloc b
			return true;
		else
			return false;
	}
	else if (p_bd.y == b.p_hg.y-1)
	{
		//the blocs are side by side (bottom of the current bloc)
		if (((b.p_hg.x >= p_hg.x-1 && b.p_hg.x <= p_bd.x+1) || (b.p_bd.x >= p_hg.x-1 && b.p_bd.x <= p_bd.x+1))
			//the xmin and the xmax of bloc b are included between the xmin and the xmax of current bloc
		||  ((p_hg.x >= b.p_hg.x-1 && p_hg.x <= b.p_bd.x+1) || (p_bd.x >= b.p_hg.x-1 && p_bd.x <= b.p_bd.x+1)))
			//the xmin and the xmax of current bloc are included between the xmin and the xmax of bloc b
			return true;
		else
			return false;
	}
	else
		return false;

}
		
bool Bloc::hasToBeSplitted(const Mat & image, const unsigned short int tabCarres [], double seuil){
	//calculation of the bloc size
	int size = (p_bd.x - p_hg.x)*(p_bd.y-p_hg.y);
	if(size>25) // if the bloc is big enough
	{
		//calculation of the variance for each canal
		int n = 0;
		int m0,m1,m2;
		double sum1,sum2,sum0;
		m0 = m1 = m2 = 0;
		sum0 = sum1 = sum2 = 0;
		int lignes = image.rows;
		int colonnes = image.cols;
		int pas = image.step;

		//for each pixel
		for (int i = p_hg.y; i < p_bd.y; i++)
		{
			for (int j = p_hg.x; j < p_bd.x; j++)
			{
				//if the pixel is not a background pixel
				if (!(image.data[i*pas+j*3+0] == BLUE 
				   && image.data[i*pas+j*3+1] == GREEN 
				   && image.data[i*pas+j*3+2] == RED))
				{
					n++; // increment of the pixel amount
					sum0 += tabCarres[i*colonnes*3+j*3+0]; //sum of the values (square)
					sum1 += tabCarres[i*colonnes*3+j*3+1];
					sum2 += tabCarres[i*colonnes*3+j*3+2];
				
					m0 += image.data[i*pas+j*3+0]; //sum of the values
					m1 += image.data[i*pas+j*3+1];
					m2 += image.data[i*pas+j*3+2];
				}
			}
		}
	
		if (n != 0)
		{
			//calcultation of the means
			m0 = m0/n;
			m1 = m1/n;
			m2 = m2/n;
	
			//calculation of the variances
			varB = abs((sum0/n) - pow(m0,2));
			varG = abs((sum1/n) - pow(m1,2));
			varR = abs((sum2/n) - pow(m2,2));
		}
	
		// if one of the variances is above a certain threshold, the bloc has to be splitted
		if (varB>seuil || varG>seuil || varR>seuil) 
				return true;
		else
			return false;
	}
	else
		return false;
}

/***********************************REGION*********************************************/

Region::Region() : varB(0), varG(0), varR(0), moyB(0), moyG(0), moyR(0){}

void Region::addBloc(Bloc * b){
	blocs.push_back(b);
}

const std::list<Bloc*> & Region::getBlocs()
{
	return blocs;
}

double * Region::getMoy()
{
	double * moys = new double[3];
	moys[0] = moyB;
	moys[1] = moyG;
	moys[2] = moyR;
	return moys;
}

void Region::updateVar(const Mat & image, const unsigned short int tabCarres [])
{
	int n = 0;
	int m0,m1,m2;
	double sum1,sum2,sum0;
	m0 = m1 = m2 = 0;
	sum0 = sum1 = sum2 = 0;
	int lignes = image.rows;
	int colonnes = image.cols;
	pixel hg,bd;
	
	//for each bloc
	for (auto it = blocs.begin(); it != blocs.end(); it++)
	{
		//calculation of the variance for each canal
		hg = (*it)->getP_hg();
		bd = (*it)->getP_bd();
		
		for (int i = hg.y; i < bd.y; i++)
		{
			for (int j = hg.x; j < bd.x; j++)
			{
				//if the pixel is not a background pixel
				if (!(image.data[i*colonnes*3+j*3+0] == BLUE 
				   && image.data[i*colonnes*3+j*3+1] == GREEN 
				   && image.data[i*colonnes*3+j*3+2] == RED))
				{
					n++;// increment of the pixel amount
					sum0 += tabCarres[i*colonnes*3+j*3+0]; //sum of the values (square)
					sum1 += tabCarres[i*colonnes*3+j*3+1];
					sum2 += tabCarres[i*colonnes*3+j*3+2];
				
					m0 += image.data[i*colonnes*3+j*3+0]; //sum of the values
					m1 += image.data[i*colonnes*3+j*3+1];
					m2 += image.data[i*colonnes*3+j*3+2];
				}
			}
		}
	}
	
	if (n != 0)
	{
		//calcultation of the means
		moyB = m0/n;
		moyG = m1/n;
		moyR = m2/n;

		//calculation of the variances
		varB = abs((sum0/n) - pow(m0,2));
		varG = abs((sum1/n) - pow(m1,2));
		varR = abs((sum2/n) - pow(m2,2));
	}
	
}

bool Region::isConsistent(Bloc & b, double seuil)
{
	double * tabVars = b.getVar();
	// if all the differences are beneath a certain threshold, the bloc can be added to the region
	if(abs(varB-tabVars[0])<seuil && abs(varG-tabVars[1])<seuil && abs(varR-tabVars[2])<seuil)
		return true;
	else
		return false;
}

