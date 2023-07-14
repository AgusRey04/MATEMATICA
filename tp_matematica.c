#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#include <string.h>
#define MAX_LOCATIONS 100

// Estructura para representar una ubicación
typedef struct {
    int id;
    char nombre[50];
} Ubicacion;

// Estructura para representar una arista (conexión entre ubicaciones)
typedef struct {
    int desde;
    int hasta;
    int distancia;
} Arista;

// Estructura para representar un grafo
typedef struct {
    Ubicacion ubicaciones[MAX_LOCATIONS];
    int numUbicaciones;
    Arista aristas[MAX_LOCATIONS];
    int numAristas;
} Grafo;

// Función para inicializar un grafo vacío
void inicializarGrafo(Grafo *grafo) {
    grafo->numUbicaciones = 0;
    grafo->numAristas = 0;
}

// Función para agregar una ubicación al grafo
void agregarUbicacion(Grafo *grafo, int id, const char *nombre) {
    Ubicacion ubicacion;
    ubicacion.id = id;
    strcpy(ubicacion.nombre, nombre);
    grafo->ubicaciones[grafo->numUbicaciones++] = ubicacion;
}

// Función para agregar una arista (conexión) entre ubicaciones
void agregarArista(Grafo *grafo, int desde, int hasta, int distancia) {
    Arista arista;
    arista.desde = desde;
    arista.hasta = hasta;
    arista.distancia = distancia;
    grafo->aristas[grafo->numAristas++] = arista;
}

// Función para encontrar el índice de una ubicación en el grafo dado su ID
int encontrarIndiceUbicacion(const Grafo *grafo, int id) {
    for (int i = 0; i < grafo->numUbicaciones; i++) {
        if (grafo->ubicaciones[i].id == id) {
            return i;
        }
    }
    return -1; // Ubicación no encontrada
}

// Declaración de la función mostrarRecorrido
void mostrarRecorrido(const Grafo *grafo, int idInicio, int idFin, const int *padre);

// Función para encontrar la ruta más corta entre dos ubicaciones utilizando Dijkstra
void encontrarRutaMasCorta(const Grafo *grafo, int idInicio, int idFin) {
    int dist[MAX_LOCATIONS];
    bool visitado[MAX_LOCATIONS];
    int padre[MAX_LOCATIONS]; // Arreglo para almacenar los padres de cada ubicación

    // Inicializar distancias, visitados y padres
    for (int i = 0; i < grafo->numUbicaciones; i++) {
        dist[i] = INT_MAX;
        visitado[i] = false;
        padre[i] = -1; // Inicialmente, no hay padres asignados
    }

    // La distancia al nodo de inicio es 0
    int indiceInicio = encontrarIndiceUbicacion(grafo, idInicio);
    dist[indiceInicio] = 0;

    // Encontrar la ruta más corta
    for (int contador = 0; contador < grafo->numUbicaciones - 1; contador++) {
        // Encontrar el nodo con la distancia mínima no visitada
        int minDist = INT_MAX, minIndice;
        for (int i = 0; i < grafo->numUbicaciones; i++) {
            if (!visitado[i] && dist[i] < minDist) {
                minDist = dist[i];
                minIndice = i;
            }
        }

        // Marcar el nodo actual como visitado
        visitado[minIndice] = true;

        // Actualizar las distancias de los nodos adyacentes
        for (int i = 0; i < grafo->numAristas; i++) {
            int indiceDesde = encontrarIndiceUbicacion(grafo, grafo->aristas[i].desde);
            int indiceHasta = encontrarIndiceUbicacion(grafo, grafo->aristas[i].hasta);
            if (indiceDesde == minIndice && !visitado[indiceHasta] && dist[minIndice] != INT_MAX && dist[minIndice] + grafo->aristas[i].distancia < dist[indiceHasta]) {
                dist[indiceHasta] = dist[minIndice] + grafo->aristas[i].distancia;
                padre[indiceHasta] = minIndice; // Actualizar el padre del nodo adyacente
            }
        }
    }

    // Imprimir la ruta más corta
    int indiceFin = encontrarIndiceUbicacion(grafo, idFin);
    printf("Ruta mas corta desde %s hasta %s: %d\n",
        grafo->ubicaciones[indiceInicio].nombre,
        grafo->ubicaciones[indiceFin].nombre,
        dist[indiceFin]);

    // Mostrar el recorrido
    mostrarRecorrido(grafo, idInicio, idFin, padre);
}

// Función para mostrar el recorrido de la ruta más corta
void mostrarRecorrido(const Grafo *grafo, int idInicio, int idFin, const int *padre) {
    int indiceFin = encontrarIndiceUbicacion(grafo, idFin);
    int recorrido[MAX_LOCATIONS];
    int longitudRecorrido = 0;

    // Construir el recorrido desde el nodo de fin hacia el nodo de inicio
    int actual = indiceFin;
    while (actual != -1) {
        recorrido[longitudRecorrido++] = actual;
        actual = padre[actual];
    }

    // Imprimir el recorrido en orden inverso (de inicio a fin)
    printf("Recorrido desde %s hasta %s:\n", grafo->ubicaciones[encontrarIndiceUbicacion(grafo, idInicio)].nombre,grafo->ubicaciones[encontrarIndiceUbicacion(grafo, idFin)].nombre);
    for (int i = longitudRecorrido - 1; i >= 0; i--) {
        printf("%s", grafo->ubicaciones[recorrido[i]].nombre);
        if (i != 0) {
            printf(" -> ");
        }
    }
    printf("\n");
}

int main() {
    Grafo grafo;
    inicializarGrafo(&grafo);

    int numVertices;
    printf("Ingrese la cantidad de vertices (ubicaciones): ");
    scanf("%d", &numVertices);

    // Agregar ubicaciones al grafo
    for (int i = 1; i <= numVertices; i++) {
        char nombre[50];
        printf("Ingrese el nombre de la ubicacion %d: ", i);
        scanf("%s", nombre);
        agregarUbicacion(&grafo, i, nombre);
    }

   // Agregar conexiones entre ubicaciones
    for (int i = 0; i < grafo.numUbicaciones - 1; i++) {
        for (int j = i + 1; j < grafo.numUbicaciones; j++) {
            int distancia;
            do{
            printf("¿Hay una conexion entre %s y %s? (1 para si, 0 para no): ", grafo.ubicaciones[i].nombre, grafo.ubicaciones[j].nombre);
            scanf("%d", &distancia);
            }while(distancia!=1 && distancia!=0);
            if (distancia == 1) {
                printf("Ingrese el peso de la conexion entre %s y %s: ", grafo.ubicaciones[i].nombre, grafo.ubicaciones[j].nombre);
                scanf("%d", &distancia);
                agregarArista(&grafo, grafo.ubicaciones[i].id, grafo.ubicaciones[j].id, distancia);
                agregarArista(&grafo, grafo.ubicaciones[j].id, grafo.ubicaciones[i].id, distancia);
            }
        }
    }

    // Encontrar la ruta más corta entre dos ubicaciones
    int idInicio, idFin;
    printf("Ingrese el ID de la ubicacion de inicio: ");
    scanf("%d", &idInicio);
    printf("Ingrese el ID de la ubicacion de fin: ");
    scanf("%d", &idFin);
    encontrarRutaMasCorta(&grafo, idInicio, idFin);
    system("pause");
    return 0;
}