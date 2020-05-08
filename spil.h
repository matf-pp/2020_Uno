#ifndef SPIL_H
#define SPIL_H

#include "karta.h"
#include <cstdlib>
#include <string>
#include <vector>
#include <ctime>

namespace spil {

class Spil {
public:
	//Konstruktor; sastavlja spil.
	Spil() {
		int j;
		for(int i=0;i<80;i++) {
			switch(i/10) {
				case 0: {
					j=i%10;
					kar::Karta k(std::string("Crvena"),std::to_string(j));
					_spil.push_back(k);
					break;	
				}
				case 1: {
					j=i%10;
					if(j!=0){
					kar::Karta k(std::string("Crvena"),std::to_string(j));
					_spil.push_back(k);
					}
					break;	
				}
				case 2: {
										
					j=i%10;
					kar::Karta k(std::string("Plava"),std::to_string(j));
					_spil.push_back(k);
					break;	
				}
				case 3: {
					j=i%10;
					if(j!=0){	
					kar::Karta k(std::string("Plava"),std::to_string(j));
					_spil.push_back(k);
					}
					break;	
				}
				case 4: {
					j=i%10;
					kar::Karta k(std::string("Zuta"),std::to_string(j));
					_spil.push_back(k);
					break;	
				}
				case 5: {
					j=i%10;
					if(j!=0){
					kar::Karta k(std::string("Zuta"),std::to_string(j));
					_spil.push_back(k);
					}
					break;	
				}
				case 6: {
					j=i%10;
					kar::Karta k(std::string("Zelena"),std::to_string(j));
					_spil.push_back(k);
					break;	
				}
				case 7: {					
					j=i%10;
					if(j!=0){
					kar::	Karta k(std::string("Zelena"),std::to_string(j));
					_spil.push_back(k);	
					}					
					break;			
				}
			}
		}
		kar::Karta k(std::string("Crna"),std::string("+4"));
		_spil.push_back(k);
		_spil.push_back(k);
		_spil.push_back(k);
		_spil.push_back(k);

		k.set_karta(std::string("Crna"),std::string("Joker"));
		_spil.push_back(k);
		_spil.push_back(k);
		_spil.push_back(k);
		_spil.push_back(k);

		k.set_karta(std::string("Crvena"),std::string("Block"));
		_spil.push_back(k);
		_spil.push_back(k);

		k.set_karta(std::string("Plava"),std::string("Block"));
		_spil.push_back(k);
		_spil.push_back(k);		

		k.set_karta(std::string("Zelena"),std::string("Block"));
		_spil.push_back(k);
		_spil.push_back(k);		

		k.set_karta(std::string("Zuta"),std::string("Block"));
		_spil.push_back(k);
		_spil.push_back(k);

		k.set_karta(std::string("Crvena"),std::string("Reverse"));
		_spil.push_back(k);
		_spil.push_back(k);

		k.set_karta(std::string("Plava"),std::string("Reverse"));
		_spil.push_back(k);
		_spil.push_back(k);		

		k.set_karta(std::string("Zelena"),std::string("Reverse"));
		_spil.push_back(k);
		_spil.push_back(k);		

		k.set_karta(std::string("Zuta"),std::string("Reverse"));
		_spil.push_back(k);
		_spil.push_back(k);

		k.set_karta(std::string("Crvena"),std::string("+2"));
		_spil.push_back(k);
		_spil.push_back(k);

		k.set_karta(std::string("Plava"),std::string("+2"));
		_spil.push_back(k);		
		_spil.push_back(k);

		k.set_karta(std::string("Zelena"),std::string("+2"));		
		_spil.push_back(k);		
		_spil.push_back(k);

		k.set_karta(std::string("Zuta"),std::string("+2"));
		_spil.push_back(k);
		_spil.push_back(k);
		
	}
	
	//Konstruktor kopije
	Spil(std::vector<kar::Karta> spil) {
		_spil = spil;
	}
	
	//Metod koji skida kartu sa vrha spila
	kar::Karta izvuci_kartu() {
		kar::Karta tmp(*(_spil.end()-1));
		_spil.pop_back();
		return tmp;
	}
	
	//Metod koji vraca broj karata u spilu
	int broj_karata_u_spilu() {
		return _spil.size();
	}
	
	//Metod koji na osnovu niza karata formira spil
	kar::Karta set_spil(std::vector<kar::Karta> spil)
	{
		kar::Karta tmp(*(spil.end()-1));
		spil.pop_back();
		while(spil.size()!=0)
		{
			_spil.push_back(*(spil.end()-1));
			spil.pop_back();
		}
		return tmp;
	}
	
	//Metod koji mesa spil
	void promesaj()
	{
	    std::random_device rd;
	    std::mt19937 gen(rd());
	    std::uniform_int_distribution<> dis(0,107);

		int j;
		kar::Karta* tmp=new kar::Karta();
		for(int i=0;i<int(_spil.size());i++)
		{
			j=dis(gen);
			tmp->set_kar(_spil[j]);
			_spil[j].set_kar(_spil[i]);
			_spil[i].set_kar(*tmp);


		}
	}

	/* Funkcija se nigde ne koristi
	void print() {
		int i=0;
		std::vector<kar::Karta>::iterator it;
		for(it=_spil.begin();it<_spil.end();it++) {
			it->toString();
			std::cout<<i<<"\n"<<_spil.size();
			i++;		
		}
	}
	*/
	
	//toString
	string toString() {
		ostringstream buffer;
		
		buffer << "[ ";
		std::vector<kar::Karta>::iterator it;
		for(it = _spil.begin(); it < _spil.end(); it++) {
			buffer << it->toString() << " ";
		}
		buffer << "]";
		
		return buffer.str();
	}

private:
	std::vector <kar::Karta> _spil;
};

}

#endif
