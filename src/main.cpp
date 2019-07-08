#include <fstream>
#include <string>
#include <boost/container/vector.hpp>
#include <sstream>
#include <boost/unordered_map.hpp>
#include <boost/container/map.hpp>
#include <boost/container/set.hpp>
#include <boost/unordered_set.hpp>
#include <cstdlib>
#include <ctime>
#include "Nodo.hpp"
#include "Arista.hpp"

using namespace std;

int main(int argc,char **argv){
    boost::container::map<uint,Nodo*> idNodo;
    boost::unordered_set<Nodo*,NodoHasher,NodoEqual> Nodos;
    boost::container::map<uint,Arista*> idArista;
    boost::unordered_map<uint,string> idPoligono;
    boost::unordered_map<uint,boost::container::set<uint>> PoligonoNodos;
    boost::unordered_map<uint,boost::container::set<uint>> PoligonoAristas;
    //std::vector<Arista*> Aristas;
    boost::container::map<uint,boost::container::map<uint,string>> Aristas;
    ifstream fe(argv[1]);
    if(!fe.good()){
        cout<<"Error"<<endl;
        exit(-1);
    }
    uint id=1;
    uint contArista=1;
    bool entrar=false;
  
    while( !fe.eof() ){
        string cadena;
        fe>> cadena;
        
        boost::container::vector<string> datos;
        datos.reserve(16);
        istringstream isstream(cadena);
        string palabra;
        /*
        0 xInicio
        1 yInicio
        2 xFin
        3 yFin
        4 long tramo
        6 sentido
        9 id poligono
        10 nombre poligono
        */
        if(entrar){
            while(std::getline(isstream, palabra,';')){
                datos.emplace_back(palabra);
            }
            //cout<<datos.size()<<endl;
            //cout<<cadena<<endl;
            if(datos.size()==0 ) break;
            
            std::string xInicio=datos.at(0);
            std::string yInicio=datos.at(1);
            std::string xFin=datos.at(2);
            std::string yFin=datos.at(3);
            std::string longTramo=datos.at(4);
            int sentido=stoi(datos.at(6));
            uint idComuna=stoi(datos.at(9));
            string nombrePoligono=datos.at(10);
            Nodo *Inicio=new Nodo(id,xInicio,yInicio);
            auto findInicio=Nodos.find(Inicio);
            if(findInicio==Nodos.end()){
                idNodo.emplace(id,Inicio);
                Nodos.emplace(Inicio);
                id++;
            }else{
                Inicio=*(findInicio);
            }
            Nodo *Fin=new Nodo(id,xFin,yFin);
            auto findFin=Nodos.find(Fin);
            if(findFin==Nodos.end()){
                idNodo.emplace(id,Fin);
                Nodos.emplace(Fin);
                id++;
            }else{
                Fin=*(findFin);
            }
            
            
            if(PoligonoAristas.find(idComuna)==PoligonoAristas.end()){
                boost::container::set<uint> aux;
                idPoligono.emplace(idComuna,nombrePoligono);
                PoligonoAristas.emplace(idComuna,aux);
            }
            if(PoligonoNodos.find(idComuna)==PoligonoNodos.end()){
                boost::container::set<uint> aux;
                PoligonoNodos.emplace(idComuna,aux);
            }
            PoligonoNodos.at(idComuna).emplace(Inicio->id);
            PoligonoNodos.at(idComuna).emplace(Fin->id);
            if(sentido==-1){
                Arista *AFin=new Arista(contArista,Fin->id,Inicio->id,longTramo);
                contArista++;
                idArista.emplace(AFin->id,AFin);
                PoligonoAristas.at(idComuna).emplace(AFin->id);
            }else if(sentido==1){
                Arista *AInicio=new Arista(contArista,Inicio->id,Fin->id,longTramo);
                contArista++;
                idArista.emplace(AInicio->id,AInicio);
                PoligonoAristas.at(idComuna).emplace(AInicio->id);
            }else{
                Arista *AInicio=new Arista(contArista,Inicio->id,Fin->id,longTramo);
                contArista++;
                Arista *AFin=new Arista(contArista,Fin->id,Inicio->id,longTramo);
                contArista++;
                idArista.emplace(AInicio->id,AInicio);
                idArista.emplace(AFin->id,AFin);
                PoligonoAristas.at(idComuna).emplace(AInicio->id);
                PoligonoAristas.at(idComuna).emplace(AFin->id);
            }

        }else{
            entrar=true;
        }
    }

    //Escribe Nodos
    ofstream os;
    os.open (argv[5]+string("Stgo.co"));
    for(auto it=idNodo.begin();it!=idNodo.end();++it){
        os<<it->second->id<<" "<<stoi(it->second->x)<<" "<<stoi(it->second->y)<<endl;
    }
    os.close();

    //Escribe Aristas

    os.open (argv[5]+string("Stgo_dist.gr"));
    os<<idNodo.size()<<" "<<idArista.size()<<endl;
    for(auto it=idArista.begin();it!=idArista.end();++it){
        double_t algo=stod(it->second->peso)*1000;
        os<<it->second->inicio<<" "<<it->second->fin<<" "<<floor(algo)<<endl;
    }
    os.close();

    
    //Escribe Nodos xComuna
    //cout<<"xComuna"<<endl;
    boost::unordered_map<uint,uint> newOld;
    for(auto it = PoligonoNodos.begin();it != PoligonoNodos.end();++it){
        os.open(argv[2]+idPoligono.at(it->first)+".co");
        id=1;
        //cout<<"hola"<<endl;
        for(auto kt:it->second){
            Nodo *aux=idNodo.at(kt);
            if(newOld.find(aux->id)==newOld.end()){
                newOld.emplace(aux->id,id);
                id++;
            }
            os<<newOld.at(aux->id)<<" "<<stoi(aux->x)<<" "<<stoi(aux->y)<<endl;
            
        }
        ofstream osa;
        osa.open (argv[3]+idPoligono.at(it->first)+"_dist.gr");
        osa<<newOld.size()<<" "<<PoligonoAristas.at(it->first).size()<<endl;
        for(auto qt : PoligonoAristas.at(it->first)){
            Arista *auxArista=idArista.at(qt);
            double_t algo=stod(auxArista->peso)*1000;
            osa<<newOld.at(auxArista->inicio)<<" "<<newOld.at(auxArista->fin)<<" "<<floor(algo)<<endl;
        }
        osa.close();
        os.close();
        newOld.clear();
    }

    

    

    //Escribe DataSet

   /* boost::unordered_set<std::pair<uint,uint>> Datos;
    for(auto it = PoligonoNodos.begin();it != PoligonoNodos.end();++it){
        ofstream os;
        os.open (argv[4]+idPoligono.at(it->first)+".dat");
        uint cont=0;
        while(cont<100){
            uint start = rand() % it->second.size();
            uint goal = rand() % it->second.size();
            pair<uint,uint> StG=std::make_pair(start,goal);
            if(start != goal && Datos.find(StG)==Datos.end()){
                Datos.emplace(StG);
                os<<it->second.at(start)->id<<" "<<it->second.at(goal)->id<<endl;
                cont++;
            }
        } 
    }*/
    cout<<"termino"<<endl;
}