#include "karta.h"
#include "spil.h"
#include "igra.h"
#include <iostream>
int main()
{
	igra::Igra* partija=new igra::Igra();
	while(1)
	{
		std::cout<<"\n";
		std::cout<<"spil za bacanje\n";
		partija->ispis_spila_za_izbacivanje();
		std::cout<<"\n";
		std::cout<<"\n";
		for(int i=0;i<4;i++)
		{
			
			if (partija->baci_kartu(i))		
			{
				std::cout<<"kraj\n\n";
				return 1;
			}			
		}	
	}

}
