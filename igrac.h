#ifndef IGRAC_H
#define IGRAC_H

#include "karta.h"
#include "spil.h"
#include <algorithm>

namespace Ig{

class Igrac{
public:


	Igrac()
	{

	}	


	void vuci(kar::Karta k)
	{
		ruka.push_back(k);

	}	


	kar::Karta izbaci(int i)
	{
		kar::Karta tmp(ruka[i]);
		std::vector<kar::Karta>::iterator it=ruka.end();
		ruka[i]=*(it-1);
		ruka.pop_back();
		return tmp;

	}


	int broj_karata()
	{
		return ruka.size(); 
	}


	void sortiraj_ruku()
	{
		std::sort(ruka.begin(),ruka.end(),kar::compare);
	}


	void sortiraj_obrnuto()
	{
		sortiraj_ruku();
		std::reverse(ruka.begin(),ruka.end());	
	}


	void ispis_ruke()
	{
		for(std::vector<kar::Karta>::iterator it=ruka.begin();it<ruka.end();it++)
			it->toString();
	}


	int broj_poena_u_ruci()
	{
		int poeni=0;
		std::vector<kar::Karta>:: iterator it=ruka.begin();
		for(;it<ruka.end();it++)
		 poeni+=it->Vrednost();
		return poeni;
	}


	void set_broj_poena(int broj)
	{
		_broj_bodova=broj;
	}


private:
	std::vector<kar::Karta> ruka;
	int _broj_bodova;
};


}
#endif
