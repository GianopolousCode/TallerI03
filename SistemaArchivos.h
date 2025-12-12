#ifndef SISTEMAARCHIVOS_H
#define SISTEMAARCHIVOS_H

#include "ArbolBPlus.h"
#include <vector>
#include <string>
#include <memory>

class SistemaArchivos {
private:
    ArbolBPlus* arbol;
    int contadorIds;
    int totalAccesosOperacion;
    
public:
    SistemaArchivos();
    ~SistemaArchivos();
    
    // Funciones requeridas por el enunciado (nombres exactos)
    void insertar_node_grafo(int clave, NodoGrafo* node_grafo);
    NodoGrafo* buscar_node_grafo(int clave);
    void crear_node(int id_padre);
    void eliminar_archivo(int id_archivo, int id_directorio_padre);
    void listar_contenido(int id_directorio);
    std::string* obtener_rutas_completas(int id_archivo, int& numRutas);
    int calcular_espacio_ocupado(int id_directorio);
    
    // Funciones auxiliares del menú
    void crearDirectorio(int idPadre, const std::string& nombre);
    void crearArchivo(int idPadre, int tamano, TipoArchivo tipo, const std::string& nombre);
    void mostrarEstado() const;
    int generarNuevoId();
    
    // Estadísticas
    int getTotalAccesos() const;
    void resetContadorAccesos();
    
private:
    // Funciones auxiliares internas
    void obtenerRutasRecursivo(int idNodo, std::string rutaActual, 
                              std::vector<std::string>& rutas);
    void dfsCalcularEspacio(int idNodo, int& espacioTotal);
    NodoGrafo* buscarConContador(int clave);
    void incrementarAccesos(int cantidad);
    
    // Validaciones
    bool validarDirectorio(int idDirectorio);
    bool validarArchivo(int idArchivo);
};

#endif // SISTEMAARCHIVOS_H