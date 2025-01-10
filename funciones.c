#include <stdio.h>
#include "funciones.h"
#include <string.h>

int menu() {
    int opcion;
    do {
        printf("\nMenu de opciones:\n");
        printf("1. Crear factura\n");
        printf("2. Leer facturas\n");
        printf("3. Editar factura\n");
        printf("4. Eliminar factura\n");
        printf("5. Consultar factura por cedula\n");
        printf("6. Salir\n");
        printf(">> ");
        scanf("%d", &opcion);
        if (opcion < 1 || opcion > 6) {
            printf("Por favor, ingrese una opción valida.\n");
        }
    } while (opcion < 1 || opcion > 6);
    return opcion;
}

void leerCadena(char *cadena, int num) {
    fflush(stdin);
    fgets(cadena, num, stdin);
    int len = strlen(cadena) - 1;
    if (len >= 0 && cadena[len] == '\n') {
        cadena[len] = '\0';
    }
}

void saveFactura(struct Factura *factura) {
    FILE *file = fopen("factura.dat", "ab+");
    if (file == NULL) {
        printf("Error al abrir el archivo\n");
        return;
    }
    fwrite(factura, sizeof(struct Factura), 1, file);
    printf("Factura guardada\n");
    fclose(file);
}
void createFactura() {
    struct Factura factura;
    validarCadena("Nombre del cliente: ", factura.nombre, 20);
    factura.cedula = validarEnteroPositivo("Cedula del cliente: ");
    factura.numProductos = validarEnteroPositivo("Numero de productos: ");

    factura.total = 0;
    for (int i = 0; i < factura.numProductos; i++) {
        validarCadena("Nombre del producto: ", factura.productos[i].nombre, 50);
        factura.productos[i].cantidad = validarEnteroPositivo("Cantidad: ");
        factura.productos[i].precio = validarFlotantePositivo("Precio: ");
        factura.total += factura.productos[i].cantidad * factura.productos[i].precio;
    }

    saveFactura(&factura);
    printf("Factura creada con exito.\n");
}


void readFactura() {
    FILE *file = fopen("factura.dat", "rb");
    struct Factura factura;

    if (file == NULL) {
        printf("Error al abrir el archivo\n");
        return;
    }

    printf("%-12s %-22s %-7s\n", "Cedula", "Nombre", "Total");
    printf("------------------------------------------------\n");

    while (fread(&factura, sizeof(struct Factura), 1, file)) {
        if (factura.cedula != 0) {
            printf("%-12d %-22s %-7.2f\n", factura.cedula, factura.nombre, factura.total);
        }
    }

    fclose(file);
}

void editarFactura() {
    FILE *file = fopen("factura.dat", "rb+");
    struct Factura factura;
    int cedulaBuscar;
    int encontrado = 0;

    if (file == NULL) {
        printf("Error al abrir el archivo\n");
        return;
    }

    cedulaBuscar = validarEnteroPositivo("Ingrese la cedula de la factura a editar: ");

    while (fread(&factura, sizeof(struct Factura), 1, file)) {
        if (factura.cedula == cedulaBuscar && factura.cedula != 0) {
            printf("Factura encontrada. Ingrese los nuevos datos:\n");
            validarCadena("Nombre del cliente: ", factura.nombre, 20);
            factura.numProductos = validarEnteroPositivo("Numero de productos: ");
            factura.total = 0;

            for (int i = 0; i < factura.numProductos; i++) {
                validarCadena("Nombre del producto: ", factura.productos[i].nombre, 50);
                factura.productos[i].cantidad = validarEnteroPositivo("Cantidad: ");
                factura.productos[i].precio = validarFlotantePositivo("Precio: ");
                factura.total += factura.productos[i].cantidad * factura.productos[i].precio;
            }

            fseek(file, -sizeof(struct Factura), SEEK_CUR);
            fwrite(&factura, sizeof(struct Factura), 1, file);

            printf("Factura actualizada con exito.\n");
            encontrado = 1;
            break;
        }
    }

    fclose(file);

    if (!encontrado) {
        printf("Factura con cedula %d no encontrada o ha sido eliminada.\n", cedulaBuscar);
    }
}



void eliminarFactura() {
    FILE *file = fopen("factura.dat", "rb+");
    struct Factura factura;
    int cedulaEliminar;
    int encontrado = 0;

    if (file == NULL) {
        printf("Error al abrir el archivo\n");
        return;
    }

    printf("Ingrese la cedula de la factura a eliminar: ");
    scanf("%d", &cedulaEliminar);

    while (fread(&factura, sizeof(struct Factura), 1, file)) {
        if (factura.cedula == cedulaEliminar) {
            factura.cedula = 0;
            fseek(file, -sizeof(struct Factura), SEEK_CUR);
            fwrite(&factura, sizeof(struct Factura), 1, file);
            printf("Factura con cedula %d eliminada.\n", cedulaEliminar);
            encontrado = 1;
            break;
        }
    }

    fclose(file);

    if (!encontrado) {
        printf("Factura con cedula %d no encontrada.\n", cedulaEliminar);
    }
}


void exportarFacturas() {
    FILE *file = fopen("factura.dat", "rb");
    FILE *txtFile = fopen("facturas_exportadas.txt", "w");
    struct Factura factura;

    if (file == NULL) {
        printf("No se encontraron facturas para exportar.\n");
        return;
    }

    if (txtFile == NULL) {
        printf("Error al crear el archivo de exportación.\n");
        fclose(file);
        return;
    }

    fprintf(txtFile, "Cedula\t\tNombre\t\tTotal\t\tProductos\n");
    fprintf(txtFile, "------------------------------------------------------------\n");

    while (fread(&factura, sizeof(struct Factura), 1, file)) {
        fprintf(txtFile, "%d\t\t%s\t\t%.2f\t\t", factura.cedula, factura.nombre, factura.total);

        for (int i = 0; i < factura.numProductos; i++) {
            fprintf(txtFile, "%s (%d x %.2f), ", factura.productos[i].nombre, factura.productos[i].cantidad, factura.productos[i].precio);
        }

        fprintf(txtFile, "\n");
    }

    printf("Facturas exportadas exitosamente a facturas_exportadas.txt\n");
    fclose(file);
    fclose(txtFile);
}

void mostrarDetalleFactura() {
    FILE *file = fopen("factura.dat", "rb");
    struct Factura factura;
    int cedulaBuscar;
    int encontrado = 0;

    if (file == NULL) {
        printf("Error al abrir el archivo\n");
        return;
    }

    do {
        printf("Ingrese la cedula de la factura que desea consultar: ");
        scanf("%d", &cedulaBuscar);
        if (cedulaBuscar < 0) {
            printf("La cedula no puede ser negativa. Intentelo de nuevo.\n");
        }
    } while (cedulaBuscar < 0);

    while (fread(&factura, sizeof(struct Factura), 1, file)) {
        if (factura.cedula == cedulaBuscar) {
            printf("Factura encontrada:\n");
            printf("Cedula del cliente: %d\n", factura.cedula);
            printf("Nombre del cliente: %s\n", factura.nombre);
            printf("Productos:\n");
            printf("------------------------------------------------------------\n");
            printf("Nombre\t\tCantidad\tPrecio\tSubtotal\n");
            printf("------------------------------------------------------------\n");

            for (int i = 0; i < factura.numProductos; i++) {
                float subtotal = factura.productos[i].cantidad * factura.productos[i].precio;
                printf("%s\t\t%d\t\t%.2f\t\t%.2f\n", 
                    factura.productos[i].nombre, 
                    factura.productos[i].cantidad,
                    factura.productos[i].precio, 
                    subtotal);
            }
            printf("------------------------------------------------------------\n");
            printf("Total de la factura: %.2f\n", factura.total);
            encontrado = 1;
            break;
        }
    }

    fclose(file);

    if (!encontrado) {
        printf("Factura con cedula %d no encontrada.\n", cedulaBuscar);
    }
}
int validarEnteroPositivo(const char *mensaje) {
    int valor;
    do {
        printf("%s", mensaje);
        if (scanf("%d", &valor) != 1) {
            printf("Entrada invalida. Ingrese un numero entero positivo.\n");
            while (getchar() != '\n'); // Limpiar el buffer de entrada
            valor = -1;
        } else if (valor < 0) {
            printf("El numero debe ser positivo. Intentelo de nuevo.\n");
        }
    } while (valor < 0);
    return valor;
}

float validarFlotantePositivo(const char *mensaje) {
    float valor;
    do {
        printf("%s", mensaje);
        if (scanf("%f", &valor) != 1) {
            printf("Entrada invalida. Ingrese un numero positivo.\n");
            while (getchar() != '\n'); // Limpiar el buffer de entrada
            valor = -1.0;
        } else if (valor < 0) {
            printf("El numero debe ser positivo. Intentelo de nuevo.\n");
        }
    } while (valor < 0);
    return valor;
}

void validarCadena(const char *mensaje, char *cadena, int longitudMaxima) {
    do {
        printf("%s", mensaje);
        fflush(stdin);
        fgets(cadena, longitudMaxima, stdin);
        size_t len = strlen(cadena);
        if (len > 0 && cadena[len - 1] == '\n') {
            cadena[len - 1] = '\0';
        }
        if (strlen(cadena) == 0) {
            printf("La entrada no puede estar vacia. Intentelo de nuevo.\n");
        }
    } while (strlen(cadena) == 0);
}
