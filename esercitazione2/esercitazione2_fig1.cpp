
#include <iostream>
int main() 
{
  double ad[4] = {0.0, 1.1, 2.2, 3.3};
  float af[8] = {0.0, 1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7}; 
  int ai[3] = {0, 1, 2};
  int x = 1;
  float y = 1.1;
  (&y)[1] = 0;                           //prende l'indirizzo della varabile y e poi si sposta in avanti di un elemento [seocndo la formula: base+i*sizeof(T)] e ci sovrascrive 0 
  std::cout << x << "\n";    
  std::cout << &ad[2] << "\n";           //questo comando stampa l'address dell'elemento in posizione 2 dell'array ad[] cioè 2.2 
  
   std::cout << &x << "\n";
   
   //OSSERVAZIONE: dalla rappresentazione grafica il blocchetto ad[2]=2.2 e x=0 (disegnando i vettori in maniera continua uno dopo l'altro E POI VARIBILE FLOAT Y E POI VAR INT X) risultano distanti 44byte, MA da terminale differiscono di 64

  
  return 0; 
}


// cosa succede: ho rappresentato i vettori uno dopo l'altro e dopo gli array ho disegnato y e poi x (come due blocchetti da 4 byte ciascuno); a causa del comando (&y)[1] = 0; il valore di x viene sostituito con 0; quindi dopo la sostituzione i due elementi distano 64byte 