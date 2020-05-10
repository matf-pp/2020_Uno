#include "igra.h"
#include <iostream>
#include <vector>

int main() {
	std::vector<std::string> imena_igraca;
	igra::ispisi_uvodnu_poruku(imena_igraca);

	igra::Igra simulacija = igra::Igra(imena_igraca);
	simulacija.odigraj();
	//simulacija.odigraj_partiju();
	
	return 0;
}
