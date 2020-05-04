#ifndef KARTA_H
#define KARTA_H

#include <string>
#include <cctype>
#include <cstdlib>
#include<iostream>
namespace kar{


class Karta{
public:
	Karta()
	{
		_boja=std::string("");
		_znak=std::string("");
		_vrednost=0;
	}
	Karta(std::string boja,std::string znak)
	{
		_boja=boja;
		_znak=znak;
		_vrednost=Vrednost(); 

	}	

	Karta(const Karta &k)
	{	
		_boja=k._boja;
		_znak=k._znak;
		_vrednost=k._vrednost;

	}
	int Vrednost()
	{
		if(isdigit(_znak[0]))
			return atoi(&_znak[0]);
		else return 20;

	}
	void set_karta(const std::string boja,const std::string znak)
	{
		_boja=boja;
		_znak=znak;
		_vrednost=Vrednost();	
	}
	void set_kar( Karta k)
	{
		_boja=k._boja;
		_znak=k._znak;
		_vrednost=k._vrednost;	
	}
	void toString()
	{
		std::cout<<_boja<<" "<<_znak<<"\n" ;
	}
	
	std::string get_boja()
	{
		return _boja;	
	}

	std::string get_znak()
	{
		return _znak;	
	}
	bool operator == (const Karta &k) const{
		if(_boja==k._boja || _znak==k._znak || _boja==std::string("Crna") || k._boja==std::string("Crna"))
			return true;
		else return false;
	}

	std::string _boja;
	std::string _znak;
	int _vrednost;
private:
	

};

bool compare(Karta k1 ,Karta k2)
{
	return k1.Vrednost()<k2.Vrednost();
}



}

#endif
