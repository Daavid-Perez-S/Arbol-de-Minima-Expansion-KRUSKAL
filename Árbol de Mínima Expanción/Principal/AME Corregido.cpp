/**********************************************
 *** Algoritmo: Árbol de Mínima Expanción
 *** Programa hecho por: David Pérez Sánchez  3ª "A"
 *** UP-Chiapas  I.D.S.
 *** Completado el 26 de Julio de 2017
 *********************************************/
#include <iostream>
#include <conio2.h>
#include <windows.h>
using namespace std;

//Estructura arista ( ARISTA )
struct ARISTA{
    int origen;     // Vértice origen
    int destino;    // Vértice destino
    int peso;       // Peso entre el vértice origen y destino
    bool aristaBloqueada;	// Define si una arista esta obligada a incluirse en el AME
} *conjuntoNoConectado= NULL;      //Arreglo de aristas para el uso en Kruskal

// Prototipos de Funciones

void algoritmo(int, int, ARISTA [], bool, int);
void inicializarComponentes(int, int []);
int encontrarRaizVertice(int, int []);
void unirComponentesConexas(int, int, int []);
bool comprobarMismaComponenteConexa(int, int, int []);
void ordenarEstructuraDeDatos(int, ARISTA [], int);
bool encontrarRaizVerticeByME(bool, int, ARISTA  []);
void imprimirArreglo(ARISTA [], int);

int main(){
	
    setlocale(LC_ALL,"spanish");
	system("title Árbol de Expansión Mínima");
	int numeroDeVertices, numeroDeAristas, verticeInicial, verticeFinal, pesoDeLaArista, numeroDeAristasBloqueadas, i;
	bool banderaAristasBloqueadas= false;
	bool banderaNumeroErroneo= false;	
	
	do{
		clrscr();
		cout<<"\n ÁRBOL DE EXPANSIÓN MÍNIMA"<< endl;
		cout<<" --------------------------------"<< endl<< endl<< endl;
		cout<<"\n Ingrese el número de vertices o nodos: ";
		cin>> numeroDeVertices;
		cout<<"\n Ingrese el número de aristas: ";
		cin>> numeroDeAristas;
		if(numeroDeVertices <=1 || numeroDeAristas <=0){
			banderaNumeroErroneo= true;
			MessageBox(NULL,"Datos incorrectos","Error", MB_OK | MB_ICONWARNING | MB_DEFBUTTON1);
		}else
			banderaNumeroErroneo= false;
	}while(banderaNumeroErroneo);
	conjuntoNoConectado= new ARISTA[numeroDeAristas];	// Asignación de tamaño del arreglo conjunto no conectado
	clrscr();
	
    //Realizamos el ingreso del grafo, almacenando las aristas en un arreglo de la estructura con los datos respectivos
    if(MessageBox(NULL,"Existen aristas obligatorias para insertar en el AME ??","Aviso", MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2) == IDYES ){
		
		cout<<"\n\n\n Ingrese el número de aristas obligatorias que hay: ";
		cin>> numeroDeAristasBloqueadas;
		banderaAristasBloqueadas= true;
	   	
		   for( i = 0 ; i < numeroDeAristasBloqueadas; i++ ){
	    	clrscr();
		   	cout<<"\n ÁRBOL DE EXPANSIÓN MÍNIMA"<< endl;
			cout<<" --------------------------------"<< endl<< endl<< endl;
			cout<<"\n Ingrese los pares de vértices y sus respectivos pesos"<< endl<< endl;
			cout<<" Del vértice: ";
			cin>> conjuntoNoConectado[i].origen;
			cout<<" Al vertice: ";
			cin>> conjuntoNoConectado[i].destino;
			cout<<" Su peso es: ";
			cin>> conjuntoNoConectado[i].peso;
			conjuntoNoConectado[i].aristaBloqueada= true;
		}
	}else{
	    for( int j = 0 ; j < numeroDeAristas; j++ ){
			clrscr();
			cout<<"\n ÁRBOL DE EXPANSIÓN MÍNIMA"<< endl;
			cout<<" --------------------------------"<< endl<< endl<< endl;
			cout<<"\n Ingrese los pares de vértices y sus respectivos pesos"<< endl<< endl;
			cout<<" Del vértice: ";
			cin>> conjuntoNoConectado[j].origen;
			cout<<" Al vertice: ";
			cin>> conjuntoNoConectado[j].destino;
			cout<<" Su peso es: ";
			cin>> conjuntoNoConectado[j].peso;
			conjuntoNoConectado[j].aristaBloqueada= false;
		}
	}
	if( banderaAristasBloqueadas== true && numeroDeAristasBloqueadas < numeroDeAristas){
		
		clrscr();
		gotoxy(28,15);
		cout<<"Ingrese las aristas restantes del grafo...";
		delay(2000);
		
	    for( int k = i ; k < numeroDeAristas; k++ ){
			clrscr();
			cout<<"\n ÁRBOL DE EXPANSIÓN MÍNIMA"<< endl;
			cout<<" --------------------------------"<< endl<< endl<< endl;
			cout<<"\n Ingrese los pares de vértices y sus respectivos pesos"<< endl<< endl;
			cout<<" Del vértice: ";
			cin>> conjuntoNoConectado[k].origen;
			cout<<" Al vertice: ";
			cin>> conjuntoNoConectado[k].destino;
			cout<<" Su peso es: ";
			cin>> conjuntoNoConectado[k].peso;
			conjuntoNoConectado[k].aristaBloqueada= false;
		}
	}
    algoritmo(numeroDeVertices, numeroDeAristas, conjuntoNoConectado, banderaAristasBloqueadas, i);
	getch();
    return 0;
}

void algoritmo(int numeroDeVertices, int numeroDeAristas, ARISTA conjuntoNoConectado[], bool banderaAristasBloqueadas, int indiceAuxiliar){
	
	ARISTA AME[ numeroDeAristas ];     // Arreglo de aristas del AME (conjunto CONECTADO)
    int verticeInicial, verticeFinal, pesoDeLaArista;
    int pesoTotalAME = 0;          // Peso total del AME
    int numeroDeAristasAME = 0;     // Número de aristas del AME
    int arrayNodosPadres[ numeroDeVertices ];	// Este arreglo contiene el padre del i-esimo nodo
    int contadorDeIteraciones= 0;
	
    inicializarComponentes( numeroDeVertices, arrayNodosPadres );           // Inicializamos cada componente del grafo
    	
    if(banderaAristasBloqueadas)
    	ordenarEstructuraDeDatos(numeroDeAristas, conjuntoNoConectado, indiceAuxiliar);    // Ordenamos las aristas por su peso
    else
    	ordenarEstructuraDeDatos(numeroDeAristas, conjuntoNoConectado, 0);    // Ordenamos las aristas por su peso

    for( int i = 0 ; i < numeroDeAristas ; i++ ){     // Recorremos las aristas ya ordenadas por peso
        verticeInicial = conjuntoNoConectado[ i ].origen;    // Vértice origen de la arista actual
        verticeFinal = conjuntoNoConectado[ i ].destino;  // Vértice destino de la arista actual
        pesoDeLaArista = conjuntoNoConectado[ i ].peso;        // Peso de la arista actual

        // Verificamos si estan o no en la misma componente conexa
        if( !comprobarMismaComponenteConexa( verticeInicial, verticeFinal, arrayNodosPadres ) ){  // Evitar ciclos
        
            pesoTotalAME += pesoDeLaArista;              // Incremento el peso total del conjunto conectado
            AME[ numeroDeAristasAME++ ] = conjuntoNoConectado[ i ];  // Agregar al conjunto conectado
            unirComponentesConexas( verticeInicial , verticeFinal, arrayNodosPadres );  // Unión de ambas componentes en una sola
            
            // Borrar arista del conjunto NO conectado
			conjuntoNoConectado[i].origen= 0;
            conjuntoNoConectado[i].destino= 0;
            conjuntoNoConectado[i].peso= 0;
            conjuntoNoConectado[i].aristaBloqueada= NULL;
            
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
    
    delete[] conjuntoNoConectado;
}


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

// Método para ordenar los datos de la estructura ARISTA
void ordenarEstructuraDeDatos(int numeroDeAristas, ARISTA conjuntoNoConectado[], int indiceAuxiliar){ // Método burbuja para ordenar
	
    int bandera;
    ARISTA aux;
    
	for(int i=indiceAuxiliar+1; i<numeroDeAristas; i++){
		bandera=0;         // Iniciamos la bandera en 0
	     for(int j=numeroDeAristas-1; j>=i; j--){
	        if(conjuntoNoConectado[j-1].peso > conjuntoNoConectado[j].peso){
	            aux=conjuntoNoConectado[j];
	            conjuntoNoConectado[j]=conjuntoNoConectado[j-1];
	            conjuntoNoConectado[j-1]=aux;
	            bandera=1; // Si hubo cambio cambiamos la bandera a 1
	        }
	    }
	    if (bandera==0)
	        break; // Si no hubo cambios entonces salir del for
	}
}

void imprimirArreglo(ARISTA arreglo[], int numeroDeAristas){
	
	cout<<"\n\n";
	for(int i=0; i<numeroDeAristas; i++){
		cout<< arreglo[i].origen<<","<< arreglo[i].destino<<","<< arreglo[i].peso<<","<< arreglo[i].aristaBloqueada<< endl;
	}
	cout<<"\n\n";
}
