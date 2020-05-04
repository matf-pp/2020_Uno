#ifndef IGRA_H
#define IGRA_H

#include "spil.h"
#include "igrac.h"
#include <vector>
#include <iostream>

namespace igra{

class Igra
{
public:
	Igra()
	{
		_spil_za_igru=*(new spil::Spil());
		for(int i=0;i<4;i++)
		{
			Ig::Igrac* ig=new Ig::Igrac();
			_igraci.push_back(*ig);			

		}
		_spil_za_igru.promesaj();
		for(int i=0;i<6;i++)
			for(int j=0;j<4;j++)
			{
				std::cout<<"igrac "<<j<<" je vukao\n";
				kar::Karta tmp(_spil_za_igru.izvuci_kartu());
				tmp.toString();				
				_igraci[j].vuci(tmp);

			}
		_spil_za_izbacivanje.push_back(_spil_za_igru.izvuci_kartu());	
		std::cout<<"\n";
		ispis_stanja();	
	}	




	bool provera_bacanja(const kar::Karta k)
	{
		if(k._boja=="Crna" || (_spil_za_izbacivanje.end()-1)->_boja=="Crna")
			return true;
		if(k==*(_spil_za_izbacivanje.end()-1))
			return true;
		return false;

	}






	bool baci_kartu(int i)
	{
		if(_spil_za_igru.broj_karata_u_spilu()==0)
		{
			kar::Karta tmp(_spil_za_igru.set_spil(_spil_za_izbacivanje));
			_spil_za_izbacivanje.push_back(tmp);

		}
		switch(i){
			case 0:{
				kar::Karta tmp(_igraci[i].izbaci(0));
				int j=0;				
				while(!provera_bacanja(tmp) && j<_igraci[i].broj_karata())
				{
					
					_igraci[i].vuci(tmp);
					tmp.set_kar(_igraci[i].izbaci(0));
					j++;
				}	

				if(j==_igraci[i].broj_karata() && j!=0)
				{
					
					tmp.set_kar(_spil_za_igru.izvuci_kartu());
					if(provera_bacanja(tmp)){
						_spil_za_izbacivanje.push_back(tmp);
						std::cout<<"igrac izbacivanje\n";		
					}
					else
					{
						_igraci[i].vuci(tmp);				
						std::cout<<"igrac "<<i<<" je vukao\n";
					}				
				}
				else 
				{
					_spil_za_izbacivanje.push_back(tmp);
					std::cout<<"igrac "<<i<<" je bacio "<<"\n";
					(_spil_za_izbacivanje.end()-1)->toString();				
				}

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
					_igraci[i].set_broj_poena(poeni);
					
					std::cout<<"igrac "<<i<<" je pobedio\n";
					return true;		
				}
				return false;				
			}
	
			case 1:{
				_igraci[i].sortiraj_ruku();
				kar::Karta tmp(_igraci[i].izbaci(0));
				int j=0;			
				while(!provera_bacanja(tmp) && j<_igraci[i].broj_karata())
				{
					
					_igraci[i].vuci(tmp);
					tmp.set_kar(_igraci[i].izbaci(0));
					j++;
				}	
				if(j==_igraci[i].broj_karata() && j!=0)
				{
					
					tmp.set_kar(_spil_za_igru.izvuci_kartu());
					if(provera_bacanja(tmp)){
						_spil_za_izbacivanje.push_back(tmp);
						std::cout<<"igrac izbacivanje\n";		
					}
					else
					{
						_igraci[i].vuci(tmp);				
						std::cout<<"igrac "<<i<<" je vukao\n";
					}
				}			
				else
				{
					_spil_za_izbacivanje.push_back(tmp);
					std::cout<<"igrac "<<i<<" je bacio "<<"\n";
					(_spil_za_izbacivanje.end()-1)->toString();				
				}
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
					_igraci[i].set_broj_poena(poeni);
				
					std::cout<<"igrac "<<i<<" je pobedio\n";
					return true;		
				}
				return false;
			}
			case 2:{
				
				_igraci[i].sortiraj_obrnuto();
				kar::Karta tmp(_igraci[i].izbaci(0));
				int j=0;			
				while(!provera_bacanja(tmp) && j<_igraci[i].broj_karata())
				{
										
					_igraci[i].vuci(tmp);
					tmp.set_kar(_igraci[i].izbaci(0));
					j++;
				}	


				if(j==_igraci[i].broj_karata() && j!=0)
				{
					
					tmp.set_kar(_spil_za_igru.izvuci_kartu());
					if(provera_bacanja(tmp)){
						_spil_za_izbacivanje.push_back(tmp);
						std::cout<<"igrac izbacivanje\n";		
					}
					else
					{
						_igraci[i].vuci(tmp);				
						std::cout<<"igrac "<<i<<" je vukao\n";
					}				
				}			
				else
				{
					_spil_za_izbacivanje.push_back(tmp);
					std::cout<<"igrac "<<i<<" je bacio "<<"\n";
					(_spil_za_izbacivanje.end()-1)->toString();				
				}


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
					_igraci[i].set_broj_poena(poeni);
										
					std::cout<<"igrac "<<i<<" je pobedio\n"<<_igraci[i].broj_karata()<<"\n";
					return true;		
				}
				return false;

			}
			case 3:{
				kar::Karta tmp(_igraci[i].izbaci(0));
				int j=0;			
				while(!provera_bacanja(tmp) && j<_igraci[i].broj_karata())
				{
					_igraci[i].vuci(tmp);
					tmp.set_kar(_igraci[i].izbaci(0));
					j++;
				}	
				
				if(j==_igraci[i].broj_karata() && j!=0)
				{
					
					tmp.set_kar(_spil_za_igru.izvuci_kartu());
					if(provera_bacanja(tmp)){
						_spil_za_izbacivanje.push_back(tmp);
						std::cout<<"igrac izbacivanje\n";		
					}
					else
					{
						_igraci[i].vuci(tmp);				
						std::cout<<"igrac "<<i<<" je vukao\n";
					}
				}
				else 
				{
					_spil_za_izbacivanje.push_back(tmp);
					std::cout<<"igrac "<<i<<" je bacio "<<"\n";
					(_spil_za_izbacivanje.end()-1)->toString();				

				}

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
					_igraci[i].set_broj_poena(poeni);
				
					std::cout<<"\nigrac "<<i<<" je pobedio\n";
					return true;		
				}
				return false;
			}
			default: std::cout<<"\nnesto nije uredu\n";
			return false;
		}


	}





	void ispis_stanja()
	{
		for(int i=0;i<4;i++)
		{
			std::cout<<i<<" igrac:\n\n";	
			_igraci[i].ispis_ruke();
			std::cout<<"\n";		
		}	
	}








	void ispis_spila_za_izbacivanje()
	{
		std::vector<kar::Karta>::iterator it;
		for(it=_spil_za_izbacivanje.begin();it<_spil_za_izbacivanje.end();it++)
		{
			it->toString();	
		}
	}
	






private:
	std::vector<Ig::Igrac> _igraci;
	spil::Spil _spil_za_igru;
	std::vector<kar::Karta> _spil_za_izbacivanje;

};




}

#endif
