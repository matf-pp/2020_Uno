#include "karta.h"
#include "spil.h"
#include "igra.h"
#include <iostream>

using namespace std;

int main() {
	igra::Igra* partija=new igra::Igra(1);
	
	partija->odigraj();

	return 0;
}
