#include <iostream>
#include <eigen3/Eigen/Dense>

Eigen::VectorXd gradiente_coniugato(const Eigen::MatrixXd& A, const Eigen::VectorXd& b, const Eigen::VectorXd& x0, const double res_tol, const unsigned int it_max, unsigned int& it)
{
Eigen::VectorXd x=x0;
Eigen::VectorXd res=b-A*x;
Eigen::VectorXd p=res; //p_0=r_0

double res_norm_0=res.norm();
it=0;

while (it<it_max && res.norm() > res_tol*res_norm_0) {
	const double alpha_k=((res.transpose()*res) / (p.transpose()*A*p)).value();
	x=x+alpha_k*p; //aggionriamo la soluzione
	
	Eigen::VectorXd res_old=res; //calcolo il vecchio residuo prima di calcolare il r_{k+1} mi serve poi per il calcolo di beta
	
	res=res-alpha_k*A*p; //aggiorno il residuo
	
	const double beta_k=((res.transpose()*res)/(res_old.transpose()*res_old)).value();
	//aggiorno la direzione di discesa
	
	p=res+beta_k*p; //nello pseudocodice il residuo è definito come r_k+1=b - Ax_k+1. Poiche nell'algortmo ho implementato l'aggiornametno del residuo come r_k+1=r_k - alpha_k*A*p, i segni dei vettori residui risultano invertiti. per preservare l'ortogonalità delle direzioni di discesa rispetto al prodotto scalare, è necessario mettere il segno positivo
	
	it++;
}
return x;
}

