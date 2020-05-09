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
		
		return tmp;
	}
	
	//TODO:prokomentarisati
	kar::Karta izbaci(kar::Karta karta) {
		kar::Karta tmp(karta);
		
		for(int i = 0; i < (int)ruka.size(); i++) {
			if(karta.get_boja() == ruka[i].get_boja() && karta.get_znak() == ruka[i].get_znak()) {
				ruka.erase(ruka.begin() + i);
				break;
			}
		}
		return tmp;
	}
	
	//TODO:prokomentarisati
	kar::Karta izbaci_kartu(const kar::Karta &karta_na_talonu, const std::vector<int> &broj_karata_ostalih_igraca, std::string &odabrana_boja) {
		int min = broj_karata_ostalih_igraca[0];
		for(int i = 1; i < (int)broj_karata_ostalih_igraca.size(); i++) {
			if(broj_karata_ostalih_igraca[i] < min) {
				min = broj_karata_ostalih_igraca[i];
			}
		}
		
		int broj_crvenih_karata = 0;
		int broj_zutih_karata = 0;
		int broj_zelenih_karata = 0;
		int broj_plavih_karata = 0;
		
		int broj_poena_crveno = 0;
		int broj_poena_zuto = 0;
		int broj_poena_zeleno = 0;
		int broj_poena_plavo = 0;
		
		int broj_crvenih_efekata = 0;
		int broj_zutih_efekata = 0;
		int broj_zelenih_efekata = 0;
		int broj_plavih_efekata = 0;
		
		int broj_karata_u_ruci = ruka.size();
		
		std::vector<kar::Karta> validne_karte;
		
		for(int i = 0; i < broj_karata_u_ruci; i++) {
			std::string boja = ruka[i].get_boja();
			std::string znak = ruka[i].get_znak();
			if(boja == "Crvena") {
				broj_crvenih_karata++;
				broj_poena_crveno += ruka[i].get_vrednost();
				if(znak == "+2" || znak == "Block" || znak == "Reverse") {
					broj_crvenih_efekata++;
				}
			} else if(boja == "Zuta") {
				broj_zutih_karata++;
				broj_poena_zuto += ruka[i].get_vrednost();
				if(znak == "+2" || znak == "Block" || znak == "Reverse") {
					broj_zutih_efekata++;
				}
			} else if(boja == "Zelena") {
				broj_zelenih_karata++;
				broj_poena_zeleno += ruka[i].get_vrednost();
				if(znak == "+2" || znak == "Block" || znak == "Reverse") {
					broj_zelenih_efekata++;
				}
			} else if(boja == "Plava") {
				broj_plavih_karata++;
				broj_poena_plavo += ruka[i].get_vrednost();
				if(znak == "+2" || znak == "Block" || znak == "Reverse") {
					broj_plavih_efekata++;
				}
			}
		}
		
		std::string preferirana_boja_broj_karata = preferirana_boja(broj_crvenih_karata, broj_zutih_karata, broj_zelenih_karata, broj_plavih_karata);
		
		std::string preferirana_boja_broj_poena = preferirana_boja(broj_poena_crveno, broj_poena_zuto, broj_poena_zeleno, broj_poena_plavo);
		
		std::string preferirana_boja_broj_efekata = preferirana_boja(broj_crvenih_efekata, broj_zutih_efekata, broj_zelenih_efekata, broj_plavih_efekata);
		
		for(int i = 0; i < broj_karata_u_ruci; i++) {
			if(ruka[i].get_boja() == "Crna" || ruka[i] == karta_na_talonu) {
				validne_karte.push_back(ruka[i]);
			}
		}
		
		kar::Karta karta_koju_izbacuje = kar::Karta();
		
		for(int i = 0; i < (int)ruka.size(); i++) {
			if(ruka[i].get_boja() == "Crna") {
				karta_koju_izbacuje.set_kar(ruka[i]);
				break;
			}
			if(ruka[i] == karta_na_talonu) {
				karta_koju_izbacuje.set_kar(ruka[i]);
				break;
			}
		}
		
		if(min <= 2 || broj_karata_u_ruci <= 2) {
			karta_koju_izbacuje.set_kar(odigraj_bezbedno(validne_karte));
			odabrana_boja = preferirana_boja_broj_efekata;
		} else if(min <= 4) {
			karta_koju_izbacuje.set_kar(odigraj_umereno(validne_karte));
			odabrana_boja = preferirana_boja_broj_poena;
		} else {
			karta_koju_izbacuje.set_kar(odigraj_lezerno(validne_karte));
			odabrana_boja = preferirana_boja_broj_karata;
		}
		
		izbaci(karta_koju_izbacuje);
		
		return karta_koju_izbacuje;
	}
	
	std::string preferirana_boja(int broj_crvena, int broj_zuta, int broj_zelena, int broj_plava) {
		std::string rezultat = "Crvena";
		int max = broj_crvena;
		if(broj_crvena < broj_zuta) {
			rezultat = "Zuta";
			max = broj_zuta;
		}
		if(max < broj_zelena) {
			rezultat = "Zelena";
			max = broj_zelena;
		}
		if(max < broj_plava) {
			rezultat = "Plava";
			max = broj_plava;
		}
		return rezultat;
	}
	
	kar::Karta odigraj_bezbedno(std::vector<kar::Karta> &validne_karte) {
		kar::Karta karta_koju_izbacuje = kar::Karta();
		
		std::vector<kar::Karta> brojevi;
		std::vector<kar::Karta> znaci;
		std::vector<kar::Karta> plus_cetiri;
		std::vector<kar::Karta> dzokeri;
		
		for(int i = 0; i < (int)validne_karte.size(); i++) {
			if(validne_karte[i].get_znak() == "+2") {
				znaci.push_back(validne_karte[i]);
			} else if(validne_karte[i].get_znak() == "Block") {
				znaci.push_back(validne_karte[i]);
			} else if(validne_karte[i].get_znak() == "Reverse") {
				znaci.push_back(validne_karte[i]);
			} else if(validne_karte[i].get_znak() == "+4") {
				plus_cetiri.push_back(validne_karte[i]);
			} else if(validne_karte[i].get_znak() == "Joker") {
				dzokeri.push_back(validne_karte[i]);
			} else {
				brojevi.push_back(validne_karte[i]);
			}
		}
		
		if(!plus_cetiri.empty()) {
			sortiraj_karte_opadajuce(plus_cetiri);
			karta_koju_izbacuje = plus_cetiri[0];
		} else if(!dzokeri.empty()) {
			sortiraj_karte_opadajuce(dzokeri);
			karta_koju_izbacuje = dzokeri[0];
		} else if(!znaci.empty()) {
			sortiraj_karte_opadajuce(znaci);
			karta_koju_izbacuje = znaci[0];
		} else if(!brojevi.empty()) {
			sortiraj_karte_opadajuce(brojevi);
			karta_koju_izbacuje = brojevi[0];
		} else {
			std::cout << "GRESKA!!!\n";
			exit(EXIT_FAILURE);
		}
		
		return karta_koju_izbacuje;
	}
	
	kar::Karta odigraj_umereno(std::vector<kar::Karta> &validne_karte) {
		kar::Karta karta_koju_izbacuje = kar::Karta();
		
		std::vector<kar::Karta> brojevi;
		std::vector<kar::Karta> znaci;
		std::vector<kar::Karta> plus_cetiri;
		std::vector<kar::Karta> dzokeri;
		
		for(int i = 0; i < (int)validne_karte.size(); i++) {
			if(validne_karte[i].get_znak() == "+2") {
				znaci.push_back(validne_karte[i]);
			} else if(validne_karte[i].get_znak() == "Block") {
				znaci.push_back(validne_karte[i]);
			} else if(validne_karte[i].get_znak() == "Reverse") {
				znaci.push_back(validne_karte[i]);
			} else if(validne_karte[i].get_znak() == "+4") {
				plus_cetiri.push_back(validne_karte[i]);
			} else if(validne_karte[i].get_znak() == "Joker") {
				dzokeri.push_back(validne_karte[i]);
			} else {
				brojevi.push_back(validne_karte[i]);
			}
		}
		
		if(!znaci.empty()) {
			sortiraj_karte_opadajuce(znaci);
			karta_koju_izbacuje = znaci[0];
		} else if(!brojevi.empty()) {
			sortiraj_karte_opadajuce(brojevi);
			karta_koju_izbacuje = brojevi[0];
		} else if(!plus_cetiri.empty()) {
			sortiraj_karte_opadajuce(plus_cetiri);
			karta_koju_izbacuje = plus_cetiri[0];
		} else if(!dzokeri.empty()) {
			sortiraj_karte_opadajuce(dzokeri);
			karta_koju_izbacuje = dzokeri[0];
		} else {
			std::cout << "GRESKA!!!\n";
			exit(EXIT_FAILURE);
		}
		
		return karta_koju_izbacuje;
	}
	
	kar::Karta odigraj_lezerno(std::vector<kar::Karta> &validne_karte) {
		kar::Karta karta_koju_izbacuje = kar::Karta();
		
		std::vector<kar::Karta> brojevi;
		std::vector<kar::Karta> znaci;
		std::vector<kar::Karta> plus_cetiri;
		std::vector<kar::Karta> dzokeri;
		
		for(int i = 0; i < (int)validne_karte.size(); i++) {
			if(validne_karte[i].get_znak() == "+2") {
				znaci.push_back(validne_karte[i]);
			} else if(validne_karte[i].get_znak() == "Block") {
				znaci.push_back(validne_karte[i]);
			} else if(validne_karte[i].get_znak() == "Reverse") {
				znaci.push_back(validne_karte[i]);
			} else if(validne_karte[i].get_znak() == "+4") {
				plus_cetiri.push_back(validne_karte[i]);
			} else if(validne_karte[i].get_znak() == "Joker") {
				dzokeri.push_back(validne_karte[i]);
			} else {
				brojevi.push_back(validne_karte[i]);
			}
		}
		
		if(!brojevi.empty()) {
			sortiraj_karte_opadajuce(brojevi);
			karta_koju_izbacuje = brojevi[0];
		} else if(!znaci.empty()) {
			sortiraj_karte_opadajuce(znaci);
			karta_koju_izbacuje = znaci[0];
		} else if(!plus_cetiri.empty()) {
			sortiraj_karte_opadajuce(plus_cetiri);
			karta_koju_izbacuje = plus_cetiri[0];
		} else if(!dzokeri.empty()) {
			sortiraj_karte_opadajuce(dzokeri);
			karta_koju_izbacuje = dzokeri[0];
		} else {
			std::cout << "GRESKA!!!\n";
			exit(EXIT_FAILURE);
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
	
	void sortiraj_karte(std::vector<kar::Karta> &niz_karata) {
		std::sort(niz_karata.begin(), niz_karata.end(), kar::compare);
	}
	
	void sortiraj_karte_opadajuce(std::vector<kar::Karta> &niz_karata) {
		std::sort(niz_karata.begin(), niz_karata.end(), kar::compare);
		std::reverse(niz_karata.begin(), niz_karata.end());
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
