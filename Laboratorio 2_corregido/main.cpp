// #include <iostream>
#include <math.h>
#include "Tablero.h"
#include "MinMax.h"

# define KEY_ESC 27
# define KEY_PLUS 43
# define KEY_MINUS 45
# define KEY_ENTER 13
# define KEY_B 98
# define KEY_R 114

#define font18 GLUT_BITMAP_HELVETICA_18
#define font12 GLUT_BITMAP_9_BY_15
#define font24 GLUT_BITMAP_TIMES_ROMAN_24


#define PI 3.14159265358979323846  
#define radius 70

# define colorRed glColor3f(1,0,0)
# define colorYellow glColor3f(1,0.843,0)
# define colorPurple glColor3f(0.502, 0.000, 0.502)
# define colorGreen glColor3f(0,0.502,0)
# define colorBlue glColor3f(0,0,0.804)
# define colorBlack glColor3f(0,0,0)
# define colorWhite glColor3f(1,1,1)

#define width 800
#define height 600

Tablero tablero(width,height);


void reshape_cb (int w, int h) {
	if (w==0||h==0) return;
	glViewport(0,0,w,h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluOrtho2D(0,w,0,h);
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity ();
}




// int tablero[3][3] = {{-1,-1,-1},{-1,-1,-1},{-1,-1,-1}};
bool turno = false; // 0 = maquina, 1 = jugador


void display_cb() {
	glClear(GL_COLOR_BUFFER_BIT);

	tablero.dibujar();

	tablero.dibujarTurnos();

	glutSwapBuffers();
}

void OnMouseClick(int button, int state, int x, int y) {
//y = height - y
	if ( (button == GLUT_LEFT_BUTTON) && state == GLUT_DOWN ) {

		if ( tablero.marcarCasilla(x,y, turno)) {
			
		}
		 std::cout <<x<<" "<<y<<std::endl;
	}
	glutPostRedisplay ();
}

void elegirTurno()
{
	string turn, dif;
	cout << "Elija el turno\n"
		<< "1. Jugador\n"
		<< "2. Maquina\n";
	cin >> turn;
	cout << "Elija la dificultad\n"
		<< "1. Facil\n"
		<< "2. Medio\n"
		<< "3. Dificil\n";
	cin >> dif;
	if (dif == "1")
		profundidad = 3;
	else if (dif == "2")
		profundidad = 6;
	else
		profundidad = 9;
	
	MinMaxTree newTree(profundidad, tablero);
	newTree.createTree(newTree.root, turn);
	vector<int> coord = newTree.returnAnswer();

	tablero.casillas[coord[0]][coord[1]] = turn; 

	maquina.setProf(profundidad);
	if (turn == "2")
		maquina.checkArbol(tablero);
	turno = 1;
}

void initialize() {
	glutInitDisplayMode (GLUT_RGBA|GLUT_DOUBLE );// 255 or 0-1 float
	glutInitWindowSize (width,height); // width, height de la ventana
	glutInitWindowPosition (100,100);//incio ventana
	glutCreateWindow ("Ventana OpenGL");//crea la ventana en base a las anteriores
	glutDisplayFunc (display_cb); // Funcion que dibuja en la ventana
	glutReshapeFunc (reshape_cb);//Resize de la ventana
	glutMouseFunc (&OnMouseClick);
	glClearColor(0.f,0.f,0.f,0.f);//blanco en la ventana al limpiar buffer
}

int main (int argc, char **argv) {
	glutInit (&argc, argv);
	initialize();//funcion importante
	elegirTurno();
	glutMainLoop();//ejecuta display
	return 0;
}