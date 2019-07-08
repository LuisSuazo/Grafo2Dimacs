#ifndef Arista_HPP
#define Arista_HPP
#include <iostream>
#include <cmath>

struct Arista{
    uint id;
    unsigned int inicio;
    unsigned int fin;
    std::string peso;

    Arista(uint id,unsigned int i,unsigned int f,std::string y):
        id(id),
        inicio(i),
        fin(f),
        peso(y)
        {}

    Arista & operator=(const Arista & rhs) {
		if (this == &rhs) return *this;		
		this-> id= rhs.id;
		this->inicio=rhs.inicio;
		this->fin=rhs.fin;
        this->peso=rhs.peso;
		return *this;
	};
};


bool Comparador(const Arista* n1, const Arista* n2){
    if (n1->inicio < n2->inicio) {
        return true;
    }else{
        return n1->fin < n2->fin;
    }

}


#endif