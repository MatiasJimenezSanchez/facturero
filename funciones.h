#define FUNCIONES_H



struct Producto {
    char nombre[50];
    int cantidad;
    float precio;
};

struct Factura {
    char nombre[20];
    int cedula;
    int numProductos;
    float total;
    struct Producto productos[100];
};

int menu();
void leerCadena(char *cadena, int num);
void saveFactura(struct Factura *factura);
void createFactura();
void readFactura();
void editarFactura();
void eliminarFactura();
void exportarFacturas();
void mostrarDetalleFactura();
int validarEnteroPositivo();
float validarFlotantePositivo();
void validarCadena();  


