#ifndef KARTA_H
#define KARTA_H

#include <bits/stdc++.h>

#include <string>
#include <cctype>
#include <cstdlib>
#include <iostream>
#include <sstream>

using namespace std;

namespace kar {

class Karta {
public:
	//Konstruktori
	Karta() {
		_boja=std::string("");
		_znak=std::string("");
		_vrednost=0;
	}
	
	Karta(std::string boja,std::string znak) {
		_boja=boja;
		_znak=znak;
		_vrednost=Vrednost();
	}

	Karta(const Karta &k) {	
		_boja=k._boja;
		_znak=k._znak;
		_vrednost=k._vrednost;

	}
	
	//Pomocna funkcija za odredjivanje vrednosti karte unutar konstruktora.
	int Vrednost() {
		if(isdigit(_znak[0])) {
			return atoi(&_znak[0]);
		}
		if(_boja == "Crna") {
			return 50;
		}
		return 20;
	}
	
	//Getteri
	int get_vrednost() const {
		return _vrednost;
	}
	
	std::string get_boja() const {
		return _boja;
	}

	std::string get_znak() const {
		return _znak;	
	}
	
	//Setteri
	void set_karta(const std::string boja,const std::string znak) {
		_boja=boja;
		_znak=znak;
		_vrednost=Vrednost();	
	}
	
	void set_kar( Karta k) {
		_boja=k._boja;
		_znak=k._znak;
		_vrednost=k._vrednost;	
	}
	
	void set_boja(const std::string &boja) {
		_boja = boja;
	}
	
	//toString
	const string toString() const {
		ostringstream buffer;
		buffer << "[" << _boja << " " << _znak << "]";
		return buffer.str();
	}
	
	//Operator za poredjenje karata
	bool operator == (const Karta &k) const {
		if(_boja==k._boja || _znak==k._znak || _boja==std::string("Crna") || k._boja==std::string("Crna")) {
			return true;
		}
		return false;
	}
private:
	std::string _boja;
	std::string _znak;
	int _vrednost;
};

//Funkcija za poredjenje karata (po njihovoj vrednosi)
bool compare(Karta k1 ,Karta k2) {
	return k1.Vrednost()<k2.Vrednost();
}

}

#endif
