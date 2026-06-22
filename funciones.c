#include "funciones.h"


int ValidarIntconRango(int a, int b) {
    int n, aux;
    do {
        aux = scanf("%d", &n);
        while(getchar() != '\n'); 
        if(aux != 1 || n < a || n > b) {
            printf("Error el valor ingresado es incorrecto\n");
            printf("Vuelva a ingresar:\n");
            printf(">> ");
        }
    } while(aux != 1 || n < a || n > b);

    return n;
}

int menu() {
    printf("\n=============================================\n");
    printf("        SISTEMA DE GESTION DE INVENTARIO\n");
    printf("=============================================\n");
    printf("1. Crear/Agregar Vehiculo\n");
    printf("2. Listar Inventario Completo\n");
    printf("3. Editar Vehiculo\n");
    printf("4. Eliminar Vehiculo\n");
    printf("5. Salir\n");
    printf(">> ");
    int opc = ValidarIntconRango(1, 5);
    return opc;
}

void crearVehiculo(int pos) {
    Vehiculo vehiculo;
    
    // Si pos es -1 es un registro nuevo, de lo contrario es una edición
    if (pos == -1) {
        printf("Ingrese el ID numerico del vehiculo: ");
        vehiculo.id = ValidarIntconRango(1, 9999);
    } else {
        FILE *f = fopen("inventario.dat", "rb");
        if (f != NULL) {
            fseek(f, pos, SEEK_SET);
            Vehiculo temp;
            fread(&temp, sizeof(Vehiculo), 1, f);
            vehiculo.id = temp.id; // Conserva el ID original intacto al editar
            fclose(f);
        }
    }

    printf("Ingrese el tipo (Ej: Camioneta, Sedan, SUV): ");
    leerCadena(vehiculo.tipo, 20);
    printf("Ingrese la marca (Ej: Toyota, Chevrolet): ");
    leerCadena(vehiculo.marca, 20);
    printf("Ingrese el modelo (Ej: Hilux, Sail): ");
    leerCadena(vehiculo.modelo, 20);
    printf("Ingrese el estado (Nuevo/Usado): ");
    leerCadena(vehiculo.estado, 10);
    printf("Ingrese el precio (Filtro Entero): ");
    vehiculo.precio = (float)ValidarIntconRango(1, 1000000); 
    vehiculo.disponible = 1; // Por defecto inicia como disponible

    guardarVehiculo(&vehiculo, pos);
}

void guardarVehiculo(Vehiculo *vehiculo, int pos) {
    FILE *f;

    if (pos != -1) {
        f = fopen("inventario.dat", "rb+"); // Modo sobreescritura/edición
    } else {
        f = fopen("inventario.dat", "ab+"); // Modo añadir al final (Anexar)
    }
    
    if (f == NULL) {
        return;
    }
    
    if (pos != -1) {
        fseek(f, pos, SEEK_SET); // Salta al byte exacto para guardar la edición
    }
    
    fwrite(vehiculo, sizeof(Vehiculo), 1, f);
    fclose(f);
}

int leerInventario(Vehiculo *vehiculos, int n) {
    FILE *f = fopen("inventario.dat", "rb");
    if (f == NULL) {
        return 0;
    }

    int numVehiculos = fread(vehiculos, sizeof(Vehiculo), n, f);
    fclose(f);
    return numVehiculos;
}

int buscarVehiculo() {
    int idBuscar;
    Vehiculo vehiculo;
    int pos = -1;
    int flag = 0;
    
    printf("Ingrese el ID numerico del vehiculo a buscar: ");
    idBuscar = ValidarIntconRango(1, 9999);
    
    FILE *f = fopen("inventario.dat", "rb");
    if (f == NULL) {
        return -1;
    }
    
    while (1) {
        long posActual = ftell(f); // Registra el byte de inicio antes de leer el bloque
        if (fread(&vehiculo, sizeof(Vehiculo), 1, f) != 1) {
            break;
        }
        
        // Verifica la coincidencia y que no esté marcado como eliminado (\0)
        if (vehiculo.id == idBuscar && vehiculo.tipo[0] != '\0') {
            flag = 1;
            pos = posActual; 
            break;
        }
    }
    
    fclose(f);
    if (flag == 0) {
        pos = -1;
    }
    return pos;
}

void editarVehiculo() {
    int pos = buscarVehiculo();
    if (pos != -1) {
        crearVehiculo(pos);
        printf("Vehiculo modificado con exito en el sistema.\n");
    } else {
        printf("No se encontro el vehiculo especificado.\n");
    }
}

void eliminarVehiculo() {
    int pos = buscarVehiculo();
    if (pos != -1) {
        FILE *f = fopen("inventario.dat", "rb+");
        if (f == NULL) {
            return;
        }
        fseek(f, pos, SEEK_SET);
        
        Vehiculo vehiculoVacio;
        vehiculoVacio.tipo[0] = '\0'; // Borrado lógico utilizando tu marca de nulo
        vehiculoVacio.id = 0;
        vehiculoVacio.precio = 0;
        vehiculoVacio.disponible = 0;
        
        fwrite(&vehiculoVacio, sizeof(Vehiculo), 1, f);
        fclose(f);
        printf("Vehiculo eliminado exitosamente.\n");
    } else {
        printf("No se encontro el vehiculo especificado.\n");
    }
}

void imprimirInventario() {
    Vehiculo vehiculos[50];
    int numVehiculos = leerInventario(vehiculos, 50);
    printf("\n%-6s %-12s %-12s %-12s %-12s %-15s\n", "ID", "Tipo", "Marca", "Modelo", "Precio", "Estado");
    printf("--------------------------------------------------------------------------------\n");
    for (int i = 0; i < numVehiculos; i++) {
        if (vehiculos[i].tipo[0] != '\0') { 
            printf("%-6d %-12s %-12s %-12s $%-11.2f %-15s (%s)\n", 
                   vehiculos[i].id, 
                   vehiculos[i].tipo, 
                   vehiculos[i].marca, 
                   vehiculos[i].modelo,
                   vehiculos[i].precio,
                   vehiculos[i].estado,
                   vehiculos[i].disponible ? "Disponible" : "No Disponible");
        }
    }
}

void inicializarInventarioBase() {
    FILE *f = fopen("inventario.dat", "rb");
    if (f != NULL) {
        fclose(f);
        return; // Si el archivo físico ya existe, no altera los registros existentes
    }
    
    // Registros iniciales totalmente estándar y genéricos para testing rápido
    Vehiculo porDefecto[3] = {
        {101, "Sedan", "Toyota", "Yaris", "Nuevo", 18500.00, 1}, 
        {102, "SUV", "Hyundai", "Tucson", "Usado", 22000.00, 1}, 
        {103, "Camioneta", "Ford", "Ranger", "Usado", 29000.00, 1}          
    };
    
    f = fopen("inventario.dat", "wb");
    if (f != NULL) {
        fwrite(porDefecto, sizeof(Vehiculo), 3, f);
        fclose(f);
    }
}

void leerCadena(char *cadena, int n) {
    fgets(cadena, n, stdin);
    int len = strlen(cadena) - 1;
    if (cadena[len] == '\n') {
        cadena[len] = '\0';
    }
}
