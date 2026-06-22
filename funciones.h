#include <stdio.h>
#include <string.h>

typedef struct {
    int id;
    char tipo[20];       
    char marca[20];      
    char modelo[20];
    char estado[10];     
    float precio;
    int disponible;      // 1 = Disponible, 0 = No disponible / Vendido
} Vehiculo;

int menu();
void crearVehiculo(int pos);
void guardarVehiculo(Vehiculo *vehiculo, int pos);
int leerInventario(Vehiculo *vehiculos, int n);
void imprimirInventario();
void leerCadena(char *cadena, int n);
int ValidarIntconRango(int a, int b);
int buscarVehiculo();
void editarVehiculo();
void eliminarVehiculo();
void inicializarInventarioBase();
