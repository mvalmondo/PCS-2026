#include <iostream>
#include <cmath>
using namespace std;

int main() 
{  
	static const int N=10;
	double arr[N]={ 3.3, 5.6, 7.8, 44.8, 32.9, 74.3, 66.9, 23.7, 46.9, 99.9};
	double a=arr[0];
	for (int i=1; i<N; i=i+1) {
	    a=min(a,arr[i]);
	}
	cout<<a<< "\n" ;
	
	double b=arr[0];
	for (int i=1; i<N; ++ i) {
		b=max(b,arr[i]);
	}
	cout<<b<<"\n"; 
	
	
	double somma=0;
	for (int j=0; j<N; j=j+1) {
		somma= somma+arr[j];
	}
	
	double media=somma/N;
	
	cout <<media <<"\n";
	
	
	
	double somma_quadrata=0;
	for (int k=0; k<N; k=k+1) {
		somma_quadrata= somma_quadrata + (arr[k]-media)*(arr[k]-media);
	}
	double dev_stand= sqrt(somma_quadrata/N);
	cout<<dev_stand<<"\n";
	
	
	return 0;
	
	
}
	