#include <iostream>
#include <fstream>  //Manejo de archivos
#include <string>
#include <vector>   //Libreria para hacer uso de algo parecido a un ArrayList
#include <regex>
#include "AFN.h"
#include "Transicion.h"
#include <functional>
//#include <boost/range/numeric.hpp>  // (Opcion 1)Cabecera para boost::accumulate que da un mejor manejo en los datos de un vector o arreglo
#include <numeric>  //Opcion numero 2
using namespace std;
fstream file;

AFN::AFN(){

}

void AFN::carga(string nombre){
    //ofstream file; //Puede que sirva :L
    file.open(nombre, ios::out);  //Cargamos l archivo, si no existe se crea un archivo
    if(file.fail()){
        std::cout << "No se pudo abrir el archivo\n";
        exit(1);
    }
    file.close();
}

void AFN::cargaTransiciones(string nombre){
    try{
        string linea;   //Almacenamos la cadena que se escribe en el archivo
        int inicio, fin;
        const char *c;

        file.open(nombre.c_str());
        if(file.fail()){
            std::cout << "No se pudo abrir el archivo\n";
            exit(1);
        }
        //linea = file.getline();
        getline(file, linea);
        std::regex expresion("(\\d+)->(\\d+),([a-zE])");    //Declaramos expresion
        std::cmatch expresionMatch;
        while(!(linea.empty())){
            if(std::regex_search(linea, expresionMatch, expresion)){ //Creo que puede ir mejor en un if :L
                inicio = stoi(expresionMatch[1]);
                fin = stoi(expresionMatch[2]);
                c = expresionMatch[3].first;
                addTransicion(inicio, fin, c);  //Agremos una transicion al archivo
            }
        }
        file.close();
    }catch(const std::exception& e){

    }
}

void AFN::addTransicion(int inicio, int fin, const char *simbolo){
    //Transicion *transicion = new Transicion(inicio, fin, simbolo);   //new??
    Transicion transicion(inicio, fin, *simbolo);
    transiciones.push_back(transicion);
}

void AFN::rmTransicion(int inicio, int fin, const char *simbolo){
    Transicion trans(inicio, fin, *simbolo);
    for(Transicion transicion : this -> transiciones){
        if(transicion.equals(trans)){
            //transiciones.pop_back(transicion);
            transiciones.erase(transicion);
        }
    }
}

void AFN::obtenerInicial(string nombre){
    try{
        string linea;

        file.open(nombre.c_str());
        if(file.fail()){
            std::cout << "No se pudo abrir el archivo\n";
            exit(1);
        }
        //linea = file.getline();
        getline(file, linea);
        std::regex expresion("([a-zA-z]+)(\\d+)");
        std::cmatch expresionMatch;
        std::regex_search(linea, expresionMatch, expresion);
        this -> incial = stoi(expresionMatch[2]);
        file.close();
    }catch(const std::exception& e){

    }
}

void AFN::obtenerFinal(string nombre){
    try{
        string linea;
        int indice = 0;
        string aux[20];
        int nums[20];

        file.open(nombre.c_str());
        if(file.fail()){
            std::cout << "No se pudo abrir el archivo\n";
            exit(1);
        }
        //getline(file, linea);
        std::regex expresion("([a-zA-z]+):(.\\s*\\d+)+)");
        std::cmatch expresionMatch;
        for(int i = 0; i <= 1; i++){
            getline(file,linea);
            if(i == 1){ //Arreglo de funciones para simular el proceso en Java.
                std::regex_search(linea, expresionMatch, expresion);
                this -> finales = std::accumulate(begin(expresionMatch), end(expresionMatch),2, [](const AFN &c){
                    return expresionMatch[2];
                });
            }
        }
        file.close();
    }catch(const std::exception& e){
        std::cerr << e.what() << '\n';
    }   
}

void AFN::establecerFinal(int estado){
    int aux[10];
    for(int i = 0; i <= sizeof(finales+1); i++){
        aux[i] = finales[i];
    }
    aux[sizeof(aux)-1]= estado;
    this->finales = aux;
}

bool AFN::esAFN(){
    for(Transicion transicion : this->transiciones){
        if(transicion.getSimbolo() == 'E'){
            return true;
        }
    }
    return false;
}

bool AFN::esAFD(){
    for(Transicion transicion : this->transiciones){
        if(transicion.getSimbolo() == 'E'){
            return false;
        }
    }
    return true;
}