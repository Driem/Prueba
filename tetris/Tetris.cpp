#include <cstdlib>
#include <ctime>
#include "miniwin.h"
#include <sstream>
#include <string>
using namespace std;
using namespace miniwin;

const int TAM = 25;
const int Filas = 20;
const int Columnas = 10;

typedef int Tablero[Columnas][Filas];

class Coord{
public:
	
int x, y;
};

class Pieza{
	
	public:
	Coord orig;// bloque central
	Coord perif[3]; // bloques perifericos
	int color;
	
	Coord posicion(int n) const; // n entre 0 y 3 (0 = central
};

Coord Pieza :: posicion(int n) const{
	Coord ret = {orig.x, orig.y};
	if(n != 0){
		ret.x += perif[n-1].x;
		ret.y += perif[n-1].y;
		
	}
		return ret;
}



void cuadrado(int x, int y)
{

rectangulo_lleno(
				 20 + 1 + x * TAM,
				 20 + 1 + y * TAM,
				 20 +  x * TAM + TAM,
				 20 + y * TAM + TAM);
}

void pinta_pieza(const Pieza& P){
	color(P.color);

	for(int i = 0; i < 4; i++){
		Coord c = P.posicion(i);
		cuadrado(c.x,c.y);
	}
}

Coord rota_derecha(const Coord& c){
	Coord ret = {c.y,-c.x};
	return ret;
}

/*Coord rota_izquierda(const Coord& c){
	Coord ret = {-c.y,c.x};
	return ret;
}*/ 

void rota_derecha(Pieza& P){
	for(int i = 0; i < 3; i++){
		P.perif[i] = rota_derecha(P.perif[i]);
	}
}

/*void rota_izquierda(Pieza& P){
	for(int i = 0; i < 3; i++){
		P.perif[i] = rota_izquierda(P.perif[i]);
	}
}*/

void tablero_vacia(Tablero& T){
	for(int i = 0; i < Columnas; i++){
 		for(int j = 0; j < Filas; j++){
			T[i][j] = NEGRO; // Casilla Vacia
		}
	}
}

void tablero_pinta(const Tablero& T){
	for(int i = 0; i < Columnas; i++){
		for(int j = 0; j < Filas; j++){
			color(T[i][j]);
			cuadrado(i,j);
		}
	}
}



void tablero_incrusta_pieza(Tablero& T, const Pieza& P){
for(int i = 0; i < 4;i++){
	Coord c = P.posicion(i);
	T[c.x][c.y] = P.color;
	
	}
}

bool tablero_colision(Tablero& T, const Pieza& P){
	
	
	for(int i = 0; i < 4;i++){
		Coord c = P.posicion(i);
		//comprobar limites
	if(c.x < 0 || c.x >= Columnas){
		return true;
	}	
	if(c.y < 0 || c.y >= Filas){
		return true;
	}
	 // Mirar basura
	if(T[c.x][c.y] != NEGRO){
		return  true;
		} 
	}
	return false;
}

const Coord perifs[7][3] ={
{{1,0},{ 0,1 },{ 1,1 }}, // cuadrado
{{1,0},{ -1,1 },{ 0,1 }},// S
{{0,1},{ 1,1 },{ -1,0 }},// 2
{{0,1},{ 0,-1 },{ 1,1 }},//L
{{0,1},{ 0,-1 },{ -1,1 }},//7
{{-1,0},{ 1,0 },{ 0,1 }},// T
{{0,1},{ 0,-1 },{ 0,2 }},// Estaca

};

void pieza_nueva(Pieza& P){
	P.orig.x = 12;
	P.orig.y = 2;
	P.color = 1 + rand() % 6;
	//Pieza al azar
	int r = rand() % 7;
	for(int i = 0; i < 3; i++){
		P.perif[i] = perifs[r][i];
  }
}

bool tablero_fila_llena(const Tablero& T, int fila){
	for(int i = 0; i < Columnas; i++){
		if(T[i][fila] == NEGRO) return false;
	}
	return true;
}

void tablero_colapsa(Tablero& T, int fila){
	// Copiar de abajo a arriba
	for(int j = fila; j > 0; j--){
		for(int i = 0; i < Columnas; i++){
			T[i][j] = T[i][j-1];
		}
	}
	//Vaciar Arriba
	for(int i = 0; i < Columnas; i++){
		T[i][0] = NEGRO;
	}
}

int tablero_cuenta_lineas(Tablero& T){
	int fila = Filas-1, cont = 0;
	while(fila >= 0){
		if(tablero_fila_llena(T,fila)){ // 17
			tablero_colapsa(T,fila);
			cont++;
		}else{
		fila--;	
			
		}
		
	}
	return cont;
}

string Make_string(int puntos){
	stringstream sout;
	sout << puntos;
	return sout.str();
	
}

const int puntos_limite [10] = {
25,50, 75, 100, 125, 150, 175 ,200 , 225, 250 
};

const int tics_nivel [10] = {
33,25, 20, 18, 16, 14, 12 , 10 , 8, 2 
};



void repinta(const Tablero& T, const Pieza& p, const Pieza& sig, int puntos, int nivel){
	const int ancho = TAM * Columnas;
	const int alto = TAM * Filas;
	borra();
	tablero_pinta(T);
	color_rgb(128, 128, 128);
	linea(20,20,20, 20+ alto);
	linea(20, 20 + alto,
		  20 + ancho, 20 + alto);
	linea(20 + ancho,20 + alto,
	      20 + ancho,20);
	texto(40 + ancho, 20, "Pieza siguiente");
	texto(40 + ancho, 150, "Nivel");
	texto(40 + ancho, 250, "Puntos");
	color(BLANCO);
	texto(40 + ancho, 270,Make_string(puntos));
	texto(40 + ancho, 170,Make_string(nivel + 1));
	pinta_pieza(p);
	pinta_pieza(sig);
	refresca();	
	
}

void game_over(){
	 color(BLANCO);
	texto(140,240,"GAME OVER");
	refresca();
	espera(1000);
	vcierra();
}

int main()
{
	vredimensiona(TAM * Columnas + 220, TAM * Filas + 100);
	srand(time(0)); // inicializar los numeros al azar(Poner la semilla YEAH)
	
	int tic = 0, puntos = 0, nivel = 0;
	
	Tablero T;
	tablero_vacia(T);
	Pieza c, sig;  
	pieza_nueva(c);
	pieza_nueva(sig);
	c.orig.x = 5;
	
	
	repinta(T,c,sig, puntos ,nivel);
	
 	int t = tecla();
	while(t != ESCAPE){
		//0. copiar la posicion actual
		Pieza copia = c;
		
		if(t == NINGUNA && tic > tics_nivel[nivel]){
			tic = 0;
			t = ABAJO;
		}
		
		if(t == int ('N')){
		nivel++;	
		}
		
		// 1. Prueba el movimiento"
		if(t == ABAJO ){
			c.orig.y++;
		} else if(t == ARRIBA ) {
			rota_derecha(c);
		} else if(t == DERECHA ) {
			c.orig.x++;
		} else if(t == IZQUIERDA ) {
			c.orig.x--;
		}
	//2. Mirar si hay colision
	if(tablero_colision(T,c)){
		 c = copia;	
		if(t == ABAJO){
			tablero_incrusta_pieza(T,c);
			int cont =tablero_cuenta_lineas(T);
			puntos += cont * cont;
			if(puntos > puntos_limite[nivel]){
				nivel++;
			}
			c = sig;
			pieza_nueva(sig);
			c.orig.y = 1;
			c.orig.x = 5;
			if(tablero_colision(T,c)){
				game_over();		
			}
		}
	}
	
	
	//repinta
	if(t != NINGUNA){
	repinta(T,c,sig,puntos, nivel);
}
 espera(30);
 tic++;
t = tecla();
	
	}
	vcierra();
	return 0;
}
