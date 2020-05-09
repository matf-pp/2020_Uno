#ifndef IGRA_H
#define IGRA_H

#include "spil.h"
#include "igrac.h"
#include <vector>
#include <iostream>
#include <sstream>
#include <string>
#include <iterator>

namespace igra {

class Igra {
public:
	//Konstruktor
	Igra(std::vector<Ig::Igrac> &igraci) {
		//Postavljamo inicijalni broj partije, kao i smer igranja
		_redni_broj_partije = 0;
		_smer_igranja = 1;
		
		//Obradjujemo podatke o igracima
		_igraci = igraci;
		_broj_igraca = igraci.size();
		
		//Pravimo i mesamo spil
		_spil_za_igru=*(new spil::Spil());
		_spil_za_igru.promesaj();
	}
	
	//Glavna metoda kojom se pokrece simulacija citave igre/meca
	void odigraj() {
		bool igra_zavrsena = false;
		
		while(!igra_zavrsena) {
			odigraj_partiju();
			igra_zavrsena = nadjen_pobednik();
		}
	}

	//Metoda kojom se pokrece jedna partija
	void odigraj_partiju() {
		pripremi_sledecu_partiju();
		
		//_log<<_spil_za_igru.toString();
		
		ispisi_spil_za_izbacivanje();
		
		bool partija_zavrsena = false;
		while(!partija_zavrsena) {
			//ispisi_spil_za_izbacivanje();		
			//ispisi_stanje();
			
			//Provera da li je spil prazan
			proveri_spil();
			
			_log << "\n";
			if(odigraj_potez(_indeks_igraca_na_potezu)) {
				ispisi_spil_za_izbacivanje();
				_log << "\n**********Kraj partije**********\n\n";
				partija_zavrsena = true;
				break;
			}
		}
		std::cout << _log.str();
	}

	/*Metoda koja simulira potez jednog igraca - jedna od centralnih metoda celog programa.
	 *IDEJA:
	 *Svaki igrac, kada i ako na njega dodje red da igra, svoj potez odvija u dve etape:
	 *1. IZVLACENJE/IZBACIVANJE KARTE
	 *Igrac odredjuje da li ce da odigra neku kartu (ako ima koju!), ili ce da vuce sa vrha spila.
	 * Ukoliko odigra kartu, ova etapa se zavrsava.
	 * Ukoliko vuce kartu, odredjuje da li ce da izbaci izvucenu kartu (ako je to moguce!) ili nece.
	 *2. EFEKAT KARTE
	 *Ovo mozemo shvatiti kao posledicu poteza igraca na dalji tok partije.
	 * Ako igrac odigra kartu sa brojem, ili ne odigra kartu uopste, nema posledice.
	 * Ako igrac odigra kartu sa specijalnim efektom, posledica je upravo taj efekat.
	 *
	 *Ako je igrac *primoran* da vuce karte (putem +2 ili +4 efekata), to se desava u situacijama
	 *kada on ujedno i preskace svoj potez.
	 *Zato ovo ne racunamo kao etapu igranja poteza.
 	 */
	bool odigraj_potez(int indeks) {
		kar::Karta odigrana_karta = kar::Karta();
		bool igrac_ima_sta_da_odigra = false;
		bool igrac_bacio_kartu = false;
		ostringstream buffer;
		
		std::string preferirana_boja("Zelena");
		
		igrac_ima_sta_da_odigra = _igraci[indeks].ima_sta_da_odigra(_karta_na_talonu);
		if(igrac_ima_sta_da_odigra) {
			std::vector<int> broj_karata_ostalih_igraca;
			for(int i = 0; i < _broj_igraca; i++) {
				if(i != indeks) {
					broj_karata_ostalih_igraca.push_back(_igraci[i].broj_karata());
				}
			}
			
			odigrana_karta = _igraci[indeks].izbaci_kartu(_karta_na_talonu, broj_karata_ostalih_igraca, preferirana_boja);
			
			_spil_za_izbacivanje.push_back(odigrana_karta);
			_karta_na_talonu = odigrana_karta;
			igrac_bacio_kartu = true;
			
			buffer << _igraci[indeks].toString() << " baca kartu ";
			buffer << _karta_na_talonu.toString() << ".\n";
		} else {
			kar::Karta izvucena_karta = kar::Karta(_spil_za_igru.izvuci_kartu());
			buffer << _igraci[indeks].toString() << " vuce kartu " << izvucena_karta.toString();
			
			if(bacanje_dozvoljeno(izvucena_karta)) {
				_spil_za_izbacivanje.push_back(izvucena_karta);
				_karta_na_talonu = izvucena_karta;
				
				odigrana_karta = izvucena_karta;
				
				buffer << " i izbacuje je.\n";
				igrac_bacio_kartu = true;
			} else {
				_igraci[indeks].vuci(izvucena_karta);
				buffer << ".\n";
			}
		}
		_log << buffer.str();
		
		if(igrac_bacio_kartu) {
			izvrsi_efekat_karte(odigrana_karta, preferirana_boja);
		} else {
			odredi_sledeceg_igraca(0);
		}

		//Obrada: ako je igrac izbacio sve karte iz ruke, on je pobedio partiju.
		if(_igraci[indeks].broj_karata() == 0) {
			int poeni = 0;
			std::vector<Ig::Igrac>::iterator igrac;
			
			for(igrac = _igraci.begin(); igrac != _igraci.end(); igrac++) {
				poeni += igrac->broj_poena_u_ruci();
			}
			_igraci[indeks].dodaj_poene(poeni);
			
			ispisi_pobednika_partije(indeks, poeni);
			ispisi_stanje();
			ispisi_rezultat_partije();
			
			return true;		
		}
		
		ispisi_sledeceg_igraca();
		ispisi_kartu_na_talonu();
		
		return false;
	}
	
	//Metoda proverava da li data karta moze da se baci na spil za izbacivanje
	bool bacanje_dozvoljeno(const kar::Karta &karta) {
		if(karta.get_boja() == "Crna") {
			return true;
		}
		if(karta == _karta_na_talonu) {
			return true;
		}
		return false;
	}
	
	//Metoda koja sluzi da na pocetku partije resetuje i uredi podatke u klasi
	void pripremi_sledecu_partiju() {
		//Uvecavamo redni broj partije i zapocinjemo novi log
		_redni_broj_partije++;
		_log.str("");
		_log << "************Partija " << _redni_broj_partije << "***********\n";
		
		//Pocinjemo od prvog igraca u nizu
		//TODO: Napraviti tako da se pocinje od onog koji je pobedio u prethodnoj partiji
		_indeks_igraca_na_potezu = 0;
		
		//Postavljamo novi, promesan spil
		_spil_za_igru = *(new spil::Spil());
		_spil_za_igru.promesaj();
		
		std::vector<Ig::Igrac>::iterator igrac;
		
		//Uzimamo karte koje igraci imaju u rukama
		for(igrac = _igraci.begin(); igrac != _igraci.end(); igrac++) {
			igrac->izbaci_sve_karte();
		}
		
		//Delimo karte igracima
		for(int i = 0; i < 6; i++) {
			for(igrac = _igraci.begin(); igrac != _igraci.end(); igrac++) {
				ostringstream buffer;
				buffer << igrac->toString() << " vuce: ";
				
				kar::Karta tmp(_spil_za_igru.izvuci_kartu());
				buffer << tmp.toString() << "\n";
				igrac->vuci(tmp);
				
				//_log << buffer.str();
			}
		}
		
		//Ispisujemo stanje na pocetku partije
		ispisi_stanje();
		
		//Praznimo spil za izbacivanje
		_spil_za_izbacivanje.clear();
		
		//Vucemo prvu kartu iz (promesanog) spila i postavljamo je na talon
		kar::Karta tmp(_spil_za_igru.izvuci_kartu());
		_spil_za_izbacivanje.push_back(tmp);
		_karta_na_talonu = tmp;
		
	}
	
	//Metod koji proverava da li je nadjen igrac koji je pobedio u celokupnoj igri/mecu
	bool nadjen_pobednik() {
		std::vector<Ig::Igrac>::iterator igrac;
		for(igrac = _igraci.begin(); igrac != _igraci.end(); igrac++) {
			if((igrac->get_broj_poena()) > 500) {
				return true;
			}
		}
		return false;
	}
	
	/*Metod modifikuje indeks igraca koji je na potezu u zavisnosti od toga koliko igraca preskacemo.
	 *Prosledjuje se 0 ili 1 i na osnovu toga se odredjuje da li igra naredni igrac u krugu, ili se
	 *preskace i igra onaj posle njega. U obzir se uzima smer u kome igraci igraju u datom trenutku.
	 */
	void odredi_sledeceg_igraca(const int &broj_preskocenih_igraca) {
		_indeks_igraca_na_potezu += _smer_igranja * (broj_preskocenih_igraca + 1);
		_indeks_igraca_na_potezu += _broj_igraca;
		_indeks_igraca_na_potezu %= _broj_igraca;
	}
	
	//Metod koji izvrsava efekat karte manipulacijom indeksa trenutnog igraca, kao i smera igranja
	void izvrsi_efekat_karte(const kar::Karta &odigrana_karta, const std::string &nova_boja) {
		//std::string boja = odigrana_karta.get_boja();
		std::string znak = odigrana_karta.get_znak();
		if(znak == "Reverse") {
			_smer_igranja *= -1;
		} else if(znak == "Block") {
			odredi_sledeceg_igraca(0);
			_log << _igraci[_indeks_igraca_na_potezu].toString() << " preskace potez.\n";
		} else if(znak == "+2") {
			odredi_sledeceg_igraca(0);
			dodeli_karte_igracu_na_potezu(2);
		} else if(znak == "Joker") {
			//TODO: Dodati da menja boju
			_log << "Nova boja: ";
			_log << nova_boja;
			_log << "\n";
			_karta_na_talonu.set_boja(nova_boja);
		} else if(znak == "+4") {
			//TODO: Dodati da menja boju
			_log << "Nova boja: ";
			_log << nova_boja;
			_log << "\n";
			_karta_na_talonu.set_boja(nova_boja);
			odredi_sledeceg_igraca(0);
			dodeli_karte_igracu_na_potezu(4);
		} else {
			//odredi_sledeceg_igraca(0);
		}
		odredi_sledeceg_igraca(0);
		//_log << "Sledeci igrac: " << _igraci[_indeks_igraca_na_potezu].toString() << "\n";
	}
	
	//Metod koji simulira efekte izvlacenja karata kod "+2" i "+4" karata.
	void dodeli_karte_igracu_na_potezu(int broj_karata) {
		ostringstream buffer, buffer_karte;
		
		buffer_karte << "[ ";
		for(int i = 0; i < broj_karata; i++) {
			kar::Karta tmp(_spil_za_igru.izvuci_kartu());
			_igraci[_indeks_igraca_na_potezu].vuci(tmp);
			//_log << _igraci[_indeks_igraca_na_potezu].toString() << " vuce: " << tmp.toString();
			buffer_karte << tmp.toString() << " ";
		}
		buffer_karte << "]";
		buffer << _igraci[_indeks_igraca_na_potezu].toString() << " vuce karte: ";
		buffer << buffer_karte.str();
		buffer << " i preskace potez.\n";
		
		_log << buffer.str();
	}
	
	//Metod proverava da li je spil prazan i, ako jeste, vraca sve karte sa talona u spil i mesa ga
	void proveri_spil() {
		if(_spil_za_igru.broj_karata_u_spilu() == 0) {
			kar::Karta tmp(_spil_za_igru.set_spil(_spil_za_izbacivanje));
			_spil_za_izbacivanje.push_back(tmp);
		}
	}

private:
	//Metod koji u log upisuje podatke o kartama u rukama svih igraca
	void ispisi_stanje() {
		ostringstream buffer;
		
		buffer << "\n";
		buffer << "Karte igraca:\n";
		
		std::vector<Ig::Igrac>::iterator igrac;
		for(igrac = _igraci.begin(); igrac != _igraci.end(); igrac++) {
			buffer << igrac->toString() << ": ";
			buffer << igrac->ispis_ruke();
			buffer << "\n";
		}
		
		_log << buffer.str();
	}

	//Metod koji u log upisuje podatke o spilu za izbacivanje karata
	void ispisi_spil_za_izbacivanje() {
		ostringstream buffer;
		
		buffer << "\n";
		buffer << "Spil za bacanje:\n";
		
		spil::Spil* tmp = new spil::Spil(_spil_za_izbacivanje);
		buffer << tmp->toString() << "\n";
		
		_log << buffer.str();
	}
	
	void ispisi_sledeceg_igraca() {
		ostringstream buffer;
		
		//buffer << "\n";
		buffer << "Sledeci igrac: ";
		buffer << _igraci[_indeks_igraca_na_potezu].toString(); 
		buffer << "\n";
		
		_log << buffer.str();
	}
	
	void ispisi_kartu_na_talonu() {
		ostringstream buffer;
		
		buffer << "\n";
		buffer << "Karta na talonu: ";
		buffer << _karta_na_talonu.toString();
		buffer << "\n";
		
		_log << buffer.str();
	}
	
	void ispisi_pobednika_partije(int indeks_pobednika, int broj_osvojenih_poena) {
		ostringstream buffer;
		
		buffer << "\n*Pobednik ";
		buffer << _redni_broj_partije;
		buffer << ". partije: ";	
		buffer << _igraci[indeks_pobednika].toString();
		buffer << "!\n*Osvojeno poena: ";
		buffer << broj_osvojenih_poena;
		buffer << "\n";
		
		_log << buffer.str();
	}
	
	//Metod koji u log upisuje trenutne rezultate igraca
	void ispisi_rezultat_partije() {
		ostringstream buffer;
		
		buffer << "\n";
		buffer << "Ukupni brojevi poena:\n";
		
		std::vector<Ig::Igrac>::iterator igrac;
		for(igrac = _igraci.begin(); igrac != _igraci.end(); igrac++) {
			buffer << igrac->toString();
			buffer << " ";
			buffer << igrac->get_broj_poena();
			buffer << "\n";
		}
		
		_log << buffer.str();
	}
	
	//Podaci o igracima
	std::vector<Ig::Igrac> _igraci;
	int _broj_igraca;
	
	//Spilovi za izvlacenje i izbacivanje karata
	spil::Spil _spil_za_igru;
	std::vector<kar::Karta> _spil_za_izbacivanje;
	
	//Redni broj koji se uvecava kad god se igra nova partija
	int _redni_broj_partije;
	
	/*U ovaj log se upisuju sve relevantne informacije tokom simulacije igranja partije.
	 *Ideja je da se na kraju svake partije ovaj log prazni u cout, i tek tada dobijamo kompletan ispis.
	 */
	ostringstream _log;
	
	//Narednim podacima se manipulise pri igranju karata sa specijalnim efektima:
	
	//Indeks igraca koji je trenutno na potezu
	int _indeks_igraca_na_potezu;
	//Znak (1 ili -1) smera u kom se igra u datom trenutku
	int _smer_igranja;
	//Karta na vrhu spila za izbacivanje
	kar::Karta _karta_na_talonu;
};

}

#endif
