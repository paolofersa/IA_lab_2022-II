#ifndef TABLERO_H
#define TABLERO_H


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
#define RADIUS 70

# define colorRed glColor3f(1,0,0)
# define colorYellow glColor3f(1,0.843,0)
# define colorPurple glColor3f(0.502, 0.000, 0.502)
# define colorGreen glColor3f(0,0.502,0)
# define colorBlue glColor3f(0,0,0.804)
# define colorBlack glColor3f(0,0,0)
# define colorWhite glColor3f(1,1,1)

#ifdef __APPLE__
# include <OpenGL/gl.h>
# include <OpenGL/glu.h>
# include <GLUT/glut.h>
#else
# include <GL/gl.h>
# include <GL/glu.h>
# include <GL/glut.h>
#endif
#include <iostream>
#include <utility>

using namespace std;
struct Tablero{
  int width;
  int height; 
  int casillas[3][3] = {{-1,-1,-1},{-1,-1,-1},{-1,-1,-1}};
  pair<int,int> centros[3][3];

  Tablero(int, int);

  void dibujar();

  bool marcarCasilla(int,int, bool&);

  void dibujarTurnos();
  
  void drawEquis(pair<int,int> pos);
  void drawHollowCircle (pair<int,int> pos);

  private:
    void marcarCentros();
};

void Tablero::drawEquis(pair<int,int> pos)
{
	colorGreen;
  float divWidth = width / 3,
  divHeight = height / 3;
  int sizeW = divWidth /3;
  int sizeH = divHeight/3;
	glBegin (GL_LINES);
    glVertex2i(pos.first + sizeW, height - (pos.second + sizeH)); // der arriba
    glVertex2i(pos.first - sizeW, height - (pos.second - sizeH));// izq abajo
    glVertex2i(pos.first - sizeW, height - (pos.second + sizeH)); // izq arriba
    glVertex2i(pos.first + sizeW, height - (pos.second - sizeH)); // der abajo
	glEnd ();
}

/// funcion sacada de internet para dibujar un circulo a partir de una linea con muchos vertices
void Tablero::drawHollowCircle (pair<int,int> pos)
{
	colorYellow;
	int i;
	int lineAmount = 100; //# of vertices used to draw circle
	GLfloat twicePi = 2.0f * PI;
	glBegin (GL_LINE_LOOP);
		for ( i = 0; i <= lineAmount; i++ ) {
			glVertex2f (
						pos.first + (RADIUS * cos (i * twicePi / lineAmount)),
						(height - pos.second ) + (RADIUS * sin (i * twicePi / lineAmount))
						);
		}
	glEnd ();
}

Tablero::Tablero(int width, int height) {
  this->width = width;
  this->height = height;
  marcarCentros();
}

void Tablero::marcarCentros() {
  float divWidth = width / 3,
  divHeight = height / 3;
  
  centros[0][0] = make_pair(divWidth/2, divHeight/2);
  centros[1][0] = make_pair(divWidth/2, (divHeight + divHeight*2)/2);
  centros[2][0] = make_pair(divWidth/2, (divHeight*2 + divHeight*3)/2);
  
  centros[0][1] = make_pair((divWidth+divWidth*2)/2, divHeight/2);
  centros[1][1] = make_pair((divWidth+divWidth*2)/2, (divHeight + divHeight*2)/2);
  centros[2][1] = make_pair((divWidth+divWidth*2)/2, (divHeight*2 + divHeight*3)/2);
  
  centros[0][2] = make_pair((divWidth*2+divWidth*3)/2, divHeight/2);
  centros[1][2] = make_pair((divWidth*2+divWidth*3)/2, (divHeight + divHeight*2)/2);
  centros[2][2] = make_pair((divWidth*2+divWidth*3)/2, (divHeight*2+ divHeight*3)/2);
  

  // for (int i=0; i<3; i++)
	// {
	// 	for(int j=0; j<3; j++)
	// 	{
  //       if(i == 0 && j ==0)
  //       centros[i][j] = 123
            
  //   }
  // }
}

void Tablero::dibujarTurnos() {
	for (int i=0; i<3; i++)
	{
		for(int j=0; j<3; j++)
		{
			if (!casillas[i][j])
			{
				drawHollowCircle(centros[i][j]);
			}
			if(casillas[i][j] == 1)
			{
				drawEquis(centros[i][j]);
			}
		}
	}
}

// return si alguna casilla ha sido marcada
bool Tablero::marcarCasilla(int x, int y, bool  &turno) {
	float divWidth = width / 3,
		divHeight = height / 3;
	
	if(x < divWidth)
	{
		// pos: 0,0
		if(y < divHeight && casillas[0][0] == -1){
		casillas[0][0] = turno;
		turno = !turno;
		return 1;
		}
		// pos: 1,0
		else if(y > divHeight && y< divHeight*2 && casillas[1][0] == -1)
		{
		casillas[1][0] = turno;
		turno = !turno;
		return 1;
		}
		// pos: 2,0
		else if( y> divHeight*2 && y< divHeight*3 && casillas[2][0] == -1)
		{
		casillas[2][0] = turno;
		turno = !turno;
		return 1;	
		}
	}	
	else if( x < divWidth*2 )
	{
		// pos: 0,1
		if(y < divHeight && casillas[0][1] == -1)
		{
			casillas[0][1] = turno;
			turno = !turno;
			return 1;
		}
		// pos: 1,1
		else if(y > divHeight && y< divHeight*2 && casillas[1][1] == -1)
		{	
			casillas[1][1] = turno; // 0 o 1
			turno = !turno;
			return 1;				
		}
		// pos: 2,1
		else if(y> divHeight*2 && y< divHeight*3 && casillas[2][1] == -1)
		{
			casillas[2][1] = turno;
			turno = !turno;
			return 1;	
		}

	}
	else if ( x <= divWidth*3)
	{
    // pos: 0,2
		if(y < divHeight  && casillas[0][2] == -1 ){
			casillas[0][2] = turno;
			turno = !turno;
			return 1;
		}
    // pos: 1,2
		else if(y > divHeight && y< divHeight*2 && casillas[1][2] == -1)
		{
			casillas[1][2] = turno;
			turno = !turno;
			return 1;				
		}
		
    	// pos: 2,2
		else if(y> divHeight*2 && y< divHeight*3 && casillas[2][2] == -1)
		{
			casillas[2][2] = turno;
			turno = !turno;
			return 1;	
		}
	}
	return 0;

}

void Tablero::dibujar()
{
	colorWhite;  // elegir un color con el cual pintar
	glLineWidth(5); // Elegir el grosor de la linea
	int padding = 20;
	
	float divWidth = width / 3,
		divHeight = height / 3;
	
	glBegin(GL_LINES);
	// Lineas Verticales
	// 200,20  -  200, 780
	glVertex2i(divWidth,padding); glVertex2i(divWidth,height-padding);
	// 600,20 - 600,780
	glVertex2i(divWidth*2,padding); glVertex2i(divWidth*2,height-padding);
	// Lineas Horizontales
	// 20,200 780,200
	glVertex2i(padding,divHeight); glVertex2i(width-padding,divHeight);
	glVertex2i(padding,divHeight*2); glVertex2i(width-padding,divHeight*2);
	glEnd();
}
#endif
