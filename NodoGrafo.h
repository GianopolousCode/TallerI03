#ifndef NODOGRAFO_H
#define NODOGRAFO_H

#include <vector>
#include <string>

enum TipoArchivo {
    IMAGEN = 0,
    DOCUMENTO = 1,
    EJECUTABLE = 2,
    VIDEO = 3,
    COMPRIMIDO = 4
};

class NodoGrafo {
protected:
    int id;
    std::vector<int> listaPadres;
    std::string nombre;
    
public:
    NodoGrafo(int id, const std::string& nombre = "");
    virtual ~NodoGrafo() {}
    
    int getId() const;
    std::string getNombre() const;
    void setNombre(const std::string& nombre);
    
    void agregarPadre(int idPadre);
    void eliminarPadre(int idPadre);
    std::vector<int> obtenerPadres() const;
    bool tienePadres() const;
    int getNumPadres() const;
    
    virtual bool esDirectorio() const = 0;
    virtual std::string getTipo() const = 0;
    virtual int getTamano() const = 0;
    virtual std::string getInfo() const = 0;
};

class NodoDirectorio : public NodoGrafo {
private:
    std::vector<int> listaHijos;
    
public:
    NodoDirectorio(int id, const std::string& nombre = "");
    
    bool esDirectorio() const override;
    std::string getTipo() const override;
    int getTamano() const override;
    std::string getInfo() const override;
    
    void agregarHijo(int idHijo);
    void eliminarHijo(int idHijo);
    std::vector<int> obtenerHijos() const;
    bool tieneHijos() const;
    int getNumHijos() const;
};

class NodoArchivo : public NodoGrafo {
private:
    int tamano;
    TipoArchivo tipo;
    
public:
    NodoArchivo(int id, int tamano, TipoArchivo tipo, const std::string& nombre = "");
    
    bool esDirectorio() const override;
    std::string getTipo() const override;
    int getTamano() const override;
    std::string getInfo() const override;
    
    TipoArchivo getTipoArchivo() const;
    void setTipoArchivo(TipoArchivo tipo);
};

#endif // NODOGRAFO_H