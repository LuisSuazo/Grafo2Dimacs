#ifndef Nodo_HPP
#define Nodo_HPP
#include <iostream>
#include <cmath>
#include <boost/functional/hash.hpp>

struct Nodo{
		unsigned int id;
    std::string x;
    std::string y;

    Nodo(unsigned int id,std::string x,std::string y):
        id(id),
        x(x),
        y(y)
        {}

    Nodo & operator=(const Nodo & rhs) {
		if (this == &rhs) return *this;		
		this-> id= rhs.id;
		this->x=rhs.x;
		this->y=rhs.y;
		return *this;
	};

    std::size_t hash_value(const Nodo &a){
		std::size_t seed=0;
		boost::hash_combine(seed,boost::hash_value(a.x));
		boost::hash_combine(seed,boost::hash_value(a.y));
		
		return seed;
	}

    bool operator==(const Nodo *rhs) const {
		return rhs->id == this->id;
	};

};

struct NodoHasher{
	std::size_t operator()(const Nodo* rhs) const{
		std::size_t seed=0;
		boost::hash_combine(seed,boost::hash_value(rhs->x));
		boost::hash_combine(seed,boost::hash_value(rhs->y));
		return seed;
  }
};


struct NodoEqual{
	bool operator() (Nodo const* t1, Nodo const* t2) const{
    	return t1->x == t2->x && t1->y == t2->y;
  	}
};

#endif