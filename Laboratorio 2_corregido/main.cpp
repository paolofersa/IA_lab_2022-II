
// #include <iostream>
#include <math.h>

#include "Tablero.h"
// #include "MinMax.h"

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
#define radius 70

#define colorRed glColor3f(1, 0, 0)
#define colorYellow glColor3f(1, 0.843, 0)
#define colorPurple glColor3f(0.502, 0.000, 0.502)
#define colorGreen glColor3f(0, 0.502, 0)
#define colorBlue glColor3f(0, 0, 0.804)
#define colorBlack glColor3f(0, 0, 0)
#define colorWhite glColor3f(1, 1, 1)

#define width 800
#define height 600


#include <stdlib.h>

bool turno = true; // 0 = maquina, 1 = jugador
int ganador = -1;

int profundidad = 0;
MinMax maquina(0);
Tablero tablero(width, height);

void elegirTurno();

void restartGame()
{
	system("cls");
	tablero.reset();
	ganador = -1;
	turno = 1;
	elegirTurno();
}

GLvoid window_key(unsigned char key, int x, int y)
{
	switch (key)
	{
	case KEY_ESC:
		exit(0);
		break;
	case KEY_R: /// Restart
		restartGame();
		break;
	default:
		break;
	}
	glutPostRedisplay();
}

void reshape_cb(int w, int h)
{
	if (w == 0 || h == 0)
		return;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glutKeyboardFunc(&window_key);
	gluOrtho2D(0, w, 0, h);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}



void display_cb()
{
	glClear(GL_COLOR_BUFFER_BIT);
	
	tablero.dibujar();
	
	tablero.dibujarTurnos();
	
	glutSwapBuffers();
}


void checkGanador()
{
	if (!tablero.jugadasRestantes)
	{
		ganador = 2;
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
			if (tablero.casillas[i][j] != 0)
				horMaquina = false; /// horizontal para X
			if (tablero.casillas[i][j] != 1)
				horJugador = false; /// horizontal para O
			
			if (tablero.casillas[j][i] != 0)
				verMaquina = false; /// vertical para X
			if (tablero.casillas[j][i] != 1)
				verJugador = false; /// vertical para O
		}
		if (horJugador or verJugador)
		{
			ganador = 1;
		}
		if (horMaquina or verMaquina)
		{
			ganador = 0;
		}
	}
	/// diagonal
	for (int i = 0; i < 3; ++i)
	{
		if (tablero.casillas[i][i] != 0)
			digMaquina = false; /// diagonal(\) para X
		if (tablero.casillas[i][i] != 1)
			digJugador = false; /// diagonal(\) para O
		
		if (tablero.casillas[i][2 - i] != 0)
			_digMaquina = false; /// diagonal(/) para X
		if (tablero.casillas[i][2 - i] != 1)
			_digJugador = false; /// diagonal(/) para O
	}
	if (digJugador or _digJugador)
	{
		ganador = 1;
	}
	if (digMaquina or _digMaquina)
	{
		ganador = 0;
	}
	
	if (ganador == 0)
		cout << "El ganador es la maquina\n";
	else if (ganador == 1)
		cout << "El ganador es el jugador\n";
}

void OnMouseClick(int button, int state, int x, int y)
{
	// y = height - y
	if ((button == GLUT_LEFT_BUTTON) && state == GLUT_DOWN)
	{
		
		if (tablero.marcarCasilla(x, y, turno))
		{
			tablero.jugadasRestantes--;
			
			if (tablero.jugadasRestantes)
				maquina.checkArbol(tablero);
			
			checkGanador();
		}
		// std::cout << x << " " << y << std::endl;
	}
	glutPostRedisplay();
}

void elegirTurno()
{
	string turn;
	int dif;
	cout << "Elija el turno\n"
		<< "1. Jugador\n"
		<< "2. Maquina\n";
	cin >> turn;
	
	cout << "Elija la profundidad\n";
	cin >> dif;
	profundidad = dif;
	
	maquina.setProf(profundidad);
	if (turn == "2")
		maquina.checkArbol(tablero);
	turno = 1;
}

void initialize()
{
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE); // 255 or 0-1 float
	glutInitWindowSize(width, height);						// width, height de la ventana
	glutInitWindowPosition(100, 100);							// incio ventana
	glutCreateWindow("Ventana OpenGL");						// crea la ventana en base a las anteriores
	glutDisplayFunc(display_cb);									// Funcion que dibuja en la ventana
	glutReshapeFunc(reshape_cb);									// Resize de la ventana
	glutMouseFunc(&OnMouseClick);
	glClearColor(0.f, 0.f, 0.f, 0.f); // blanco en la ventana al limpiar buffer
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	elegirTurno();
	initialize();		// funcion importante
	glutMainLoop(); // ejecuta display
	return 0;
}

