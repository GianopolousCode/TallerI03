#include "NodoGrafo.h"
#include <algorithm>
#include <string>
#include <sstream>

using namespace std;

// Implementacion de NodoGrafo
NodoGrafo::NodoGrafo(int id, const string& nombre) 
    : id(id), nombre(nombre.empty() ? "nodo_" + to_string(id) : nombre) {}

int NodoGrafo::getId() const {
    return id;
}

string NodoGrafo::getNombre() const {
    return nombre;
}

void NodoGrafo::setNombre(const string& nombre) {
    this->nombre = nombre;
}

void NodoGrafo::agregarPadre(int idPadre) {
    if (find(listaPadres.begin(), listaPadres.end(), idPadre) == listaPadres.end()) {
        listaPadres.push_back(idPadre);
    }
}

void NodoGrafo::eliminarPadre(int idPadre) {
    listaPadres.erase(remove(listaPadres.begin(), listaPadres.end(), idPadre), 
                      listaPadres.end());
}

vector<int> NodoGrafo::obtenerPadres() const {
    return listaPadres;
}

bool NodoGrafo::tienePadres() const {
    return !listaPadres.empty();
}

int NodoGrafo::getNumPadres() const {
    return listaPadres.size();
}

// Implementacion de NodoDirectorio
NodoDirectorio::NodoDirectorio(int id, const string& nombre) 
    : NodoGrafo(id, nombre) {}

bool NodoDirectorio::esDirectorio() const {
    return true;
}

string NodoDirectorio::getTipo() const {
    return "Directorio";
}

int NodoDirectorio::getTamano() const {
    return 4096; // Tamano fijo para directorios (4KB)
}

string NodoDirectorio::getInfo() const {
    stringstream ss;
    ss << "ID: " << getId() 
       << " | Nombre: " << getNombre()
       << " | Tipo: Directorio"
       << " | Tamano: " << getTamano() << " bytes"
       << " | Hijos: " << getNumHijos()
       << " | Padres: " << getNumPadres();
    return ss.str();
}

void NodoDirectorio::agregarHijo(int idHijo) {
    if (find(listaHijos.begin(), listaHijos.end(), idHijo) == listaHijos.end()) {
        listaHijos.push_back(idHijo);
    }
}

void NodoDirectorio::eliminarHijo(int idHijo) {
    listaHijos.erase(remove(listaHijos.begin(), listaHijos.end(), idHijo), 
                     listaHijos.end());
}

vector<int> NodoDirectorio::obtenerHijos() const {
    return listaHijos;
}

bool NodoDirectorio::tieneHijos() const {
    return !listaHijos.empty();
}

int NodoDirectorio::getNumHijos() const {
    return listaHijos.size();
}

// Implementacion de NodoArchivo
NodoArchivo::NodoArchivo(int id, int tamano, TipoArchivo tipo, const string& nombre) 
    : NodoGrafo(id, nombre), tamano(tamano), tipo(tipo) {}

bool NodoArchivo::esDirectorio() const {
    return false;
}

string NodoArchivo::getTipo() const {
    switch(tipo) {
        case IMAGEN: return "Imagen";
        case DOCUMENTO: return "Documento";
        case EJECUTABLE: return "Ejecutable";
        case VIDEO: return "Video";
        case COMPRIMIDO: return "Comprimido";
        default: return "Desconocido";
    }
}

int NodoArchivo::getTamano() const {
    return tamano;
}

string NodoArchivo::getInfo() const {
    stringstream ss;
    ss << "ID: " << getId() 
       << " | Nombre: " << getNombre()
       << " | Tipo: " << getTipo()
       << " | Tamano: " << getTamano() << " bytes"
       << " | Padres: " << getNumPadres();
    return ss.str();
}

TipoArchivo NodoArchivo::getTipoArchivo() const {
    return tipo;
}

void NodoArchivo::setTipoArchivo(TipoArchivo tipo) {
    this->tipo = tipo;
}