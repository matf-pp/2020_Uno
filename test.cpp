#include "karta.h"
#include "spil.h"
#include "igra.h"
#include <iostream>
#include <vector>
#include <iterator>

using namespace std;

int main() {
	Ig::Igrac igrac1 = Ig::Igrac("Petar");
	Ig::Igrac igrac2 = Ig::Igrac("Dubravka");
	Ig::Igrac igrac3 = Ig::Igrac("Dobrivoje");
	Ig::Igrac igrac4 = Ig::Igrac("Momir");
	
	std::vector<Ig::Igrac> igraci = {igrac1, igrac2, igrac3, igrac4};
	
	igra::Igra partija = igra::Igra(igraci);
	
	//partija.odigraj_partiju();
	partija.odigraj();
	
	return 0;
}
