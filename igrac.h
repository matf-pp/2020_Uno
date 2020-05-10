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

	//Metod koji postavlja datu kartu u ruku igraca
	void vuci(kar::Karta k) {
		ruka.push_back(k);
	}
	
	//Metod koji izbacuje datu kartu iz ruke igraca
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
	
	/* Metod koji odredjuje koju kartu igrac izbacuje kada je red na njega da igra.
	 * Ona sadrzi analizu podataka koje su igracu na raspolaganju, a to su:
	 * -koliko ostali igraci imaju karata u rukama;
	 * -koje karte sam igrac ima u ruci.
	 * Igrac bira strategiju koja odgovara situaciji, i izbacuje odgovarajucu kartu.
	 */
	kar::Karta izbaci_kartu(const kar::Karta &karta_na_talonu, const std::vector<int> &broj_karata_ostalih_igraca, std::string &odabrana_boja) {
		/* Odredjuje se broj karata u ruci onog igraca koji ih trenutno ima najmanje.
		 * Ova informacija okvirno govori koliko je partija blizu zavrsetku, i to igrac razmatra.
		 */
		int min = broj_karata_ostalih_igraca[0];
		for(int i = 1; i < (int)broj_karata_ostalih_igraca.size(); i++) {
			if(broj_karata_ostalih_igraca[i] < min) {
				min = broj_karata_ostalih_igraca[i];
			}
		}
		
		/* Igrac analizira karte koje ima u ruci po razlicitim faktorima:
		 * -broj karata u svakoj boji;
		 * -broj poena u svakoj boji;
		 * -broj karata sa efektom u svakoj boji;
		 */
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
		
		/* Igrac analizira koja boja mu najvise odgovara da bude na talonu.
		 * On razmatra:
		 * -broj karata u svakoj boji;
		 * -broj poena u svakoj boji;
		 * -broj karata sa efektom u svakoj boji.
		 * Za nesto od ovoga ce se odluciti da mu je najvaznije.
		 */

		std::string preferirana_boja_broj_karata = preferirana_boja(broj_crvenih_karata, broj_zutih_karata, broj_zelenih_karata, broj_plavih_karata);

		std::string preferirana_boja_broj_poena = preferirana_boja(broj_poena_crveno, broj_poena_zuto, broj_poena_zeleno, broj_poena_plavo);

		std::string preferirana_boja_broj_efekata = preferirana_boja(broj_crvenih_efekata, broj_zutih_efekata, broj_zelenih_efekata, broj_plavih_efekata);
		
		//Odredjuju se sve karte koje igrac trenutno ima u ruci, a moze da odigra.
		for(int i = 0; i < broj_karata_u_ruci; i++) {
			if(ruka[i].get_boja() == "Crna" || ruka[i] == karta_na_talonu) {
				validne_karte.push_back(ruka[i]);
			}
		}
		
		/* Na osnovu toga koliko je partija blizu kraja, igrac se odlucuje koju kartu izbacuje.
		 * Takodje, on po istoj osnovi bira koja boja mu najvise odgovara.
		 * Ova informacija se prosledjuje kako bi igrac, ako je odigrao +4 ili dzokera,
		 * mogao da promeni boju na talonu u bas tu koja mu najvise odgovara.
		 */
		kar::Karta karta_koju_izbacuje = kar::Karta();
		
		if(min <= 2 || broj_karata_u_ruci <= 2) {
			karta_koju_izbacuje.set_karta(odigraj_bezbedno(validne_karte));
			odabrana_boja = preferirana_boja_broj_efekata;
		} else if(min <= 4) {
			karta_koju_izbacuje.set_karta(odigraj_umereno(validne_karte));
			odabrana_boja = preferirana_boja_broj_poena;
		} else {
			karta_koju_izbacuje.set_karta(odigraj_lezerno(validne_karte));
			odabrana_boja = preferirana_boja_broj_karata;
		}
		
		izbaci(karta_koju_izbacuje);

		return karta_koju_izbacuje;
	}
	
	//Pomocni metod koji na osnovu datih informacija o bojama odredjuje preferiranu boju igraca
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
	
	/* Jedan od metoda koji govori sta igrac treba da odigra, u slucaju da igra bezbedno
	 * Ako igra bezbedno, igrac zeli da igra karte u sledecem redosledu:
	 * +4 -> dzokeri -> efekti -> brojevi
	 * Objasnjenje: ako je situacija napeta, igrac bi voleo da minimizuje broj poena u ruci.
	 */
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
			std::cout << "GRESKA!\n";
			exit(EXIT_FAILURE);
		}

		return karta_koju_izbacuje;
	}
	
	/* Jedan od metoda koji govori sta igrac treba da odigra, u slucaju da igra umereno
	 * Ako igra umereno, igrac zeli da igra karte u sledecem redosledu:
	 * efekti -> brojevi -> +4 -> dzokeri
	 * Objasnjenje: ako je situacija relativno opustena, ali ne skroz, igrac bi trebalo da 
	 * polako krene da izbacuje karte sa efektom, ali nece zuriti sa +4 ili dzokerom.
	 */
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
	
	/* Jedan od metoda koji govori sta igrac treba da odigra, u slucaju da igra lezerno
	 * Ako igra lezerno, igrac zeli da igra karte u sledecem redosledu:
	 * brojevi -> efekti -> +4 -> dzokeri
	 * Objasnjenje: ako je situacija potpuno opustena, igrac nije pod pritiskom da izbacuje
	 * karte sa efektom, vec ce prvo izbacivati brojeve.
	 */
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

	//Pomocni metod koji sortira niz karata opadajuce po njihovoj vrednosti
	void sortiraj_karte_opadajuce(std::vector<kar::Karta> &niz_karata) {
		std::sort(niz_karata.begin(), niz_karata.end(), kar::compare);
		std::reverse(niz_karata.begin(), niz_karata.end());
	}
	
	///Metod odredjuje da li igrac ima da izbaci, odn. odigra neku kartu
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
	
	//Getteri:
	int get_broj_poena() {
		return _broj_poena;
	}
	
	std::string get_ime() {
		return _ime;
	}
	
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

bool compare(Ig::Igrac igrac1, Ig::Igrac igrac2) {
	return igrac1.get_broj_poena() < igrac2.get_broj_poena();
}

}

#endif
