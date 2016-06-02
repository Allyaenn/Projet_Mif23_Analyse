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
	if (p_hg.x == b.p_hg.x && p_hg.y == b.p_hg.y && p_bd.x == b.p_bd.x && p_bd.y == b.p_bd.y)
		return true;
	
	return false;
}

bool Bloc::estVoisin(const Bloc & b){
	
	if (p_hg.x == b.p_bd.x+1)
	{
		if (((b.p_hg.y >= p_hg.y-1 && b.p_hg.y <= p_bd.y+1) || (b.p_bd.y >= p_hg.y-1 && b.p_bd.y <= p_bd.y+1))
		 || ((p_hg.y >= b.p_hg.y-1 && p_hg.y <= b.p_bd.y+1) || (p_bd.y >= b.p_hg.y-1 && p_bd.y <= b.p_bd.y+1)))
			return true;
		else
			return false;
	}
	else if (p_bd.x == b.p_hg.x-1)
	{
		if (((b.p_hg.y >= p_hg.y-1 && b.p_hg.y <= p_bd.y+1) || (b.p_bd.y >= p_hg.y-1 && b.p_bd.y <= p_bd.y+1))
		 || ((p_hg.y >= b.p_hg.y-1 && p_hg.y <= b.p_bd.y+1) || (p_bd.y >= b.p_hg.y-1 && p_bd.y <= b.p_bd.y+1)))
		{
			return true;
		}
		else
			return false;
	}
	else if (p_hg.y == b.p_bd.y+1)
	{
		if (((b.p_hg.x >= p_hg.x-1 && b.p_hg.x <= p_bd.x+1) || (b.p_bd.x >= p_hg.x-1 && b.p_bd.x <= p_bd.x+1))
		||  ((p_hg.x >= b.p_hg.x-1 && p_hg.x <= b.p_bd.x+1) || (p_bd.x >= b.p_hg.x-1 && p_bd.x <= b.p_bd.x+1)))
			return true;
		else
			return false;
	}
	else if (p_bd.y == b.p_hg.y-1)
	{
		if (((b.p_hg.x >= p_hg.x-1 && b.p_hg.x <= p_bd.x+1) || (b.p_bd.x >= p_hg.x-1 && b.p_bd.x <= p_bd.x+1))
		||  ((p_hg.x >= b.p_hg.x-1 && p_hg.x <= b.p_bd.x+1) || (p_bd.x >= b.p_hg.x-1 && p_bd.x <= b.p_bd.x+1)))
			return true;
		else
			return false;
	}
	else
		return false; //les deux blocs ne sont pas voisins

}
		
bool Bloc::hasToBeSplitted(const Mat & image, const unsigned short int tabCarres [], double seuil){
	int size = (p_bd.x - p_hg.x)*(p_bd.y-p_hg.y);
	if(size>25)
	{
		int n = 0;
		int m0,m1,m2;
		double sum1,sum2,sum0;
		m0 = m1 = m2 = 0;
		sum0 = sum1 = sum2 = 0;
		int lignes = image.rows;
		int colonnes = image.cols;
		int pas = image.step;

		for (int i = p_hg.y; i < p_bd.y; i++)
		{
			for (int j = p_hg.x; j < p_bd.x; j++)
			{
				if (!(image.data[i*pas+j*3+0] == BLUE 
				   && image.data[i*pas+j*3+1] == GREEN 
				   && image.data[i*pas+j*3+2] == RED))
				{
					n++;
					sum0 += tabCarres[i*colonnes*3+j*3+0];
					sum1 += tabCarres[i*colonnes*3+j*3+1];
					sum2 += tabCarres[i*colonnes*3+j*3+2];
				
					m0 += image.data[i*pas+j*3+0];
					m1 += image.data[i*pas+j*3+1];
					m2 += image.data[i*pas+j*3+2];
				}
			}
		}
	
		if (n != 0)
		{
			m0 = m0/n;
			m1 = m1/n;
			m2 = m2/n;
	
			varB = abs((sum0/n) - pow(m0,2));
			varG = abs((sum1/n) - pow(m1,2));
			varR = abs((sum2/n) - pow(m2,2));
		}
	
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
	
	for (auto it = blocs.begin(); it != blocs.end(); it++)
	{
		hg = (*it)->getP_hg();
		bd = (*it)->getP_bd();
		
		for (int i = hg.y; i < bd.y; i++)
		{
			for (int j = hg.x; j < bd.x; j++)
			{
				if (!(image.data[i*colonnes*3+j*3+0] == BLUE 
				   && image.data[i*colonnes*3+j*3+1] == GREEN 
				   && image.data[i*colonnes*3+j*3+2] == RED))
				{
					n++;
					//std::cout<<"J'ai trouvé un pixel";
					sum0 += tabCarres[i*colonnes*3+j*3+0];
					sum1 += tabCarres[i*colonnes*3+j*3+1];
					sum2 += tabCarres[i*colonnes*3+j*3+2];
				
					m0 += image.data[i*colonnes*3+j*3+0];
					m1 += image.data[i*colonnes*3+j*3+1];
					m2 += image.data[i*colonnes*3+j*3+2];
				}
			}
		}
	}
	
	if (n != 0)
	{
		moyB = m0/n;
		moyG = m1/n;
		moyR = m2/n;

		varB = abs((sum0/n) - pow(m0,2));
		varG = abs((sum1/n) - pow(m1,2));
		varR = abs((sum2/n) - pow(m2,2));
	}
	
}

bool Region::isConsistent(Bloc & b, double seuil)
{
	double * tabVars = b.getVar();
	if(abs(varB-tabVars[0])<seuil && abs(varG-tabVars[1])<seuil && abs(varR-tabVars[2])<seuil)
		return true;
	else
		return false;
}

