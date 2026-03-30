#include <concepts>
#include <iostream>
#include <numeric>


template<typename I> 
requires std::integral<I>
class rational
{
	I num_;
	I den_;
	
public:
	rational() : num_(I{0}), den_(I{1})       //costruttore di default
	{}                                        
	
	
	rational(I pnum, I pden) :num_(pnum), den_(pden) { //costruttore user-defined	
	if (den_!=0) {
		I common=std::gcd(num_,den_);
		num_/= common;
		den_/= common;
		if (den_<0) {
			num_=-num_;
			den_=-den_;
		}
	}
	
	}  
	
	I num() const {return num_;}
	I den() const { return den_;}
	
	
	rational& operator+=(const rational& other) {                      //uso l'implementazione canonica dell'incremento per completare la parte del testo del secondo punto in cui dice "Di conseguenza, per esempio, un qualsiasi rational sommato ad Inf deve dare Inf, mentre un qualsiasi rational sommato a NaN deve dare NaN."
		
		
		if ((num_==0 &&den_==0) || (other.num_==0 && other.den_==0)) {
			num_=0;
			den_=0;
		}
		
		else if (den_==0 || other.den_==0) {
			num_=1;
			den_=0;
		}
		
		else {
			I n= num_ * other.den_ + other.num_ * den_;
			I d=den_ * other.den_;
			I common= std::gcd(n,d); 
			num_=n/common;
			den_=d/common;
		}
		
		return *this;
	}
	
	rational operator+(const rational& other) const{
		rational ret = *this;
		ret+=other;
		return ret;
	}	
		
		
	rational& operator-=(const rational& other) {
		if ((num_==0 && den_==0) || (other.num_==0 && other.den_==0)) {
			num_=0;
			den_=0;
		}
		else if (den_==0 || other.den_==0) {
			num_=1;
			den_=0;
		}
		else {
			I n=num_ *other.den_ - other.num_ *den_;
			I d=den_ * other.den_;
			I common=std::gcd(n,d);
			num_=n/common;
			den_=d/common;
		}
		return *this;
	}
	rational operator-(const rational& other) const{
		rational ret = *this;
		ret -=other;
		return ret;
	}
	
	
	rational& operator*=(const rational& other) {
		if ((num_==0 && den_==0) || (other.num_==0 && other.den_==0)) {
			num_=0;
			den_=0;
			
		}
		 else if (den_==0 || other.den_==0) {
			 num_=1;
			 den_=0;
		}
		else {
			I n= num_*other.num_;
			I d=den_*other.den_;
			I common=std::gcd(n,d);
			num_=n/common;
			den_=d/common;
		}
		return *this;
	}
	
	
	rational operator*(const rational& other) const{
		rational ret = *this;
		ret *= other;
		return ret;
	}
	
	rational& operator/=( const rational& other) {
		if (other.den_==0 && den_!=0) {
			num_=0;
			den_=1;;
			
		}
		else if (other.num_==0 && den_!=0) {
			num_=1;
			den_=0;
		}
		
		else {
			I n=num_*other.den_;
			I d=den_*other.num_;
			I common=std::gcd(n,d);
			num_=n/common;
			den_=d/common;
		}
		return *this;
	}
	rational operator/(const rational& other) const{
		rational ret = *this;
		ret /= other ;
		return ret;
	}
};


template<typename I>
requires std::integral<I>
std::ostream& 
operator<<(std::ostream& os, const rational<I>& r) {            //se denominatore=0 e numeratore !=0 --> infinito; se denominatore=0 e numeratore=0 --> non accettabile
	if (r.den() == 0 && r.num() !=0) {
		os << "Inf"; 
	}
	else if (r.den()==0 && r.num()==0) {
		os << "NaN";
	}
	else {
		os<< r.num();
		if (r.den() != 1) {
			os<< "/" << r.den();
		}
	}
	return os;
}                      