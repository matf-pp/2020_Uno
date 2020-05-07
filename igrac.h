#ifndef IGRAC_H
#define IGRAC_H

#include "karta.h"
#include "spil.h"
#include <algorithm>
#include <sstream>
#include <string>

namespace Ig {

class Igrac {
public:
	//Konstruktor
	Igrac(const std::string &ime) {
		_ime = ime;
		_broj_poena = 0;
	}

	//Metod koji postavlja kartu u ruku igraca
	void vuci(kar::Karta k) {
		ruka.push_back(k);
	}	

	/*Metod koji iz ruke igraca izbacuje jednu kartu
	 *Poziv simulira odvajanje jedne karte iz ruke kako bi se razmotrilo
	 *njeno eventualno izbacivanje.
	 */
	kar::Karta izbaci(int i) {
		kar::Karta tmp(ruka[i]);
		std::vector<kar::Karta>::iterator it=ruka.end();
		ruka[i]=*(it-1);
		ruka.pop_back();
		return tmp;
	}

	//Metod koji vraca broj karata u ruci
	int broj_karata() {
		return ruka.size(); 
	}
	
	void izbaci_sve_karte() {
		ruka.clear();
	}

	//Metodi za sortiranje karata u ruci
	void sortiraj_ruku() {
		std::sort(ruka.begin(),ruka.end(),kar::compare);
	}

	void sortiraj_obrnuto() {
		sortiraj_ruku();
		std::reverse(ruka.begin(),ruka.end());	
	}

	//Metod koji "ispisuje" karte u ruci igraca
	std::string ispis_ruke() {
		ostringstream buffer;
		
		buffer << "[ ";
		for(std::vector<kar::Karta>::iterator it=ruka.begin();it<ruka.end();it++) {
			buffer << it->toString() << " ";
		}
		buffer << "]";
		
		return buffer.str();
	}

	//Metod koji vraca broj poena u ruci
	int broj_poena_u_ruci() {
		int poeni=0;
		std::vector<kar::Karta>:: iterator it=ruka.begin();
		for(;it<ruka.end();it++)
		 poeni+=it->get_vrednost();
		return poeni;
	}
	
	//Getter:
	int get_broj_poena() {
		return _broj_poena;
	}

	/*
	void set_broj_poena(int broj) {
		_broj_poena=broj;
	}
	*/
	
	//Metod koji dodaje osvojene poene na ukupne poene igraca
	void dodaj_poene(int broj_poena) {
		_broj_poena += broj_poena;
	}
	
	//toString
	std::string toString() const {
		ostringstream buffer;
		buffer << "[";
		buffer << _ime;
		buffer << "]";
		
		return buffer.str();
	}

private:
	std::string _ime;
	std::vector<kar::Karta> ruka;
	int _broj_poena;
};

}

#endif
