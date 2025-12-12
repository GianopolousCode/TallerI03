#ifndef ARBOLBPLUS_H
#define ARBOLBPLUS_H

#include "NodoGrafo.h"
#include <vector>
#include <string>

const int ORDEN_DEFAULT = 3;

class NodoBPlus {
protected:
    int orden;
    int numClaves;
    int* claves;
    bool esHoja;
    
public:
    NodoBPlus(int orden, bool esHoja);
    virtual ~NodoBPlus();
    
    bool estaLleno() const;
    bool esNodoHoja() const;
    int getNumClaves() const;
    int* getClaves() const;
    int getOrden() const;
    
    // Métodos para modificar numClaves
    void setNumClaves(int n);
    void incrementarClaves();
    void decrementarClaves();
    
    virtual void insertar(int clave, NodoGrafo* dato, NodoBPlus*& nuevaRaiz) = 0;
    virtual NodoGrafo* buscar(int clave, int& accesos) = 0;
    virtual NodoGrafo* eliminar(int clave, bool& eliminado) = 0;
    virtual void dividir(NodoBPlus* hermanoDerecho, int& clavePromocion) = 0;
    virtual void imprimir(int nivel = 0) const = 0;
    virtual void obtenerHojas(std::vector<NodoBPlus*>& hojas) = 0;
    virtual bool puedePrestar() const = 0;
    virtual bool necesitaFusion() const = 0;
};

class NodoHojaBPlus : public NodoBPlus {
private:
    NodoGrafo** datos;
    NodoHojaBPlus* siguiente;
    
public:
    NodoHojaBPlus(int orden);
    ~NodoHojaBPlus();
    
    void insertar(int clave, NodoGrafo* dato, NodoBPlus*& nuevaRaiz) override;
    NodoGrafo* buscar(int clave, int& accesos) override;
    NodoGrafo* eliminar(int clave, bool& eliminado) override;
    void dividir(NodoBPlus* hermanoDerecho, int& clavePromocion) override;
    void imprimir(int nivel = 0) const override;
    void obtenerHojas(std::vector<NodoBPlus*>& hojas) override;
    bool puedePrestar() const override;
    bool necesitaFusion() const override;
    
    NodoGrafo** getDatos() const;
    NodoHojaBPlus* getSiguiente() const;
    void setSiguiente(NodoHojaBPlus* siguiente);
    
private:
    int encontrarPosicion(int clave) const;
};

class NodoInternoBPlus : public NodoBPlus {
private:
    NodoBPlus** hijos;
    
public:
    NodoInternoBPlus(int orden);
    ~NodoInternoBPlus();
    
    void insertar(int clave, NodoGrafo* dato, NodoBPlus*& nuevaRaiz) override;
    NodoGrafo* buscar(int clave, int& accesos) override;
    NodoGrafo* eliminar(int clave, bool& eliminado) override;
    void dividir(NodoBPlus* hermanoDerecho, int& clavePromocion) override;
    void imprimir(int nivel = 0) const override;
    void obtenerHojas(std::vector<NodoBPlus*>& hojas) override;
    bool puedePrestar() const override;
    bool necesitaFusion() const override;
    
    NodoBPlus** getHijos() const;
    
private:
    int encontrarHijo(int clave) const;
    void insertarEnHijo(int indice, int clave, NodoGrafo* dato, NodoBPlus*& nuevaRaiz);
    void eliminarEnHijo(int indice, int clave, bool& eliminado);
    void fusionarConHermano(int indice);
    void redistribuirConHermano(int indice);
};

class ArbolBPlus {
private:
    NodoBPlus* raiz;
    int orden;
    int totalAccesos;
    int operaciones;
    
public:
    ArbolBPlus(int orden = ORDEN_DEFAULT);
    ~ArbolBPlus();
    
    // Operaciones principales
    void insertar(int clave, NodoGrafo* dato);
    NodoGrafo* buscar(int clave);
    bool eliminar(int clave);
    
    // Operaciones auxiliares
    void imprimir() const;
    void imprimirHojas() const;
    void limpiar();
    
    // Estadisticas
    int getTotalAccesos() const;
    int getOperaciones() const;
    double getPromedioAccesos() const;
    void resetEstadisticas();
    
    // Para pruebas
    bool estaVacio() const;
    bool verificarIntegridad() const;
    
private:
    void limpiarRecursivo(NodoBPlus* nodo);
    NodoGrafo* buscarRecursivo(NodoBPlus* nodo, int clave, int& accesos);
    void dividirRaiz();
    void ajustarDespuesEliminacion(NodoBPlus* nodo);
    void fusionarNodos(NodoBPlus* padre, int indice);
};

#endif // ARBOLBPLUS_H