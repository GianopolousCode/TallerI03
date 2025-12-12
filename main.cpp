#include "SistemaArchivos.h"
#include <iostream>
#include <limits>
#include <memory>

using namespace std;

void mostrarMenu() {
    cout << "\n=== SISTEMA DE ARCHIVOS CON GRAFO Y ARBOL B+ ===" << endl;
    cout << "1. Crear directorio" << endl;
    cout << "2. Crear archivo" << endl;
    cout << "3. Buscar nodo por ID" << endl;
    cout << "4. Listar contenido de directorio" << endl;
    cout << "5. Eliminar archivo" << endl;
    cout << "6. Obtener rutas completas de archivo" << endl;
    cout << "7. Calcular espacio ocupado por directorio" << endl;
    cout << "8. Mostrar estado del sistema" << endl;
    cout << "9. Resetear estadisticas de accesos" << endl;
    cout << "10. Salir" << endl;
    cout << "Seleccione una opcion: ";
}

int obtenerEntero(const string& mensaje) {
    int valor;
    while (true) {
        cout << mensaje;
        if (cin >> valor) {
            return valor;
        } else {
            cout << "Entrada invalida. Intente nuevamente." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

TipoArchivo seleccionarTipoArchivo() {
    cout << "\nSeleccione el tipo de archivo:" << endl;
    cout << "0. Imagen" << endl;
    cout << "1. Documento" << endl;
    cout << "2. Ejecutable" << endl;
    cout << "3. Video" << endl;
    cout << "4. Comprimido" << endl;
    
    int opcion;
    while (true) {
        cout << "Opcion: ";
        if (cin >> opcion && opcion >= 0 && opcion <= 4) {
            return static_cast<TipoArchivo>(opcion);
        } else {
            cout << "Opcion invalida. Intente nuevamente." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

int main() {
    SistemaArchivos sistema;
    
    // Crear directorio raiz
    cout << "=== INICIALIZANDO SISTEMA ===" << endl;
    sistema.crearDirectorio(-1, "raiz");
    cout << "Sistema listo. Directorio raiz creado." << endl;
    cout << "=============================\n" << endl;
    
    bool ejecutando = true;
    
    while (ejecutando) {
        mostrarMenu();
        
        int opcion;
        cin >> opcion;
        
        switch (opcion) {
            case 1: {
                int idPadre = obtenerEntero("ID del directorio padre (-1 para raiz): ");
                string nombre;
                cout << "Nombre del directorio: ";
                cin >> nombre;
                sistema.crearDirectorio(idPadre, nombre);
                break;
            }
            
            case 2: {
                int idPadre = obtenerEntero("ID del directorio padre: ");
                int tamano = obtenerEntero("Tamano del archivo (bytes): ");
                TipoArchivo tipo = seleccionarTipoArchivo();
                string nombre;
                cout << "Nombre del archivo: ";
                cin >> nombre;
                sistema.crearArchivo(idPadre, tamano, tipo, nombre);
                break;
            }
            
            case 3: {
                int id = obtenerEntero("ID del nodo a buscar: ");
                sistema.buscar_node_grafo(id);
                break;
            }
            
            case 4: {
                int idDirectorio = obtenerEntero("ID del directorio: ");
                sistema.listar_contenido(idDirectorio);
                break;
            }
            
            case 5: {
                int idArchivo = obtenerEntero("ID del archivo a eliminar: ");
                int idPadre = obtenerEntero("ID del directorio padre: ");
                sistema.eliminar_archivo(idArchivo, idPadre);
                break;
            }
            
            case 6: {
                int idArchivo = obtenerEntero("ID del archivo: ");
                int numRutas;
                string* rutas = sistema.obtener_rutas_completas(idArchivo, numRutas);
                if (rutas) {
                    cout << "\nTotal de rutas: " << numRutas << endl;
                    delete[] rutas; // Liberar memoria asignada
                }
                break;
            }
            
            case 7: {
                int idDirectorio = obtenerEntero("ID del directorio: ");
                sistema.calcular_espacio_ocupado(idDirectorio);
                break;
            }
            
            case 8: {
                sistema.mostrarEstado();
                break;
            }
            
            case 9: {
                sistema.resetContadorAccesos();
                cout << "\nEstadisticas de accesos reseteadas." << endl;
                break;
            }
            
            case 10: {
                ejecutando = false;
                cout << "\n=== FINALIZANDO SISTEMA ===" << endl;
                cout << "Accesos totales a disco: " << sistema.getTotalAccesos() << endl;
                cout << "Saliendo del sistema..." << endl;
                cout << "===========================\n" << endl;
                break;
            }
            
            default: {
                cout << "Opcion invalida. Intente nuevamente." << endl;
                break;
            }
        }
        
        // Limpiar buffer de entrada
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    
    return 0;
}