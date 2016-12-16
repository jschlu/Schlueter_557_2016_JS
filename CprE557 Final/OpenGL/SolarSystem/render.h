#ifndef _RENDER_H
#define _RENDER_H

#include <stdlib.h>
#include <math.h>
#include <iostream>
#include "glut.h"
#include "imageloader.h"

class Render
{
public:
	
	//constructor definition
	Render();
	//destructor definition
	~Render();

	// These functions are defined public so they can be called by the main function in main.cpp
	void display(void);
	void init(void);
	void reshape(int w, int h);
	void mouseClick(int button, int state, int x, int y);
	void mouseMove(int x, int y);
	void keyPos(unsigned char key, int x, int y);
	void specialKeyPos(int key, int x, int y);
	
private:
	// These functions are private and cannot be called anywhere outside of render class
	void clamp(float v0, float v1, float v2);
	GLuint loadTexture(Image* image);

	void drawSkybox();
	void drawObjJet(void);
	void drawPlanetarySystem(void);
	void drawSun(void);
	void drawMercury(void);
	void drawVenus(void);
	void drawEarth(void);
	void drawMars();
	void drawJupiter();
	void drawSaturn();
	void drawUranus();
	void drawNeptune();

	// Variables used in the member functions of render class
	float eye[3];
	float rot[3];
	int Wx, Wy;
	int LEFT, MIDDLE, RIGHT;
	int mButton;
	int mOldX, mOldY;
	int view;
	float roll, pitch, yaw;
	float moveX, moveZ, moveY;
	int align;
	float factor;
	float turnLeft, turnRight;
	
	float jetMatrix[16];

	GLUquadricObj *sun, *mercury, *venus, *earth, *moon, *mars,
					*jupiter, *saturn, *saturnRing, *uranus, *neptune, *skybox;
	GLuint sun_tex, mercury_tex, venus_tex, earth_tex, moon_tex,
				mars_tex, jupiter_tex, saturn_tex, saturnRing_tex, uranus_tex, neptune_tex, skybox_tex;
};

#endif
