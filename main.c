#include <stdio.h>
#include "funciones.h"

int main(int argc, char *argv[]) {
    int opc;

    // Genera los datos base de prueba si la base de datos se encuentra vacía
    inicializarInventarioBase();

    do {
        opc = menu();
        switch (opc) {
            case 1:
                crearVehiculo(-1); 
                break;
            case 2:
                imprimirInventario();
                break;
            case 3:
                editarVehiculo();
                break;
            case 4:
                eliminarVehiculo();
                break;
            case 5:
                printf("Saliendo de la aplicacion...\n");
                break;
            default:
                printf("Opcion invalida.\n");
                break;
        }
    } while (opc != 5);

    return 0;
}