#include <iostream>
#include <ctime>
#include <cstdlib>
#include <eigen3/Eigen/Eigen>
#include "gradiente_coniugato.hpp"

int main (int argc, char **argv) {
	//definisco la dimensione e inizio a costruire la matrice simmetrica definita positiva
	const double tol=1.0e-15;
	unsigned int n=3;
	
	Eigen::MatrixXd B=Eigen::MatrixXd::Random(n,n);
	
	if(std::abs(B.determinant())<tol) {
		std::cout<<"Matrice singolare, esco!" << std::endl;
		return -1;
	}
	
	Eigen::MatrixXd A=B.transpose()*B;
	Eigen::VectorXd x_ex=Eigen::VectorXd::Ones(n);
	
	Eigen::VectorXd b=A*x_ex;
	
	Eigen::VectorXd x0=Eigen::VectorXd::Zero(n);
	Eigen::VectorXd res_init=b-A*x0;
	double res_norm_0= res_init.norm();
	
	const unsigned int it_max=10000;
	const double res_tol=1.0e-12; //tolleranza della soluzione
	unsigned int it=0;
	
	
	Eigen::VectorXd x=gradiente_coniugato(A,b,x0,res_tol, it_max, it); //chiamata all'algoritmo
	
	Eigen::VectorXd res_fin=b-A*x;
	double err_rel=(x_ex.norm()==0.0) ? (x-x_ex).norm() : 
                                        (x - x_ex).norm() / x_ex.norm();
	
	
	std::cout.precision(4);
	std::cout<< std::scientific<< "it"<< "/"<< "it_max"<< std::endl;
	std::cout<< std::scientific<< it<< "/"<< it_max<< std::endl;
	std::cout<< std::scientific<< "res"<< "/"<< "res_tol"<< std::endl;
	std::cout<< std::scientific<< res_fin.norm() / res_norm_0<< "/"<< res_tol<< std::endl;
	std::cout<< std::scientific<< "err_rel"<< std::endl;
	std::cout<< std::scientific<< err_rel<< std::endl;
	std::cout<< std::scientific<< "soluzione"<< std::endl;
	std::cout<< std::scientific<< x << std::endl;

	return 0;
}

