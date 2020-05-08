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
	 *Poziv simulira izdvajanje jedne karte iz ruke kako bi se razmotrilo
	 *njeno eventualno izbacivanje.
	 */
	kar::Karta izbaci(int i) {
		kar::Karta tmp(ruka[i]);
		
		ruka.erase(ruka.begin() + i);
		
		/*
		std::vector<kar::Karta>::iterator it=ruka.end();
		ruka[i]=*(it-1);
		ruka.pop_back();
		*/
		
		return tmp;
	}
	
	//TODO: Na osnovu "strategije" ili cega god, izbaci neku kartu iz ruke. =D
	//P.S. ako je moguce, naravno (metod se ne poziva bez provere putem ima_sta_da_odigra)
	kar::Karta izbaci_kartu(const kar::Karta &karta_na_talonu) {
		kar::Karta karta_koju_izbacuje = kar::Karta();
		
		for(int i = 0; i < (int)ruka.size(); i++) {
			if(ruka[i].get_boja() == "Crna") {
				karta_koju_izbacuje.set_kar(izbaci(i));
				break;
			}
			if(ruka[i] == karta_na_talonu) {
				karta_koju_izbacuje.set_kar(izbaci(i));
				break;
			}
		}
		return karta_koju_izbacuje;
	}
	
	//TODO: Predstavlja to da li igrac ima, odn. *zeli* da izbaci neku kartu
	bool ima_sta_da_odigra(const kar::Karta &karta_na_talonu) {
		bool ind = false;
		
		std::vector<kar::Karta>::iterator karta;
		for(karta = ruka.begin(); karta != ruka.end(); karta++) {
			if(karta->get_boja() == "Crna") {
				ind = true;
				break;
			}
			if(*karta == karta_na_talonu) {
				ind = true;
				break;
			}
		}
		
		return ind;
	}

	//Metod koji vraca broj karata u ruci
	int broj_karata() {
		return ruka.size(); 
	}
	
	//Metod kojim praznimo ruku igraca
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
	void dodaj_poene(const int &broj_poena) {
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
