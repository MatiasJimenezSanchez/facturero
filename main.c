#include <stdio.h>
#include "funciones.h"

int main() {
    int opcion;
    do {
        opcion = menu();
        switch (opcion) {
            case 1:
                createFactura();
                break;
            case 2:
                readFactura();
                break;
            case 3:
                editarFactura();
                break;
            case 4:
                eliminarFactura();
                break;
            case 5:
                mostrarDetalleFactura();
                break;
            case 6:
                printf("Exportando facturas...\n");
                exportarFacturas();
                printf("Saliendo del programa...\n");
                break;
            default:
                printf("Opción no válida.\n");
        }
    } while (opcion != 6);

    return 0;
}