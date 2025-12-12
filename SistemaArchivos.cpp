#include "SistemaArchivos.h"
#include <iostream>
#include <queue>
#include <stack>
#include <algorithm>
#include <sstream>

using namespace std;

SistemaArchivos::SistemaArchivos() 
    : arbol(new ArbolBPlus()), contadorIds(1000), totalAccesosOperacion(0) {}

SistemaArchivos::~SistemaArchivos() {
    delete arbol;
}

int SistemaArchivos::generarNuevoId() {
    return contadorIds++;
}

void SistemaArchivos::incrementarAccesos(int cantidad) {
    totalAccesosOperacion += cantidad;
}

NodoGrafo* SistemaArchivos::buscarConContador(int clave) {
    NodoGrafo* resultado = arbol->buscar(clave);
    // El contador ya se incrementa dentro de arbol->buscar()
    return resultado;
}

// ==================== FUNCIONES REQUERIDAS POR EL ENUNCIADO ====================

void SistemaArchivos::insertar_node_grafo(int clave, NodoGrafo* node_grafo) {
    cout << "\n=== INSERTANDO NODO EN GRAFO Y ARBOL B+ ===" << endl;
    arbol->insertar(clave, node_grafo);
    cout << "Nodo insertado - ID: " << clave 
         << ", Tipo: " << (node_grafo->esDirectorio() ? "Directorio" : "Archivo")
         << ", Nombre: " << node_grafo->getNombre() << endl;
    cout << "==========================================\n" << endl;
}

NodoGrafo* SistemaArchivos::buscar_node_grafo(int clave) {
    cout << "\n=== BUSCANDO NODO POR ID ===" << endl;
    NodoGrafo* resultado = buscarConContador(clave);
    
    if (resultado) {
        cout << "NODO ENCONTRADO:" << endl;
        cout << "  " << resultado->getInfo() << endl;
    } else {
        cout << "Nodo con ID " << clave << " no encontrado" << endl;
    }
    cout << "============================\n" << endl;
    
    return resultado;
}

void SistemaArchivos::crear_node(int id_padre) {
    // Crear un nodo con valores por defecto
    crearDirectorio(id_padre, "nuevo_directorio_" + to_string(generarNuevoId()));
}

void SistemaArchivos::eliminar_archivo(int id_archivo, int id_directorio_padre) {
    cout << "\n=== ELIMINANDO ARCHIVO ===" << endl;
    
    // Buscar el archivo (conteo de accesos)
    NodoGrafo* archivo = buscarConContador(id_archivo);
    if (!archivo) {
        cout << "ERROR: Archivo con ID " << id_archivo << " no existe" << endl;
        return;
    }
    
    if (archivo->esDirectorio()) {
        cout << "ERROR: ID " << id_archivo << " corresponde a un directorio, no a un archivo" << endl;
        return;
    }
    
    // Buscar el directorio padre (conteo de accesos)
    NodoGrafo* padre = buscarConContador(id_directorio_padre);
    if (!padre || !padre->esDirectorio()) {
        cout << "ERROR: Directorio padre con ID " << id_directorio_padre << " no valido" << endl;
        return;
    }
    
    // Eliminar referencia del directorio padre
    NodoDirectorio* dirPadre = dynamic_cast<NodoDirectorio*>(padre);
    dirPadre->eliminarHijo(id_archivo);
    cout << "  Referencia eliminada del directorio " << id_directorio_padre << endl;
    
    // Eliminar referencia del padre en el archivo
    archivo->eliminarPadre(id_directorio_padre);
    
    // Verificar si el archivo aún tiene padres (conteo de referencias)
    if (!archivo->tienePadres()) {
        // Eliminar completamente del árbol B+
        bool eliminado = arbol->eliminar(id_archivo);
        if (eliminado) {
            cout << "  Archivo " << id_archivo << " eliminado completamente del sistema" << endl;
            delete archivo; // Liberar memoria
        } else {
            cout << "  Advertencia: No se pudo eliminar el archivo del árbol B+" << endl;
        }
    } else {
        cout << "  Archivo mantiene " << archivo->getNumPadres() << " referencia(s) mas" << endl;
    }
    
    cout << "==========================\n" << endl;
}

void SistemaArchivos::listar_contenido(int id_directorio) {
    cout << "\n=== LISTANDO CONTENIDO DE DIRECTORIO ===" << endl;
    
    // Buscar directorio (conteo de accesos)
    NodoGrafo* directorio = buscarConContador(id_directorio);
    if (!directorio) {
        cout << "ERROR: Directorio con ID " << id_directorio << " no existe" << endl;
        return;
    }
    
    if (!directorio->esDirectorio()) {
        cout << "ERROR: ID " << id_directorio << " no corresponde a un directorio" << endl;
        return;
    }
    
    NodoDirectorio* dir = dynamic_cast<NodoDirectorio*>(directorio);
    vector<int> hijos = dir->obtenerHijos();
    
    cout << "Contenido del directorio [" << id_directorio << "] " 
         << directorio->getNombre() << ":" << endl;
    cout << "----------------------------------------" << endl;
    
    if (hijos.empty()) {
        cout << "  (Directorio vacio)" << endl;
    } else {
        // Para cada hijo, buscar en el árbol B+ (conteo de accesos por cada hijo)
        for (int idHijo : hijos) {
            NodoGrafo* hijo = buscarConContador(idHijo);
            if (hijo) {
                cout << "  " << hijo->getInfo() << endl;
            }
        }
    }
    
    cout << "----------------------------------------" << endl;
    cout << "Total elementos: " << hijos.size() << endl;
    cout << "========================================\n" << endl;
}

string* SistemaArchivos::obtener_rutas_completas(int id_archivo, int& numRutas) {
    cout << "\n=== OBTENIENDO RUTAS COMPLETAS ===" << endl;
    
    numRutas = 0;
    string* rutasArray = nullptr;
    
    // Buscar archivo (conteo de accesos)
    NodoGrafo* archivo = buscarConContador(id_archivo);
    if (!archivo) {
        cout << "ERROR: Archivo con ID " << id_archivo << " no existe" << endl;
        return nullptr;
    }
    
    if (archivo->esDirectorio()) {
        cout << "ERROR: ID " << id_archivo << " corresponde a un directorio" << endl;
        return nullptr;
    }
    
    vector<string> rutas;
    obtenerRutasRecursivo(id_archivo, "", rutas);
    
    numRutas = rutas.size();
    if (numRutas > 0) {
        rutasArray = new string[numRutas];
        for (int i = 0; i < numRutas; i++) {
            rutasArray[i] = rutas[i];
        }
    }
    
    cout << "RUTAS ENCONTRADAS (" << numRutas << "):" << endl;
    for (int i = 0; i < numRutas; i++) {
        cout << "  " << i + 1 << ". " << rutas[i] << endl;
    }
    
    cout << "==================================\n" << endl;
    return rutasArray;
}

int SistemaArchivos::calcular_espacio_ocupado(int id_directorio) {
    cout << "\n=== CALCULANDO ESPACIO OCUPADO ===" << endl;
    
    // Buscar directorio (conteo de accesos)
    NodoGrafo* directorio = buscarConContador(id_directorio);
    if (!directorio) {
        cout << "ERROR: Directorio con ID " << id_directorio << " no existe" << endl;
        return 0;
    }
    
    if (!directorio->esDirectorio()) {
        cout << "ERROR: ID " << id_directorio << " no corresponde a un directorio" << endl;
        return 0;
    }
    
    int espacioTotal = 0;
    dfsCalcularEspacio(id_directorio, espacioTotal);
    
    cout << "RESULTADO:" << endl;
    cout << "  Directorio: [" << id_directorio << "] " << directorio->getNombre() << endl;
    cout << "  Espacio total: " << espacioTotal << " bytes (" 
         << (espacioTotal / 1024.0) << " KB)" << endl;
    cout << "  Accesos a disco realizados en esta operacion: " 
         << totalAccesosOperacion << endl;
    
    cout << "==================================\n" << endl;
    
    int resultado = espacioTotal;
    totalAccesosOperacion = 0; // Resetear para próxima operación
    return resultado;
}

// ==================== FUNCIONES AUXILIARES ====================

void SistemaArchivos::crearDirectorio(int idPadre, const string& nombre) {
    cout << "\n=== CREANDO DIRECTORIO ===" << endl;
    
    int nuevoId = generarNuevoId();
    NodoDirectorio* nuevoDir = new NodoDirectorio(nuevoId, nombre);
    
    // Insertar en el árbol B+
    insertar_node_grafo(nuevoId, nuevoDir);
    
    // Si tiene padre, establecer relación
    if (idPadre != -1) {
        NodoGrafo* padre = buscarConContador(idPadre);
        if (padre && padre->esDirectorio()) {
            NodoDirectorio* dirPadre = dynamic_cast<NodoDirectorio*>(padre);
            if (dirPadre) {
                dirPadre->agregarHijo(nuevoId);
                nuevoDir->agregarPadre(idPadre);
                cout << "  Relacion establecida: " << idPadre << " -> " << nuevoId << endl;
            }
        } else {
            cout << "  Advertencia: Padre no encontrado o no es directorio" << endl;
        }
    } else {
        cout << "  Directorio creado como raiz" << endl;
    }
    
    cout << "DIRECTORIO CREADO: " << nuevoDir->getInfo() << endl;
    cout << "===========================\n" << endl;
}

void SistemaArchivos::crearArchivo(int idPadre, int tamano, TipoArchivo tipo, const string& nombre) {
    cout << "\n=== CREANDO ARCHIVO ===" << endl;
    
    int nuevoId = generarNuevoId();
    NodoArchivo* nuevoArchivo = new NodoArchivo(nuevoId, tamano, tipo, nombre);
    
    // Insertar en el árbol B+
    insertar_node_grafo(nuevoId, nuevoArchivo);
    
    // Si tiene padre, establecer relación
    if (idPadre != -1) {
        NodoGrafo* padre = buscarConContador(idPadre);
        if (padre && padre->esDirectorio()) {
            NodoDirectorio* dirPadre = dynamic_cast<NodoDirectorio*>(padre);
            if (dirPadre) {
                dirPadre->agregarHijo(nuevoId);
                nuevoArchivo->agregarPadre(idPadre);
                cout << "  Relacion establecida: " << idPadre << " -> " << nuevoId << endl;
            }
        } else {
            cout << "  Advertencia: Padre no encontrado o no es directorio" << endl;
        }
    }
    
    cout << "ARCHIVO CREADO: " << nuevoArchivo->getInfo() << endl;
    cout << "=======================\n" << endl;
}

void SistemaArchivos::obtenerRutasRecursivo(int idNodo, string rutaActual, 
                                          vector<string>& rutas) {
    // Buscar nodo (conteo de accesos)
    NodoGrafo* nodo = buscarConContador(idNodo);
    if (!nodo) return;
    
    string elemento = "/" + nodo->getNombre() + 
                     (nodo->esDirectorio() ? " (D)" : " (A)");
    
    rutaActual = elemento + rutaActual;
    
    if (!nodo->tienePadres()) {
        // Llegamos a la raíz
        rutas.push_back(rutaActual);
        return;
    }
    
    vector<int> padres = nodo->obtenerPadres();
    for (int idPadre : padres) {
        obtenerRutasRecursivo(idPadre, rutaActual, rutas);
    }
}

void SistemaArchivos::dfsCalcularEspacio(int idNodo, int& espacioTotal) {
    // Buscar nodo (conteo de accesos)
    NodoGrafo* nodo = buscarConContador(idNodo);
    if (!nodo) return;
    
    // Sumar espacio del nodo actual
    espacioTotal += nodo->getTamano();
    
    // Si es directorio, procesar hijos
    if (nodo->esDirectorio()) {
        NodoDirectorio* directorio = dynamic_cast<NodoDirectorio*>(nodo);
        vector<int> hijos = directorio->obtenerHijos();
        
        for (int idHijo : hijos) {
            dfsCalcularEspacio(idHijo, espacioTotal);
        }
    }
}

void SistemaArchivos::mostrarEstado() const {
    cout << "\n=== ESTADO DEL SISTEMA DE ARCHIVOS ===" << endl;
    cout << "Proximo ID disponible: " << contadorIds << endl;
    cout << "Estadisticas del Arbol B+:" << endl;
    cout << "  Operaciones realizadas: " << arbol->getOperaciones() << endl;
    cout << "  Accesos a disco totales: " << arbol->getTotalAccesos() << endl;
    cout << "  Promedio accesos por operacion: " << arbol->getPromedioAccesos() << endl;
    cout << "\nEstructura del Arbol B+:" << endl;
    arbol->imprimir();
    cout << "\nHojas del Arbol B+ (lista enlazada):" << endl;
    arbol->imprimirHojas();
    cout << "=======================================\n" << endl;
}

int SistemaArchivos::getTotalAccesos() const {
    return arbol->getTotalAccesos();
}

void SistemaArchivos::resetContadorAccesos() {
    arbol->resetEstadisticas();
}

bool SistemaArchivos::validarDirectorio(int idDirectorio) {
    NodoGrafo* nodo = buscarConContador(idDirectorio);
    return nodo && nodo->esDirectorio();
}

bool SistemaArchivos::validarArchivo(int idArchivo) {
    NodoGrafo* nodo = buscarConContador(idArchivo);
    return nodo && !nodo->esDirectorio();
}