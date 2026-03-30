
#include "header_es3.hpp"       //essendo mio header_es3.hpp un mio file uso le virgolette
#include <iostream>             //per le librerie standard uso le parentesi angolate
#include <ostream>

int main() {
	rational<int> r_inf(5,0);
	rational<int> r_nan(0,0);
	rational<int> r_normale(1,2);
	rational<int> r(3,6);
	rational<int> r1(-56, 8);
	rational<int> r2(44,0);
	rational<int> r3(35, -5);
	rational<int> a(10,2);
	rational<int> b(1,4);
	
	
	rational<int> somma1=a+b;
	rational<int> somma2= r1+r3;
	rational<int> sottraz1= a-b;
	rational<int> sottraz2= r1-r3;
	rational<int> prodotto1= a*b;
	rational<int> prodotto2= r1*r3;
	rational<int> divisione1= a/b;
	rational<int> divisione2= r1/r3;
	
	
	
	std::cout << r_inf << "\n";
	std::cout << r_nan << "\n";
	std::cout << r_normale<< "\n";
	std::cout <<r<<"\n";
	std::cout <<r1<<"\n";
	std::cout <<r2<<"\n";
	std::cout <<r3<<"\n";
	std::cout<<somma1<<"\n";
	std::cout<<somma2<<"\n";
	std::cout<<prodotto1<<"\n";
	std::cout<<prodotto2<<"\n";
	std::cout<<sottraz1<<"\n";
	std::cout<<sottraz2<<"\n";
	std::cout<<divisione1<<"\n";
	std::cout<<divisione2<<"\n";

	return 0; 
}


