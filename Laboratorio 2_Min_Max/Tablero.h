#ifndef TABLERO_H
#define TABLERO_H

#define KEY_ESC 27
#define KEY_PLUS 43
#define KEY_MINUS 45
#define KEY_ENTER 13
#define KEY_B 98
#define KEY_R 114

#define font18 GLUT_BITMAP_HELVETICA_18
#define font12 GLUT_BITMAP_9_BY_15
#define font24 GLUT_BITMAP_TIMES_ROMAN_24

#define PI 3.14159265358979323846
#define RADIUS 70

#define colorRed glColor3f(1, 0, 0)
#define colorYellow glColor3f(1, 0.843, 0)
#define colorPurple glColor3f(0.502, 0.000, 0.502)
#define colorGreen glColor3f(0, 0.502, 0)
#define colorBlue glColor3f(0, 0, 0.804)
#define colorBlack glColor3f(0, 0, 0)
#define colorWhite glColor3f(1, 1, 1)

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif
#include <iostream>
#include <utility>
#include <iostream>
#include <vector>
#include "Tablero.h"

using namespace std;
struct Tablero
{
	int width;
	int height;
	int jugadasRestantes;
	int casillas[3][3] = {{-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}};
	pair<int, int> centros[3][3];
	
	Tablero();
	Tablero(int, int);
	
	void dibujar();
	
	bool marcarCasilla(int, int, bool &);
	
	void dibujarTurnos();
	
	void drawEquis(pair<int, int> pos);
	void drawHollowCircle(pair<int, int> pos);
	void copy(Tablero &);
	
	void reset();
	
private:
	void marcarCentros();
};

void Tablero::reset()
{
	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; ++j)
			casillas[i][j] = -1;
			
			jugadasRestantes = 9;
}

void Tablero::copy(Tablero &newTablero)
{
	// cout<<newTablero.jugadasRestantes<<endl;
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			casillas[i][j] = newTablero.casillas[i][j];
			jugadasRestantes = newTablero.jugadasRestantes;
		}
	}
}

void Tablero::drawEquis(pair<int, int> pos)
{
	colorGreen;
	float divWidth = width / 3,
		divHeight = height / 3;
	int sizeW = divWidth / 3;
	int sizeH = divHeight / 3;
	glBegin(GL_LINES);
	glVertex2i(pos.first + sizeW, height - (pos.second + sizeH)); // der arriba
	glVertex2i(pos.first - sizeW, height - (pos.second - sizeH)); // izq abajo
	glVertex2i(pos.first - sizeW, height - (pos.second + sizeH)); // izq arriba
	glVertex2i(pos.first + sizeW, height - (pos.second - sizeH)); // der abajo
	glEnd();
}

/// funcion sacada de internet para dibujar un circulo a partir de una linea con muchos vertices
void Tablero::drawHollowCircle(pair<int, int> pos)
{
	colorYellow;
	int i;
	int lineAmount = 100; //# of vertices used to draw circle
	GLfloat twicePi = 2.0f * PI;
	glBegin(GL_LINE_LOOP);
	for (i = 0; i <= lineAmount; i++)
	{
		glVertex2f(
				   pos.first + (RADIUS * cos(i * twicePi / lineAmount)),
				   (height - pos.second) + (RADIUS * sin(i * twicePi / lineAmount)));
	}
	glEnd();
}

Tablero::Tablero()
{
	this->width = 0;
	this->height = 0;
	jugadasRestantes = 9;
}
Tablero::Tablero(int width, int height)
{
	this->width = width;
	this->height = height;
	jugadasRestantes = 9;
	marcarCentros();
}

void Tablero::marcarCentros()
{
	float divWidth = width / 3,
		divHeight = height / 3;
	
	centros[0][0] = make_pair(divWidth / 2, divHeight / 2);
	centros[1][0] = make_pair(divWidth / 2, (divHeight + divHeight * 2) / 2);
	centros[2][0] = make_pair(divWidth / 2, (divHeight * 2 + divHeight * 3) / 2);
	
	centros[0][1] = make_pair((divWidth + divWidth * 2) / 2, divHeight / 2);
	centros[1][1] = make_pair((divWidth + divWidth * 2) / 2, (divHeight + divHeight * 2) / 2);
	centros[2][1] = make_pair((divWidth + divWidth * 2) / 2, (divHeight * 2 + divHeight * 3) / 2);
	
	centros[0][2] = make_pair((divWidth * 2 + divWidth * 3) / 2, divHeight / 2);
	centros[1][2] = make_pair((divWidth * 2 + divWidth * 3) / 2, (divHeight + divHeight * 2) / 2);
	centros[2][2] = make_pair((divWidth * 2 + divWidth * 3) / 2, (divHeight * 2 + divHeight * 3) / 2);
	
	// for (int i=0; i<3; i++)
	// {
	// 	for(int j=0; j<3; j++)
	// 	{
	//       if(i == 0 && j ==0)
	//       centros[i][j] = 123
	
	//   }
	// }
}

void Tablero::dibujarTurnos()
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (casillas[i][j] == 0)
			{
				drawHollowCircle(centros[i][j]);
			}
			if (casillas[i][j] == 1)
			{
				drawEquis(centros[i][j]);
			}
		}
	}
}

// return si alguna casilla ha sido marcada
bool Tablero::marcarCasilla(int x, int y, bool &turno)
{
	float divWidth = width / 3,
		divHeight = height / 3;
	
	if (x < divWidth)
	{
		// pos: 0,0
		if (y < divHeight && casillas[0][0] == -1)
		{
			casillas[0][0] = turno;
			// turno = !turno;
			return 1;
		}
		// pos: 1,0
		else if (y > divHeight && y < divHeight * 2 && casillas[1][0] == -1)
		{
			casillas[1][0] = turno;
			// turno = !turno;
			return 1;
		}
		// pos: 2,0
		else if (y > divHeight * 2 && y < divHeight * 3 && casillas[2][0] == -1)
		{
			casillas[2][0] = turno;
			// turno = !turno;
			return 1;
		}
	}
	else if (x < divWidth * 2)
	{
		// pos: 0,1
		if (y < divHeight && casillas[0][1] == -1)
		{
			casillas[0][1] = turno;
			// turno = !turno;
			return 1;
		}
		// pos: 1,1
		else if (y > divHeight && y < divHeight * 2 && casillas[1][1] == -1)
		{
			casillas[1][1] = turno; // 0 o 1
			// turno = !turno;
			return 1;
		}
		// pos: 2,1
		else if (y > divHeight * 2 && y < divHeight * 3 && casillas[2][1] == -1)
		{
			casillas[2][1] = turno;
			// turno = !turno;
			return 1;
		}
	}
	else if (x <= divWidth * 3)
	{
		// pos: 0,2
		if (y < divHeight && casillas[0][2] == -1)
		{
			casillas[0][2] = turno;
			// turno = !turno;
			return 1;
		}
		// pos: 1,2
		else if (y > divHeight && y < divHeight * 2 && casillas[1][2] == -1)
		{
			casillas[1][2] = turno;
			// turno = !turno;
			return 1;
		}
		
		// pos: 2,2
		else if (y > divHeight * 2 && y < divHeight * 3 && casillas[2][2] == -1)
		{
			casillas[2][2] = turno;
			// turno = !turno;
			return 1;
		}
	}
	return 0;
}

void Tablero::dibujar()
{
	colorWhite;			// elegir un color con el cual pintar
	glLineWidth(5); // Elegir el grosor de la linea
	int padding = 20;
	
	float divWidth = width / 3,
		divHeight = height / 3;
	
	glBegin(GL_LINES);
	// Lineas Verticales
	// 200,20  -  200, 780
	glVertex2i(divWidth, padding);
	glVertex2i(divWidth, height - padding);
	// 600,20 - 600,780
	glVertex2i(divWidth * 2, padding);
	glVertex2i(divWidth * 2, height - padding);
	// Lineas Horizontales
	// 20,200 780,200
	glVertex2i(padding, divHeight);
	glVertex2i(width - padding, divHeight);
	glVertex2i(padding, divHeight * 2);
	glVertex2i(width - padding, divHeight * 2);
	glEnd();
}

struct Node
{
	Tablero tablero;
	int estado, height;
	int idxCadena;
	bool stop = false;
	int utilidad = 0;
	Node *back; /// padre
	vector<Node *> nodes;
	
	Node(int height, Node *back)
	{
		this->height = height;
		this->back = back;
		this->stop = false;
		estado = 0;
		idxCadena = -1;
	}
	
	/// 0=Maquina,1=Jugador
	int calcularUtilidad()
	{
		int jugadorV = 10, MaquinaV = 10;
		// Verficar 3 en raya horizontal y vertical(cruz)
		for (int i = 0; i < 3; ++i)
		{
			bool horJugador, horMaquina;
			bool verJugador, verMaquina; // gano en alguna orientacion
			horJugador = true, horMaquina = true;
			verJugador = true, verMaquina = true;
			for (int j = 0; j < 3; ++j)
			{
				if (tablero.casillas[i][j] == 1)
					horMaquina = false; /// horizontal para X
				if (tablero.casillas[i][j] == 0)
					horJugador = false; /// horizontal para O
				
				if (tablero.casillas[j][i] == 1)
					verMaquina = false; /// vertical para X
				if (tablero.casillas[j][i] == 0)
					verJugador = false; /// vertical para O
			}
			if (horJugador)
				++jugadorV;
			if (horMaquina)
				++MaquinaV;
			if (verJugador)
				++jugadorV;
			if (verMaquina)
				++MaquinaV;
		}
		/// diagonal
		bool digjugador = true, digMaquina = true;
		bool digInversaJugador = true, digInversaMaquina = true;
		for (int i = 0; i < 3; ++i)
		{
			if (tablero.casillas[i][i] == 1)
				digMaquina = false; /// diagonal(\) para X
			if (tablero.casillas[i][i] == 0)
				digjugador = false; /// diagonal(\) para O
			
			if (tablero.casillas[i][2 - i] == 1)
				digInversaMaquina = false; /// diagonal(/) para X
			if (tablero.casillas[i][2 - i] == 0)
				digInversaJugador = false; /// diagonal(/) para O
		}
		if (digjugador)
			++jugadorV;
		if (digMaquina)
			++MaquinaV;
		if (digInversaJugador)
			++jugadorV;
		if (digInversaMaquina)
			++MaquinaV;
		
		utilidad = (MaquinaV * 1) - (jugadorV * 1);
	}
	
	~Node()
	{
		while (!nodes.empty())
		{
			Node *node = nodes.back();
			delete node;
			nodes.pop_back();
		}
	}
};

struct MinMax
{
	int maxHeight;
	Node *root;
	
	void setProf(int maxHeight)
	{
		this->maxHeight = maxHeight;
		root = new Node(0, nullptr);
		createTree(root, 1, maxHeight);
	}
	
	MinMax(int maxHeight)
	{
		this->maxHeight = maxHeight;
		root = new Node(0, nullptr);
		createTree(root, 1, maxHeight);
	}
	
	void createTree(Node *actual, int height, int maxHeight)
	{
		if (maxHeight == 0)
			return;
		
		for (int i = 0; i < 10 - height; ++i)
		{
			Node *node = new Node(height, actual);
			actual->nodes.push_back(node);
			createTree(node, height + 1, maxHeight - 1);
		}
	}
	
	// Verifica si hay una ganador 
	// Si gano la maquina, la utilidad es positiva (Favorable)
	// Si gano el jugador, la utilidad es negativa (Desfavorable)
	// Si hay empate, la utilidad es cero (Favorable)
	void checkWinner(Node *actual)
	{
		
		if (actual->tablero.jugadasRestantes == 0)
		{
			actual->utilidad = 0;// Quiza cambiar a la misma utilidad como cuando gana la maquina
			actual->stop = true;
			return;
		}
		bool horJugador, horMaquina;
		bool verJugador, verMaquina;
		bool digJugador = true, digMaquina = true;
		bool _digJugador = true, _digMaquina = true;
		for (int i = 0; i < 3; ++i)
		{
			horJugador = true, horMaquina = true;
			verJugador = true, verMaquina = true;
			for (int j = 0; j < 3; ++j)
			{
				if (actual->tablero.casillas[i][j] != 0)
					horMaquina = false; /// horizontal para X
				if (actual->tablero.casillas[i][j] != 1)
					horJugador = false; /// horizontal para O
				
				if (actual->tablero.casillas[j][i] != 0)
					verMaquina = false; /// vertical para X
				if (actual->tablero.casillas[j][i] != 1)
					verJugador = false; /// vertical para O
			}
			if (horJugador or verJugador)
			{
				actual->utilidad = -10; // quiza cambiar a menos infinito
				actual->stop = true;
				return;
				// ganador = 1;
			}
			if (horMaquina or verMaquina)
			{
				actual->utilidad = 10; // quiza cambiar a mas infinito
				actual->stop = true;
				return;
				// ganador = 0;
			}
		}
		/// diagonal
		for (int i = 0; i < 3; ++i)
		{
			if (actual->tablero.casillas[i][i] != 0)
				digMaquina = false; /// diagonal(\) para X
			if (actual->tablero.casillas[i][i] != 1)
				digJugador = false; /// diagonal(\) para O
			
			if (actual->tablero.casillas[i][2 - i] != 0)
				_digMaquina = false; /// diagonal(/) para X
			if (actual->tablero.casillas[i][2 - i] != 1)
				_digJugador = false; /// diagonal(/) para O
		}
		if (digJugador or _digJugador)
		{
			actual->utilidad = -10;
			actual->stop = true;
			return;
		}
		if (digMaquina or _digMaquina)
		{
			actual->utilidad = 10;
			actual->stop = true;
			return;
		}
		
		actual->stop = false;
	}
	
	void agregarJugada(Node *node, int turno)
	{
		int pos = 0;
		for (int i = 0; i < 3; ++i)
			for (int j = 0; j < 3; ++j)
				if (node->tablero.casillas[i][j] == -1)
		{
					// copiamos el tablero al hijo
					node->nodes[pos]->tablero.copy(node->tablero);
					node->nodes[pos]->tablero.casillas[i][j] = turno;
					node->nodes[pos]->tablero.jugadasRestantes--;
					
					// Si la dificultad es dificil(profundidad=9), verificar si el jugador puede ganar
					// Porque si el jugador puede ganar en la siguiente jugada, es mejor bloquearlo
					// Por otro lado, si yo(maquina) puedo ganar en la siguiente jugada, es mejor ganar
					if (maxHeight > 6)
						checkWinner(node->nodes[pos]);
					pos++;
		}
	}
	
	// genero arbol con maxima altura como condicion de parada
	//
	void generalArbol(Node *actual, int turno)
	{
		//    Si llegue al nodo hijo    ||       si ya no hay mas jugadas        || Si hay un ganador o empate
		if (actual->height == maxHeight || actual->tablero.jugadasRestantes == 0 || actual->stop)
		{
			actual->calcularUtilidad(); // hoja o final
			return;
		}
		agregarJugada(actual, turno);
		for (int i = 0; i < actual->tablero.jugadasRestantes; ++i)
		{
			if (!actual->nodes[i]->stop) // Si no hay un ganador, seguir creando hijos
				generalArbol(actual->nodes[i], !turno);
		}
		//		cout << "actual->height" << actual->height << endl;
		if (actual->height % 2 == 0) // altura par
			maximo(actual);
		else
			minimo(actual);
	}
	
	void maximo(Node *actual)
	{
		int idxElegido = 0;
		for (int i = 1; i < actual->tablero.jugadasRestantes; ++i)
		{
			if (actual->nodes[i]->utilidad > actual->nodes[idxElegido]->utilidad)
				idxElegido = i;
		}
		actual->utilidad = actual->nodes[idxElegido]->utilidad;
		actual->idxCadena = idxElegido;
	}
	
	void minimo(Node *actual)
	{
		int idxElegido = 0;
		for (int i = 1; i < actual->tablero.jugadasRestantes; ++i)
		{
			if (actual->nodes[i]->utilidad < actual->nodes[idxElegido]->utilidad)
				idxElegido = i;
		}
		actual->utilidad = actual->nodes[idxElegido]->utilidad;
		actual->idxCadena = idxElegido;
	}
	
	void checkArbol(Tablero &tablero)
	{
		root->tablero.copy(tablero);
		root->stop = false;
		generalArbol(root, 0);															// genera el arbol para el tablero actual
		Node *jugadaMaquina = root->nodes[root->idxCadena]; // caminoi a elegir
		//cout << root->idxCadena << endl;
		
		tablero.copy(jugadaMaquina->tablero); // copiamos el tablero elegido por la maquina
	}
};

#endif

