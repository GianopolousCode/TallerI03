#include "ArbolBPlus.h"
#include <iostream>
#include <algorithm>
#include <queue>
#include <iomanip>

using namespace std;

// ==================== IMPLEMENTACION NodoBPlus ====================
NodoBPlus::NodoBPlus(int orden, bool esHoja) 
    : orden(orden), numClaves(0), esHoja(esHoja) {
    claves = new int[2 * orden - 1];
}

NodoBPlus::~NodoBPlus() {
    delete[] claves;
}

bool NodoBPlus::estaLleno() const {
    return numClaves == (2 * orden - 1);
}

bool NodoBPlus::esNodoHoja() const {
    return esHoja;
}

int NodoBPlus::getNumClaves() const {
    return numClaves;
}

int* NodoBPlus::getClaves() const {
    return claves;
}

int NodoBPlus::getOrden() const {
    return orden;
}

void NodoBPlus::setNumClaves(int n) {
    numClaves = n;
}

void NodoBPlus::incrementarClaves() {
    numClaves++;
}

void NodoBPlus::decrementarClaves() {
    if (numClaves > 0) numClaves--;
}

// ==================== IMPLEMENTACION NodoHojaBPlus ====================
NodoHojaBPlus::NodoHojaBPlus(int orden) 
    : NodoBPlus(orden, true), siguiente(nullptr) {
    datos = new NodoGrafo*[2 * orden - 1];
    for (int i = 0; i < 2 * orden - 1; i++) {
        datos[i] = nullptr;
    }
}

NodoHojaBPlus::~NodoHojaBPlus() {
    delete[] datos;
}

int NodoHojaBPlus::encontrarPosicion(int clave) const {
    int i = 0;
    while (i < numClaves && claves[i] < clave) {
        i++;
    }
    return i;
}

void NodoHojaBPlus::insertar(int clave, NodoGrafo* dato, NodoBPlus*& nuevaRaiz) {
    int pos = encontrarPosicion(clave);
    
    // Desplazar elementos para hacer espacio
    for (int i = numClaves - 1; i >= pos; i--) {
        claves[i + 1] = claves[i];
        datos[i + 1] = datos[i];
    }
    
    // Insertar nuevo elemento
    claves[pos] = clave;
    datos[pos] = dato;
    incrementarClaves();
    
    nuevaRaiz = nullptr;
}

NodoGrafo* NodoHojaBPlus::buscar(int clave, int& accesos) {
    accesos++;
    int pos = encontrarPosicion(clave);
    if (pos < numClaves && claves[pos] == clave) {
        return datos[pos];
    }
    return nullptr;
}

NodoGrafo* NodoHojaBPlus::eliminar(int clave, bool& eliminado) {
    int pos = encontrarPosicion(clave);
    if (pos < numClaves && claves[pos] == clave) {
        NodoGrafo* eliminadoPtr = datos[pos];
        
        // Desplazar elementos
        for (int i = pos; i < numClaves - 1; i++) {
            claves[i] = claves[i + 1];
            datos[i] = datos[i + 1];
        }
        decrementarClaves();
        
        eliminado = true;
        return eliminadoPtr;
    }
    eliminado = false;
    return nullptr;
}

void NodoHojaBPlus::dividir(NodoBPlus* hermanoDerecho, int& clavePromocion) {
    NodoHojaBPlus* nuevoHermano = dynamic_cast<NodoHojaBPlus*>(hermanoDerecho);
    int mitad = orden - 1;
    
    clavePromocion = claves[mitad];
    
    // Copiar la mitad derecha al nuevo hermano
    for (int i = 0; i < mitad; i++) {
        nuevoHermano->claves[i] = claves[mitad + i];
        nuevoHermano->datos[i] = datos[mitad + i];
    }
    nuevoHermano->setNumClaves(mitad);
    
    // Ajustar el nodo actual
    setNumClaves(mitad);
    
    // Mantener lista enlazada de hojas
    nuevoHermano->siguiente = siguiente;
    siguiente = nuevoHermano;
}

void NodoHojaBPlus::imprimir(int nivel) const {
    cout << string(nivel * 4, ' ') << "[HOJA: ";
    for (int i = 0; i < numClaves; i++) {
        cout << claves[i];
        if (i < numClaves - 1) cout << ", ";
    }
    cout << "]";
    if (siguiente) {
        cout << " -> " << siguiente->claves[0];
    }
    cout << endl;
}

void NodoHojaBPlus::obtenerHojas(vector<NodoBPlus*>& hojas) {
    hojas.push_back(this);
}

bool NodoHojaBPlus::puedePrestar() const {
    return numClaves > (orden - 1);
}

bool NodoHojaBPlus::necesitaFusion() const {
    return numClaves < (orden - 1);
}

NodoGrafo** NodoHojaBPlus::getDatos() const {
    return datos;
}

NodoHojaBPlus* NodoHojaBPlus::getSiguiente() const {
    return siguiente;
}

void NodoHojaBPlus::setSiguiente(NodoHojaBPlus* siguiente) {
    this->siguiente = siguiente;
}

// ==================== IMPLEMENTACION NodoInternoBPlus ====================
NodoInternoBPlus::NodoInternoBPlus(int orden) 
    : NodoBPlus(orden, false) {
    hijos = new NodoBPlus*[2 * orden];
    for (int i = 0; i < 2 * orden; i++) {
        hijos[i] = nullptr;
    }
}

NodoInternoBPlus::~NodoInternoBPlus() {
    delete[] hijos;
}

int NodoInternoBPlus::encontrarHijo(int clave) const {
    int i = 0;
    while (i < numClaves && clave >= claves[i]) {
        i++;
    }
    return i;
}

void NodoInternoBPlus::insertarEnHijo(int indice, int clave, NodoGrafo* dato, NodoBPlus*& nuevaRaiz) {
    NodoBPlus* nuevaRaizHijo = nullptr;
    hijos[indice]->insertar(clave, dato, nuevaRaizHijo);
    
    if (nuevaRaizHijo) {
        int clavePromocion = nuevaRaizHijo->getClaves()[0];
        
        // Desplazar claves e hijos
        for (int i = numClaves - 1; i >= indice; i--) {
            claves[i + 1] = claves[i];
        }
        for (int i = numClaves; i > indice; i--) {
            hijos[i + 1] = hijos[i];
        }
        
        claves[indice] = clavePromocion;
        hijos[indice + 1] = nuevaRaizHijo;
        incrementarClaves();
        
        if (estaLleno()) {
            NodoInternoBPlus* nuevoHermano = new NodoInternoBPlus(orden);
            int nuevaClavePromocion;
            dividir(nuevoHermano, nuevaClavePromocion);
            
            nuevaRaiz = new NodoInternoBPlus(orden);
            NodoInternoBPlus* nuevaRaizPtr = dynamic_cast<NodoInternoBPlus*>(nuevaRaiz);
            if (nuevaRaizPtr) {
                nuevaRaizPtr->getClaves()[0] = nuevaClavePromocion;
                nuevaRaizPtr->getHijos()[0] = this;
                nuevaRaizPtr->getHijos()[1] = nuevoHermano;
                nuevaRaizPtr->setNumClaves(1);
            }
        }
    }
}

void NodoInternoBPlus::insertar(int clave, NodoGrafo* dato, NodoBPlus*& nuevaRaiz) {
    int indice = encontrarHijo(clave);
    insertarEnHijo(indice, clave, dato, nuevaRaiz);
}

NodoGrafo* NodoInternoBPlus::buscar(int clave, int& accesos) {
    accesos++;
    int indice = encontrarHijo(clave);
    if (hijos[indice]) {
        return hijos[indice]->buscar(clave, accesos);
    }
    return nullptr;
}

void NodoInternoBPlus::eliminarEnHijo(int indice, int clave, bool& eliminado) {
    hijos[indice]->eliminar(clave, eliminado);
}

NodoGrafo* NodoInternoBPlus::eliminar(int clave, bool& eliminado) {
    int indice = encontrarHijo(clave);
    NodoGrafo* resultado = nullptr;
    
    if (hijos[indice]) {
        resultado = hijos[indice]->eliminar(clave, eliminado);
        
        if (eliminado) {
            // Marcar que se necesita ajuste (se maneja en ArbolBPlus)
            // En una implementación completa aquí se haría el ajuste
        }
    }
    
    return resultado;
}

void NodoInternoBPlus::dividir(NodoBPlus* hermanoDerecho, int& clavePromocion) {
    NodoInternoBPlus* nuevoHermano = dynamic_cast<NodoInternoBPlus*>(hermanoDerecho);
    int mitad = orden - 1;
    
    clavePromocion = claves[mitad];
    
    // Copiar claves e hijos derechos al nuevo hermano
    for (int i = 0; i < mitad; i++) {
        nuevoHermano->claves[i] = claves[mitad + 1 + i];
        nuevoHermano->hijos[i] = hijos[mitad + 1 + i];
    }
    nuevoHermano->hijos[mitad] = hijos[2 * orden - 1];
    nuevoHermano->setNumClaves(mitad);
    
    // Ajustar el nodo actual
    setNumClaves(mitad);
}

void NodoInternoBPlus::imprimir(int nivel) const {
    cout << string(nivel * 4, ' ') << "[INTERNO: ";
    for (int i = 0; i < numClaves; i++) {
        cout << claves[i];
        if (i < numClaves - 1) cout << ", ";
    }
    cout << "]" << endl;
    
    for (int i = 0; i <= numClaves; i++) {
        if (hijos[i]) {
            hijos[i]->imprimir(nivel + 1);
        }
    }
}

void NodoInternoBPlus::obtenerHojas(vector<NodoBPlus*>& hojas) {
    for (int i = 0; i <= numClaves; i++) {
        if (hijos[i]) {
            hijos[i]->obtenerHojas(hojas);
        }
    }
}

bool NodoInternoBPlus::puedePrestar() const {
    return numClaves > (orden - 1);
}

bool NodoInternoBPlus::necesitaFusion() const {
    return numClaves < (orden - 1);
}

NodoBPlus** NodoInternoBPlus::getHijos() const {
    return hijos;
}

void NodoInternoBPlus::fusionarConHermano(int indice) {
    // Implementación simplificada
}

void NodoInternoBPlus::redistribuirConHermano(int indice) {
    // Implementación simplificada
}

// ==================== IMPLEMENTACION ArbolBPlus ====================
ArbolBPlus::ArbolBPlus(int orden) 
    : raiz(nullptr), orden(orden), totalAccesos(0), operaciones(0) {}

ArbolBPlus::~ArbolBPlus() {
    limpiar();
}

void ArbolBPlus::limpiar() {
    limpiarRecursivo(raiz);
    raiz = nullptr;
}

void ArbolBPlus::limpiarRecursivo(NodoBPlus* nodo) {
    if (!nodo) return;
    
    if (!nodo->esNodoHoja()) {
        NodoInternoBPlus* nodoInterno = dynamic_cast<NodoInternoBPlus*>(nodo);
        NodoBPlus** hijos = nodoInterno->getHijos();
        
        for (int i = 0; i <= nodo->getNumClaves(); i++) {
            if (hijos[i]) {
                limpiarRecursivo(hijos[i]);
            }
        }
    }
    
    delete nodo;
}

void ArbolBPlus::insertar(int clave, NodoGrafo* dato) {
    operaciones++;
    
    if (!raiz) {
        raiz = new NodoHojaBPlus(orden);
        NodoBPlus* nuevaRaiz = nullptr;
        raiz->insertar(clave, dato, nuevaRaiz);
        return;
    }
    
    NodoBPlus* nuevaRaiz = nullptr;
    raiz->insertar(clave, dato, nuevaRaiz);
    
    if (nuevaRaiz) {
        // Convertir a NodoInternoBPlus para acceder a hijos
        NodoInternoBPlus* nuevaRaizInterna = dynamic_cast<NodoInternoBPlus*>(nuevaRaiz);
        if (nuevaRaizInterna) {
            // Si ya es un NodoInternoBPlus, usarlo directamente
            raiz = nuevaRaizInterna;
        } else {
            // Si no, crear uno nuevo
            NodoInternoBPlus* nuevaRaizFinal = new NodoInternoBPlus(orden);
            // Usar getClaves() en lugar de acceso directo
            int primeraClave = nuevaRaiz->getClaves()[0];
            nuevaRaizFinal->getClaves()[0] = primeraClave;
            nuevaRaizFinal->getHijos()[0] = raiz;
            nuevaRaizFinal->getHijos()[1] = nuevaRaiz;
            nuevaRaizFinal->setNumClaves(1);
            raiz = nuevaRaizFinal;
        }
    }
}

NodoGrafo* ArbolBPlus::buscar(int clave) {
    operaciones++;
    int accesos = 0;
    
    if (!raiz) {
        totalAccesos += accesos;
        return nullptr;
    }
    
    NodoGrafo* resultado = raiz->buscar(clave, accesos);
    totalAccesos += accesos;
    
    return resultado;
}

bool ArbolBPlus::eliminar(int clave) {
    operaciones++;
    
    if (!raiz) {
        return false;
    }
    
    bool eliminado = false;
    NodoGrafo* eliminadoPtr = raiz->eliminar(clave, eliminado);
    
    if (eliminado) {
        // Verificar si la raiz quedó vacía
        if (raiz->getNumClaves() == 0 && !raiz->esNodoHoja()) {
            NodoInternoBPlus* raizVieja = dynamic_cast<NodoInternoBPlus*>(raiz);
            raiz = raizVieja->getHijos()[0];
            delete raizVieja;
        }
        return true;
    }
    
    return false;
}

void ArbolBPlus::imprimir() const {
    if (!raiz) {
        cout << "Arbol B+ vacio" << endl;
        return;
    }
    
    cout << "\n=== ESTRUCTURA DEL ARBOL B+ ===" << endl;
    raiz->imprimir();
    cout << "===============================\n" << endl;
}

void ArbolBPlus::imprimirHojas() const {
    if (!raiz) {
        cout << "Arbol vacio" << endl;
        return;
    }
    
    vector<NodoBPlus*> hojas;
    raiz->obtenerHojas(hojas);
    
    cout << "\n=== HOJAS DEL ARBOL B+ (lista enlazada) ===" << endl;
    for (size_t i = 0; i < hojas.size(); i++) {
        hojas[i]->imprimir(0);
    }
    cout << "==========================================\n" << endl;
}

int ArbolBPlus::getTotalAccesos() const {
    return totalAccesos;
}

int ArbolBPlus::getOperaciones() const {
    return operaciones;
}

double ArbolBPlus::getPromedioAccesos() const {
    return operaciones > 0 ? (double)totalAccesos / operaciones : 0.0;
}

void ArbolBPlus::resetEstadisticas() {
    totalAccesos = 0;
    operaciones = 0;
}

bool ArbolBPlus::estaVacio() const {
    return raiz == nullptr;
}

bool ArbolBPlus::verificarIntegridad() const {
    if (!raiz) return true;
    
    vector<NodoBPlus*> hojas;
    raiz->obtenerHojas(hojas);
    
    for (NodoBPlus* hoja : hojas) {
        int* claves = hoja->getClaves();
        int numClaves = hoja->getNumClaves();
        
        for (int i = 1; i < numClaves; i++) {
            if (claves[i] <= claves[i - 1]) {
                return false;
            }
        }
    }
    
    return true;
}

void ArbolBPlus::dividirRaiz() {
    if (!raiz || !raiz->estaLleno()) return;
    
    if (raiz->esNodoHoja()) {
        NodoHojaBPlus* raizVieja = dynamic_cast<NodoHojaBPlus*>(raiz);
        NodoHojaBPlus* nuevaHoja = new NodoHojaBPlus(orden);
        int clavePromocion;
        
        raizVieja->dividir(nuevaHoja, clavePromocion);
        
        NodoInternoBPlus* nuevaRaiz = new NodoInternoBPlus(orden);
        nuevaRaiz->getClaves()[0] = clavePromocion;
        nuevaRaiz->getHijos()[0] = raizVieja;
        nuevaRaiz->getHijos()[1] = nuevaHoja;
        nuevaRaiz->setNumClaves(1);
        
        raiz = nuevaRaiz;
    } else {
        NodoInternoBPlus* raizVieja = dynamic_cast<NodoInternoBPlus*>(raiz);
        NodoInternoBPlus* nuevaHoja = new NodoInternoBPlus(orden);
        int clavePromocion;
        
        raizVieja->dividir(nuevaHoja, clavePromocion);
        
        NodoInternoBPlus* nuevaRaiz = new NodoInternoBPlus(orden);
        nuevaRaiz->getClaves()[0] = clavePromocion;
        nuevaRaiz->getHijos()[0] = raizVieja;
        nuevaRaiz->getHijos()[1] = nuevaHoja;
        nuevaRaiz->setNumClaves(1);
        
        raiz = nuevaRaiz;
    }
}

void ArbolBPlus::ajustarDespuesEliminacion(NodoBPlus* nodo) {
    if (!nodo || nodo->esNodoHoja()) return;
    
    NodoInternoBPlus* nodoInterno = dynamic_cast<NodoInternoBPlus*>(nodo);
    if (nodoInterno && nodoInterno->necesitaFusion()) {
        // Simplificado - en implementación real aquí se fusionarían nodos
    }
}

void ArbolBPlus::fusionarNodos(NodoBPlus* padre, int indice) {
    // Implementación simplificada
}