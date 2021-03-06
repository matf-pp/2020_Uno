#ifndef IGRA_H
#define IGRA_H

#include "igrac.h"
#include "spil.h"
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <iterator>

namespace igra {

/*IDEJA:
 * Klasa Igra se bavi simulacijama celog meca/igre.
 * Ovo se realizuje uzastopnim simulacijama partija.
 * U svakoj partiji pobedjuje onaj igrac koji je izbacio sve karte iz ruke.
 * Na kraju svake partije se pobedniku dodeljuju poeni koji se racunaju prema datim pravilima.
 * Kada igrac premasi 500 poena, on je pobednik cele igre, i simulacija se zavrsava
 */
class Igra {
public:
	//Konstruktor
	Igra(std::vector<std::string> &imena_igraca) {
		//Postavljamo inicijalni broj partije, kao i smer igranja
		_redni_broj_partije = 0;
		_smer_igranja = 1;
		
		//Postavljamo indikatore za kontrolu toka programa
		_interakcije_preskocene = false;
		_partija_preskocena = false;
		_partija_preskocena_kompletno = false;
		
		//Obradjujemo podatke o igracima
		_broj_igraca = imena_igraca.size();
		for(int i = 0; i < _broj_igraca; i++) {
			Ig::Igrac igrac = Ig::Igrac(imena_igraca[i]);
			_igraci.push_back(igrac);
		}
		
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
			
			if(!_interakcije_preskocene && !igra_zavrsena) {
				cekaj_unos();
			}
		}
		
		ispisi_zavrsnu_poruku(_igraci);
	}

	//Metoda kojom se pokrece jedna partija
	void odigraj_partiju() {
		pripremi_sledecu_partiju();
		
		//_log<<_spil_za_igru.toString();
		//ispisi_spil_za_izbacivanje();
		
		ispisi_kartu_na_talonu();
		
		bool partija_zavrsena = false;
		while(!partija_zavrsena) {
			//ispisi_spil_za_izbacivanje();		
			//ispisi_stanje();
			
			//Provera da li je spil prazan
			proveri_spil();
			
			//_log << "\n";
			if(odigraj_potez(_indeks_igraca_na_potezu)) {
				//ispisi_spil_za_izbacivanje();
				
				ostringstream buffer;
				buffer << "\n";
				buffer << "---------------------------------------------------------------\n";
				buffer << "                          KRAJ PARTIJE                         \n";
				buffer << "---------------------------------------------------------------\n";
				
				if(_interakcije_preskocene) {
					_log << buffer.str();
				} else {
					std::cout << buffer.str();
				}
				
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
		
		if(_interakcije_preskocene || _partija_preskocena) {
			buffer << "\n";
		}
		
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
		
		if(_interakcije_preskocene) {
			_log << buffer.str();
		} else {
			std::cout << buffer.str();
		}
		
		
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
			
			ispisi_stanje();
			ispisi_pobednika_partije(indeks, poeni);
			ispisi_rezultat_partije();
			
			return true;		
		}
		
		ispisi_sledeceg_igraca();
		ispisi_kartu_na_talonu();
		
		//Ukoliko nije preskoceno, od korisnika se ocekuje unos sa standardnog ulaza.
		if(!_interakcije_preskocene && !_partija_preskocena) {
			char c = getchar();
			if(c != '\n') {
				char r = getchar();
				if(r != '\n') {
					std::cout << "Greska: posle svakog poteza treba uneti ili novi red ili karakter 'q'\n";
					exit(EXIT_FAILURE);
				}
				
				if(c == 'q') {
					std::cout << "Unesen karakter 'q'. Ostatak partije se automatski ispisuje.\n";
					_partija_preskocena = true;
				} else {
					std::cout << "Greska: posle svakog poteza treba uneti ili novi red ili karakter 'q'\n";
					exit(EXIT_FAILURE);
				}
			}
		}
		
		return false;
	}
	
	//Metoda provera da li data karta moze da se baci na spil za izbacivanje
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
		//Uvecavamo redni broj partije
		_redni_broj_partije++;
		
		//Zapocinjemo novi log
		_log.str("");
		
		ostringstream buffer;
		buffer << "---------------------------------------------------------------\n";
		buffer << "                           ";
		buffer << "PARTIJA " << _redni_broj_partije;
		buffer << "                           ";
		buffer << "\n";
		buffer << "---------------------------------------------------------------\n";
		
		//Podesavamo indikatore za kontrolu toka programa
		_partija_preskocena = false;
		if(_partija_preskocena_kompletno) {
			_partija_preskocena = true;
		}
		
		if(_interakcije_preskocene) {
			_log << buffer.str();
		} else {
			std::cout << buffer.str();
		}
		
		//Pocinjemo od prvog igraca u nizu
		_indeks_igraca_na_potezu = 0;
		
		//Smer u kom se igra se resetuje
		_smer_igranja = 1;
		
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
		
		//Vucemo prvu kartu iz (sada promesanog) spila i postavljamo je na talon
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
		std::string znak = odigrana_karta.get_znak();
		
		std::string poruka = "";
		ostringstream buffer;
		
		if(znak == "Reverse") {
			_smer_igranja *= -1;
			poruka = "Redosled igranja se obrce!\n";
		} else if(znak == "Block") {
			odredi_sledeceg_igraca(0);
			buffer << _igraci[_indeks_igraca_na_potezu].toString() << " preskace potez.\n";
			poruka = buffer.str();
		} else if(znak == "+2") {
			odredi_sledeceg_igraca(0);
			dodeli_karte_igracu_na_potezu(2);
		} else if(znak == "Joker") {
			buffer << "Nova boja: ";
			buffer << nova_boja;
			buffer << "\n";
			poruka = buffer.str();
			
			_karta_na_talonu.set_boja(nova_boja);
		} else if(znak == "+4") {
			buffer << "Nova boja: ";
			buffer << nova_boja;
			buffer << "\n";
			poruka = buffer.str();
			
			_karta_na_talonu.set_boja(nova_boja);
			
			odredi_sledeceg_igraca(0);
			dodeli_karte_igracu_na_potezu(4);
		} else {
			//odredi_sledeceg_igraca(0);
		}
		odredi_sledeceg_igraca(0);
		
		if(!poruka.empty()) {
			if(_interakcije_preskocene) {
				_log << poruka;
			} else {
				std::cout << poruka;
			}
		}
		//_log << "Sledeci igrac: " << _igraci[_indeks_igraca_na_potezu].toString() << "\n";
	}
	
	//Metod koji simulira efekte izvlacenja karata kod "+2" i "+4" karata.
	void dodeli_karte_igracu_na_potezu(int broj_karata) {
		ostringstream buffer, buffer_karte;
		
		//buffer_karte << "[ ";
		for(int i = 0; i < broj_karata; i++) {
			proveri_spil();
			kar::Karta tmp(_spil_za_igru.izvuci_kartu());
			_igraci[_indeks_igraca_na_potezu].vuci(tmp);
			//_log << _igraci[_indeks_igraca_na_potezu].toString() << " vuce: " << tmp.toString();
			buffer_karte << " " << tmp.toString();
		}
		//buffer_karte << "]";
		buffer << _igraci[_indeks_igraca_na_potezu].toString() << " vuce karte:";
		buffer << buffer_karte.str();
		buffer << " i preskace potez.\n";
		//buffer << "\n";
		
		if(_interakcije_preskocene) {
			_log << buffer.str();
		} else {
			std::cout << buffer.str();
		}
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
		
		if(_interakcije_preskocene) {
			_log << buffer.str();
		} else {
			std::cout << buffer.str();
		}
	}

	//Metod koji u log upisuje podatke o spilu za izbacivanje karata
	void ispisi_spil_za_izbacivanje() {
		ostringstream buffer;
		
		buffer << "\n";
		buffer << "Spil za bacanje:\n";
		
		spil::Spil* tmp = new spil::Spil(_spil_za_izbacivanje);
		buffer << tmp->toString() << "\n";
		
		if(_interakcije_preskocene) {
			_log << buffer.str();
		} else {
			std::cout << buffer.str();
		}
	}
	
	void ispisi_sledeceg_igraca() {
		ostringstream buffer;
		
		//buffer << "\n";
		buffer << "Sledeci igrac: ";
		buffer << _igraci[_indeks_igraca_na_potezu].toString(); 
		buffer << "\n";
		
		if(_interakcije_preskocene) {
			_log << buffer.str();
		} else {
			std::cout << buffer.str();
		}
	}
	
	void ispisi_kartu_na_talonu() {
		ostringstream buffer;
		
		buffer << "\n";
		buffer << "Karta na talonu: ";
		buffer << _karta_na_talonu.toString();
		buffer << "\n";
		
		if(_interakcije_preskocene) {
			_log << buffer.str();
		} else {
			std::cout << buffer.str();
		}
	}
	
	void ispisi_pobednika_partije(int indeks_pobednika, int broj_osvojenih_poena) {
		ostringstream buffer;
		
		buffer << "\n*Pobednik ";
		buffer << _redni_broj_partije;
		buffer << ". partije: ";	
		buffer << _igraci[indeks_pobednika].get_ime();
		buffer << "!\n*Osvojeno poena: ";
		buffer << broj_osvojenih_poena;
		buffer << "\n";
		
		if(_interakcije_preskocene) {
			_log << buffer.str();
		} else {
			std::cout << buffer.str();
		}
	}
	
	//Metod koji u log upisuje trenutne rezultate igraca
	void ispisi_rezultat_partije() {
		ostringstream buffer;
		
		buffer << "\n";
		buffer << "Ukupni poeni:\n";
		
		std::vector<Ig::Igrac>::iterator igrac;
		for(igrac = _igraci.begin(); igrac != _igraci.end(); igrac++) {
			buffer << igrac->toString();
			buffer << " ";
			buffer << igrac->get_broj_poena();
			buffer << "\n";
		}
		
		if(_interakcije_preskocene) {
			_log << buffer.str();
		} else {
			std::cout << buffer.str();
		}
	}
	
	//Metod koji ispisuje zavrsnu poruku na kraju programa
	void ispisi_zavrsnu_poruku(std::vector<Ig::Igrac> igraci) {
		ostringstream buffer;
		std::sort(igraci.begin(), igraci.end(), Ig::compare);
		
		buffer << "\n";
		buffer << "***************************************************************\n";
		buffer << "*                          REZULTATI                          *\n";
		buffer << "***************************************************************\n";
		buffer << "*                                                             *\n";
		buffer << "* Nasi takmicari su ostvarili sledece rezultate:              *\n";
		
		std::sort(igraci.begin(), igraci.end(), Ig::compare);
		std::reverse(igraci.begin(), igraci.end());
		
		std::vector<Ig::Igrac>::iterator igrac;
		std::string razmaci;
		int broj_razmaka;
		for(igrac = igraci.begin(); igrac != igraci.end(); igrac++) {
			buffer << "* ";
			buffer << igrac->get_ime();
			buffer << " ";
			buffer << to_string(igrac->get_broj_poena());
			
			broj_razmaka = 59 - igrac->get_ime().length() - to_string(igrac->get_broj_poena()).length();
			razmaci = std::string(broj_razmaka, ' ');
			buffer << razmaci;
			buffer << "*";
			buffer << "\n";
		}
		
		buffer << "*                                                             *\n";
		buffer << "* Pobednik turnira je: ";
		buffer << igraci[0].get_ime();
		buffer << "! Cestitamo!";
		broj_razmaka = 27 - igraci[0].get_ime().length();
		razmaci = std::string(broj_razmaka, ' ');
		buffer << razmaci;
		buffer << "*\n";
		buffer << "*                                                             *\n";
		buffer << "***************************************************************\n";
		
		std::cout << buffer.str();
	}
	
	/*Metod koji od korisnika ocekuje da unese opciju koja odredjuje kako se nadalje izvrsava program
	 */
	void cekaj_unos() {
		ostringstream buffer;
		
		buffer << "\n";
		buffer << "Unesite karakter:\n";
		buffer << "c - continue opcija\n";
		buffer << "Biranjem ove opcije se naredna partija prati potez po potez.\n";
		buffer << "s - skip opcija\n";
		buffer << "Biranjem ove opcije se naredna partija preskace, odnosno ispisuje bez pracenja poteza.\n";
		buffer << "q - quit opcija\n";
		buffer << "Biranjem ove opcije se naredna i sve sledece partije preskacu, sve do kraja turnira.\n";
		buffer << ">> ";
		std::cout << buffer.str();
		
		std::string unos;
		char uneseni_karakter;
		
		std::cin >> unos;
		uneseni_karakter = unos[0];
		
		bool karakter_validan;
		
		karakter_validan = (uneseni_karakter == 'c');
		karakter_validan = karakter_validan || (uneseni_karakter == 'q');
		karakter_validan = karakter_validan || (uneseni_karakter == 's');
		karakter_validan = karakter_validan && (unos.length() <= 2);
		
		while(!karakter_validan) {
			std::cout << "[!] Molimo, unesite karakter 'c', 's' ili 'q'.\n";
			std::cout << ">> ";
			
			std::cin >> unos;
			uneseni_karakter = unos[0];
			
			karakter_validan = (uneseni_karakter == 'c');
			karakter_validan = karakter_validan || (uneseni_karakter == 'q');
			karakter_validan = karakter_validan || (uneseni_karakter == 's');
			karakter_validan = karakter_validan && (unos.length() <= 2);
		}
		
		if(uneseni_karakter == 'c') {
			_partija_preskocena_kompletno = false;
		} else if(uneseni_karakter == 'q') {
			_interakcije_preskocene = true;
		} else if(uneseni_karakter == 's') {
			_partija_preskocena_kompletno = true;
		}
	}
	
	//Podaci o igracima
	std::vector<Ig::Igrac> _igraci;
	int _broj_igraca;
	
	//Spilovi za izvlacenje i izbacivanje karata
	spil::Spil _spil_za_igru;
	std::vector<kar::Karta> _spil_za_izbacivanje;
	
	//Redni broj koji se uvecava kad god se igra nova partija
	int _redni_broj_partije;
	
	/* U ovaj log se upisuju sve relevantne informacije tokom simulacije igranja partije.
	 * Ideja je da se na kraju partije ovaj log prazni u cout, i tek tada dobijamo kompletan ispis.
	 * Ovo, doduse, vazi jedino ako korisnik nije ubrzao tok izvrsavanja programa na neki nacin.
	 */
	ostringstream _log;
	
	//Indikatori koji sluze da kontrolisu tok programa u zavisnosti od toga sta korisnik unosi
	bool _interakcije_preskocene;
	bool _partija_preskocena;
	bool _partija_preskocena_kompletno;
	
	//Narednim podacima se manipulise pri igranju karata sa specijalnim efektima:
	
	//Indeks igraca koji je trenutno na potezu
	int _indeks_igraca_na_potezu;
	//Znak (1 ili -1) smera u kom se igra u datom trenutku
	int _smer_igranja;
	//Karta na vrhu spila za izbacivanje
	kar::Karta _karta_na_talonu;
};

void ispisi_uvodnu_poruku(std::vector<std::string> &imena_igraca) {
	ostringstream buffer;
	
	buffer << "***************************************************************\n";
	buffer << "*                         UNO! TURNIR                         *\n";
	buffer << "***************************************************************\n";
	buffer << "*                                                             *\n";
	buffer << "* Dobrodosli! Prisustvujete takmicenju izmedju 4 Uno igraca!  *\n";
	buffer << "* Unesite im imena da bismo poceli.                           *\n";
	buffer << "* (ime igraca treba da bude u obliku jedne reci)              *\n";
	buffer << "*                                                             *\n";
	buffer << "***************************************************************\n";
	std::cout << buffer.str();
	
	std::string info;
	for(int i = 0; i < 4; i++) {
		std::cout << ">> " << i+1 << ". " << "igrac: ";
		std::cin >> info;
		
		while(info.length() > 15) {
			std::cout << "[!] Molimo, unesite ime sa najvise 15 karaktera.\n";
			std::cout << ">> " << i+1 << ". " << "igrac: ";
			std::cin >> info;
		}
		
		imena_igraca.push_back(info);
	}
	getchar();
	
	buffer.str("");
	buffer << "***************************************************************\n";
	buffer << "*                                                             *\n";
	buffer << "* Napomena: Tokom jedne partije, svaki igrac ce odigrati svoj *\n";
	buffer << "* potez, nakon cega mozete uneti ENTER, ako zelite da dalje   *\n";
	buffer << "* nastavite, ili karakter 'q' da biste preskocili do kraja    *\n";
	buffer << "* tekuce partije.                                             *\n";
	buffer << "*                                                             *\n";
	buffer << "***************************************************************\n";
	std::cout << buffer.str();
}

}

#endif
