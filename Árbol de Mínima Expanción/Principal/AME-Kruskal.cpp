/**********************************************
 *** Algoritmo: Árbol de Mínima Expanción - Método de Kruskal
 *** Programa hecho por: David Pérez Sánchez  3ª "A"
 *** UP-Chiapas  I.D.S.
 *** Completado el 16 de Julio de 2017, en 3 días.
 *********************************************/
#include <iostream>
#include <conio2.h>
#include <algorithm>
#include <windows.h>
using namespace std;

// Prototipos de Funciones

void algoritmoDeKruskal(int, int);
void inicializarComponentes(int, int []);
int encontrarRaizVertice(int, int []);
void unirComponentesConexas(int, int, int []);
bool comprobarMismaComponenteConexa(int, int, int []);

//Estructura arista ( ARISTA )
struct ARISTA{
    int origen;     // Vértice origen
    int destino;    // Vértice destino
    int peso;       // Peso entre el vértice origen y destino
    //ARISTA(){}
    // Comparador por peso, al momento de ordenar lo realizara en orden ascendente
    // Cambiar signo a > para obtener el arbol de expansión máxima
    bool operator<( const ARISTA &a ) const {
        return peso < a.peso;
    }
} arista[ 1024 ];      //Arreglo de aristas para el uso en Kruskal

int main(){
	
    setlocale(LC_ALL,"spanish");
	system("title Árbol de Expansión Mínima");
	int numeroDeVertices, numeroDeAristas, verticeInicial, verticeFinal, pesoDeLaArista;
	bool banderaNumeroExcedido= false;
	do{
		clrscr();
		cout<<"\n ÁRBOL DE EXPANSIÓN MÍNIMA"<< endl;
		cout<<" --------------------------------"<< endl<< endl<< endl;
		cout<<"\n Ingrese el número de vertices o nodos: ";
		if(banderaNumeroExcedido == true)
			cout<< numeroDeVertices;
		else
			cin>> numeroDeVertices;
		cout<<"\n Ingrese el número de aristas: ";
		cin>> numeroDeAristas;
		if(numeroDeAristas > 1024){
			banderaNumeroExcedido= true;
			MessageBox(NULL,"Ingrese un número menor de Aristas","Error", MB_OK | MB_ICONWARNING | MB_DEFBUTTON1);
		}
		else
			banderaNumeroExcedido= false;
	}while(banderaNumeroExcedido == true);

    //Realizamos el ingreso del grafo, almacenando las aristas en un arreglo de la estructura con los datos respectivos
    for( int i = 0 ; i < numeroDeAristas; i++ ){
    	clrscr();
    	cout<<"\n ÁRBOL DE EXPANSIÓN MÍNIMA"<< endl;
		cout<<" --------------------------------"<< endl<< endl<< endl;
		cout<<"\n Ingrese los pares de vértices y sus respectivos pesos"<< endl<< endl;
		cout<<" Del vértice: ";
		cin>> arista[i].origen;
		cout<<" Al vertice: ";
		cin>> arista[i].destino;
		cout<<" Su peso es: ";
		cin>> arista[i].peso;
	}
    algoritmoDeKruskal(numeroDeVertices, numeroDeAristas);
    
	getch();
    return 0;
}

void algoritmoDeKruskal(int numeroDeVertices, int numeroDeAristas){
	
	ARISTA AME[ 1024 ];     // Arreglo de aristas del AME encontrado
    int verticeInicial, verticeFinal, pesoDeLaArista;
    int pesoTotalAME = 0;          // Peso total del AME
    int numeroDeAristasAME = 0;     // Número de aristas del AME
    int arrayNodosPadres[ 1024 ];	// Este arreglo contiene el padre del i-esimo nodo
    int contadorDeIteraciones= 0;

    inicializarComponentes( numeroDeVertices, arrayNodosPadres );           // Inicializamos cada componente del grafo
    sort( arista , arista + numeroDeAristas );    // Ordenamos las aristas por su comparador

    for( int i = 0 ; i < numeroDeAristas ; ++i ){     // Recorremos las aristas ya ordenadas por peso
        verticeInicial = arista[ i ].origen;    // Vértice origen de la arista actual
        verticeFinal = arista[ i ].destino;  // Vértice destino de la arista actual
        pesoDeLaArista = arista[ i ].peso;        // Peso de la arista actual

        // Verificamos si estan o no en la misma componente conexa
        if( !comprobarMismaComponenteConexa( verticeInicial , verticeFinal, arrayNodosPadres ) ){  // Evitar ciclos
            pesoTotalAME += pesoDeLaArista;              // Incremento el peso total del AME
            AME[ numeroDeAristasAME++ ] = arista[ i ];  // Agregar al AME la arista actual
            unirComponentesConexas( verticeInicial , verticeFinal, arrayNodosPadres );  // Unión de ambas componentes en una sola
            contadorDeIteraciones++;
        }
    }
    // Si el AME encontrado no posee todos los vértices se mostrará un mensaje de error
    // Para saber si contiene o no todos los vértices, basta con que el número de aristas sea igual al numero de vertices - 1
    clrscr();
    if( numeroDeVertices - 1 != numeroDeAristasAME ){
        MessageBox(NULL,"No existe AME válido para el grafo ingresado, el grafo debe ser conexo","Error", MB_OK | MB_ICONWARNING | MB_DEFBUTTON1);
        return;
    }
    cout<<"\n ÁRBOL DE EXPANSIÓN MÍNIMA"<< endl;
	cout<<" --------------------------------"<< endl<< endl<< endl;
    cout<<"\n\n ----- El AME encontrado contiene las siguientes aristas -----"<< endl<< endl;
    for( int i = 0 ; i < numeroDeAristasAME ; i++ )
        cout<< "\t("<< AME[ i ].origen<< ","<< AME[ i ].destino<< ") : "<< AME[ i ].peso<< endl; 	// Vértice origen , Vértice destino, Peso de la Arista

    cout<<"\n\n El costo mínimo de todas las aristas del AME es de: "<< pesoTotalAME<< endl;
    cout<<" Número de iteraciones: "<< contadorDeIteraciones<< endl<< endl;
}

///////////////////////// INICIO UNIR - ENCONTRAR

// Método de inicialización de componentes
void inicializarComponentes( int numeroDeVertices, int arrayNodosPadres[] ){
    for( int i = 1 ; i <= numeroDeVertices ; i++ )
		arrayNodosPadres[ i ] = i;
}

// Método para encontrar la raíz del vértice actual X
int encontrarRaizVertice( int verticeInicial, int arrayNodosPadres[] ){
    if( verticeInicial == arrayNodosPadres[ verticeInicial ] )
    	return verticeInicial;
    else
    	arrayNodosPadres[ verticeInicial ] = encontrarRaizVertice( arrayNodosPadres[ verticeInicial ], arrayNodosPadres );
}

//Método para unir 2 componentes conexas
void unirComponentesConexas( int verticeInicial , int verticeFinal, int arrayNodosPadres[] ){
    arrayNodosPadres[ encontrarRaizVertice( verticeInicial, arrayNodosPadres ) ] = encontrarRaizVertice( verticeFinal, arrayNodosPadres );
}

//Método que determina si 2 vértices estan o no en la misma componente conexa
bool comprobarMismaComponenteConexa( int verticeInicial , int verticeFinal, int arrayNodosPadres[] ){
    if( encontrarRaizVertice( verticeInicial, arrayNodosPadres ) == encontrarRaizVertice( verticeFinal, arrayNodosPadres ) )
		return true;
    return false;
}
//////////////////////// FIN UNIR - ENCONTRAR
