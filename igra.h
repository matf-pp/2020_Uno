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
		
		ispisi_spil_za_izbacivanje();
		
		bool partija_zavrsena = false;
		while(!partija_zavrsena) {
			//_log << "\n";
			
			//ispisi_spil_za_izbacivanje();
			_log << "\n";
					
			//ispisi_stanje();
			//_log << "\n";
			
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
	bool odigraj_potez(int i) {
		//Provera da li je spil prazan
		if(_spil_za_igru.broj_karata_u_spilu()==0) {
			kar::Karta tmp(_spil_za_igru.set_spil(_spil_za_izbacivanje));
			_spil_za_izbacivanje.push_back(tmp);
		}
		
		//TODO "Strategije"
		if(i == 1) {
			_igraci[i].sortiraj_ruku();
		}
		if(i == 2) {
			_igraci[i].sortiraj_obrnuto();
		}
		
		//Prolazi se kroz ruku u potrazi za upotrebljivom kartom
		kar::Karta tmp(_igraci[i].izbaci(0));
		int j=0;				
		while(!provera_bacanja(tmp) && j<_igraci[i].broj_karata()) {
			_igraci[i].vuci(tmp);
			tmp.set_kar(_igraci[i].izbaci(0));
			j++;
		}

		ostringstream buffer;
		bool bacio_kartu = false;
		/*Obrada: da li je pronadjena upotrebljiva karta?
		 *Ukoliko jeste, igrac je odigra.
		 *Ukoliko nije, igrac vuce.
		 * -ako ni izvucena karta ne moze da se odigra, igrac je zavrsio potez.
		 * -ako moze, igrac je odigra.
		 */
		if(j==_igraci[i].broj_karata() && j!=0) {
			
			tmp.set_kar(_spil_za_igru.izvuci_kartu());				
			buffer << _igraci[i].toString() << " vuce kartu " << tmp.toString();
			
			if(provera_bacanja(tmp)) {
				_spil_za_izbacivanje.push_back(tmp);
				buffer << " i izbacuje je.\n";
				bacio_kartu = true;
			}
			else {
				_igraci[i].vuci(tmp);				
				buffer << ".\n";
			}				
		} else {
			_spil_za_izbacivanje.push_back(tmp);
			
			buffer << _igraci[i].toString() << " baca kartu ";
			buffer << (_spil_za_izbacivanje.end()-1)->toString() << ".\n";
			bacio_kartu = true;				
		}
		
		_log << buffer.str();
		
		if(bacio_kartu == true) {
			//TODO
			//izvrsi_efekat_karte(tmp);
			
			//TODO: Obrisati kad se doda efekat.
			odredi_sledeceg_igraca(0);
			_log << "Sledeci igrac: " << _igraci[_indeks_igraca_na_potezu].toString() << "\n";
		} else {
			odredi_sledeceg_igraca(0);
			_log << "Sledeci igrac: " << _igraci[_indeks_igraca_na_potezu].toString() << "\n";
		}

		//Obrada: ako je igrac izbacio sve karte iz ruke, on je pobedio partiju.
		if(_igraci[i].broj_karata()==0) {
			int poeni=0;
			for(int k=0;k<4;k++) {
				if(k!=i) {
					poeni+=_igraci[k].broj_poena_u_ruci();
				}
			}
			_igraci[i].dodaj_poene(poeni);
			
			_log << "*" << _igraci[i].toString() << " je pobednik u partiji " << _redni_broj_partije << "!\n";
			_log << "*Osvojeno poena: " << poeni << "\n";
			ispisi_stanje();
			
			ispisi_rezultat();
			
			return true;		
		}
		return false;
	}
	
	//Metoda provera da li data karta moze da se baci na spil za izbacivanje
	bool provera_bacanja(const kar::Karta k) {
		if(k.get_boja()=="Crna" || (_spil_za_izbacivanje.end()-1)->get_boja()=="Crna")
			return true;
		if(k==*(_spil_za_izbacivanje.end()-1))
			return true;
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
		//_log << "Igraci na pocetku partije imaju sledece karte:\n";
		ispisi_stanje();
		
		//Praznimo spil za izbacivanje, a potom postavljamo (novu)prvu kartu
		_spil_za_izbacivanje.clear();
		_spil_za_izbacivanje.push_back(_spil_za_igru.izvuci_kartu());
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
	void odredi_sledeceg_igraca(int broj_preskocenih_igraca) {
		_indeks_igraca_na_potezu += _smer_igranja * (broj_preskocenih_igraca + 1);
		_indeks_igraca_na_potezu %= _broj_igraca;
	}
	
	//Metod koji izvrsava efekat karte manipulacijom indeksa trenutnog igraca, kao i smera igranja
	void izvrsi_efekat_karte(const kar::Karta &odigrana_karta) {
		//std::string boja = odigrana_karta.get_boja();
		std::string znak = odigrana_karta.get_znak();
		if(znak == "Reverse") {
			_smer_igranja *= -1;
			odredi_sledeceg_igraca(0);
		} else if(znak == "Block") {
			odredi_sledeceg_igraca(1);
		} else if(znak == "+2") {
			//TODO
			odredi_sledeceg_igraca(1);
		} else if(znak == "Joker") {
			//TODO
			odredi_sledeceg_igraca(0);
		} else if(znak == "+4") {
			odredi_sledeceg_igraca(1);
			//TODO
		}
		else {
			odredi_sledeceg_igraca(0);
		}
		_log << "Sledeci igrac: " << _igraci[_indeks_igraca_na_potezu].toString() << "\n";
	}

private:
	//Metod koji upisuje u log podatke o kartama u rukama svih igraca
	void ispisi_stanje() {
		ostringstream buffer;
		
		buffer << "\n";
		buffer << "Karte igraca:\n";
		
		for(int i = 0; i < 4; i++) {
			buffer << _igraci[i].toString() << ": ";
			buffer << _igraci[i].ispis_ruke();
			buffer << "\n";		
		}
		
		_log << buffer.str();
	}

	//Metod koji upisuje u log podatke o spilu za izbacivanje karata
	void ispisi_spil_za_izbacivanje() {
		ostringstream buffer;
		
		buffer << "\n";
		buffer << "Spil za bacanje:\n";
		
		spil::Spil* tmp = new spil::Spil(_spil_za_izbacivanje);
		buffer << tmp->toString() << "\n";
		
		_log << buffer.str();
	}
	
	//Metod koji "ispisuje" trenutne rezultate igraca u log
	void ispisi_rezultat() {
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
	
	/*U ovaj log se ispisuju sve relevantne informacije tokom simulacije igranja partije.
	 *Ideja je da se na kraju svake partije ovaj log "prazni" u cout, i tek tada dobijamo kompletan ispis.
	 *Vrlo korisno za debagovanje.
	 */
	ostringstream _log;
	
	/*Indeks igraca koji je trenutno na potezu kao i znak (1 ili -1) smera u kom se igra u datom trenutku.
	 *Ovim podacima se manipulise pri igranju karata sa specijalnim efektima.
	 */
	int _indeks_igraca_na_potezu;
	int _smer_igranja;
};

}

#endif
