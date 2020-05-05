#ifndef IGRA_H
#define IGRA_H

#include "spil.h"
#include "igrac.h"
#include <vector>
#include <iostream>
#include <sstream>
#include <string>

namespace igra{

class Igra {
public:
	//Konstruktor
	Igra(int redni_broj) {
		_redni_broj = redni_broj;
		_log << "\n**********Partija " << _redni_broj << "**********\n\n";
	
		_spil_za_igru=*(new spil::Spil());
		for(int i=0;i<4;i++) {
			Ig::Igrac* ig=new Ig::Igrac();
			_igraci.push_back(*ig);			
		}
		
		_spil_za_igru.promesaj();
		for(int i=0;i<6;i++)
			for(int j=0;j<4;j++) {
				ostringstream buffer;
				
				buffer << "Igrac " << j << " je vukao: ";
				kar::Karta tmp(_spil_za_igru.izvuci_kartu());
				buffer << tmp.toString() << "\n";
				_igraci[j].vuci(tmp);
				
				//_log << buffer.str();
			}
		_spil_za_izbacivanje.push_back(_spil_za_igru.izvuci_kartu());
		
		_log << "Igraci na pocetku partije imaju sledece karte:\n";
		_log << ispis_stanja();
	}

	//Glavna metoda kojom se pokrece jedna partija
	void odigraj() {
		_log << "\n";
		_log << ispis_spila_za_izbacivanje();
		
		bool partija_zavrsena = false;
		while(!partija_zavrsena) {
			_log << "\n";
			
			//_log << ispis_spila_za_izbacivanje();
			//_log << "\n";
					
			//_log << ispis_stanja();
			//_log << "\n";
			
			
			for(int i=0;i<4;i++) {
				if (odigraj_potez(i)) {
					_log << "\n" << ispis_spila_za_izbacivanje();
					_log << "\n**********Kraj partije**********\n";
					partija_zavrsena = true;
					break;
				}
			}
		}
		
		std::cout << _log.str();
	}

	//Provera da li data karta moze da se baci na spil za izbacivanje
	bool provera_bacanja(const kar::Karta k) {
		if(k.get_boja()=="Crna" || (_spil_za_izbacivanje.end()-1)->get_boja()=="Crna")
			return true;
		if(k==*(_spil_za_izbacivanje.end()-1))
			return true;
		return false;
	}

	//Metoda koja simulira potez jednog igraca
	bool odigraj_potez(int i) {
		//Provera da li je spil prazan
		if(_spil_za_igru.broj_karata_u_spilu()==0) {
			kar::Karta tmp(_spil_za_igru.set_spil(_spil_za_izbacivanje));
			_spil_za_izbacivanje.push_back(tmp);
		}
		
		//"Strategije"
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
		
		/*Obrada: da li je pronadjena upotrebljiva karta?
		 *Ukoliko jeste, igrac je odigra.
		 *Ukoliko nije, igrac vuce.
		 * -ako ni izvucena karta ne moze da se odigra, igrac je zavrsio potez.
		 * -ako moze, igrac je odigra.
		 */
		if(j==_igraci[i].broj_karata() && j!=0) {
			
			tmp.set_kar(_spil_za_igru.izvuci_kartu());				
			buffer << "Igrac " << i << " je vukao kartu " << tmp.toString();
			
			if(provera_bacanja(tmp)) {
				_spil_za_izbacivanje.push_back(tmp);
				buffer << " i izbacio je.\n";		
			}
			else {
				_igraci[i].vuci(tmp);				
				buffer << ".\n";
			}				
		}
		else
		{
			_spil_za_izbacivanje.push_back(tmp);
			buffer << "Igrac " << i << " je bacio kartu ";
			buffer << (_spil_za_izbacivanje.end()-1)->toString() << ".\n";				
		}
		
		_log << buffer.str();

		//Obrada: ako je igrac izbacio sve karte iz ruke, on je pobedio partiju.
		if(_igraci[i].broj_karata()==0)
		{
			int poeni=0;
			for(int k=0;k<4;k++)
			{
				if(k!=i)
				{
					poeni+=_igraci[k].broj_poena_u_ruci();
				}

			}
			_igraci[i].dodaj_poene(poeni);
			
			_log << "Igrac " << i << " je pobedio u partiji " << _redni_broj << "!\n";
			return true;		
		}
		return false;
	}

	//Metod koji "ispisuje" karte u rukama svakog od igraca
	std::string ispis_stanja() {
		ostringstream buffer;
			
		for(int i=0;i<4;i++) {
			buffer << "Igrac " << i << ": ";	
			buffer << _igraci[i].ispis_ruke();
			buffer << "\n";		
		}
		
		return buffer.str();
	}

	//Metod koji "ispisuje" spil za izbacivanje
	std::string ispis_spila_za_izbacivanje() {
		ostringstream buffer;
		
		buffer << "Spil za bacanje:\n";
		spil::Spil* tmp = new spil::Spil(_spil_za_izbacivanje);
		buffer << tmp->toString() << "\n";
		
		return buffer.str();
	}

private:
	std::vector<Ig::Igrac> _igraci;
	spil::Spil _spil_za_igru;
	std::vector<kar::Karta> _spil_za_izbacivanje;
	int _redni_broj;
	ostringstream _log;
};




}

#endif
