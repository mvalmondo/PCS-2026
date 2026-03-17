//per lo sviluppo di questo codice è stato utilizzato l'aiuto dell'intelligenza artificiale (Gemini)


#include <iostream> 
#include <fstream> 
#include <string> 

using namespace std;

int main(int argc, char* argv[]) {
	if (argc< 2) {
		return 1 ;
	}
	
	string filename = argv[1];
	ifstream ifs(filename);
	if (ifs.is_open() ) {
		string location;
		double t1, t2,t3, t4;
		while (ifs>>location>>t1>>t2>>t3>>t4) {
			double media = (t1+t2+t3+t4) / 4.0; 
			cout <<location<< " " <<media<<"\n";
		}
	} else { return 1 ;
	return 0;
}
}

	