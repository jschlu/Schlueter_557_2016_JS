#include "render.h"
#include "glm.h"

#define TIMER 25

using namespace std;

GLMmodel* jet = NULL;

void recomputeFrame(int value);

float rotMult = 1.0;
float orbMult = rotMult / 365.2;
float distMult = 300.0;
float distOffset = 1200.0;
float sizeMult = 10.0;
// Index 0 = local rotation
// Index 1 = orbital rotation
float sunRot = 0.0;
float mercuryRot[2] = {0.0, 0.0 };
float venusRot[2] = { 0.0, 0.0 };
float earthRot[2] = { 0.0, 0.0 };
float moonRot[2] = { 0.0, 0.0 };
float marsRot[2] = { 0.0, 0.0 };
float jupiterRot[2] = { 0.0, 0.0 };
float saturnRot[2] = { 0.0, 0.0 };
float uranusRot[2] = { 0.0, 0.0 };
float neptuneRot[2] = { 0.0, 0.0 };

bool isPaused = false;
bool useLocalRotate = true;
bool toScale = true;

GLfloat ambientColor[] = { 0.2f, 0.2f, 0.2f, 1.0f };
GLfloat lightColor0[] = { 0.7f, 0.24f, 0.0f, 1.0f };
GLfloat lightPos0[] = { 0.0f, 0.0f, 0.0f, 1.0f };


void recomputeFrame(int value) {
	if (!isPaused) {
		mercuryRot[1] += orbMult * 4.15;
		venusRot[1] += orbMult * 1.6253;
		earthRot[1] += orbMult;
		//moonRot[1] += orbMult * 13.3773;
		marsRot[1] += orbMult / 1.8812;
		jupiterRot[1] += orbMult / 11.8593;
		saturnRot[1] += orbMult / 29.4277;
		uranusRot[1] += orbMult / 83.7596;
		neptuneRot[1] += orbMult / 163.7459;
	}
	if (!isPaused && useLocalRotate) {
		sunRot += rotMult / 25.1046;
		mercuryRot[0] += rotMult / 58.8954;
		venusRot[0] -= rotMult / 244.0377;
		earthRot[0] += rotMult;

		moonRot[1] += orbMult * 13.3773;
		moonRot[0] += rotMult / 27.4351;

		marsRot[0] += rotMult / 1.0293;
		jupiterRot[0] += rotMult * 2.4141;
		saturnRot[0] += rotMult * 2.2337;
		uranusRot[0] -= rotMult * 1.3895;
		neptuneRot[0] += rotMult * 1.4845;
	}

	glutPostRedisplay();
	glutTimerFunc(TIMER, recomputeFrame, value);
}

// Default constructor
// used for initializing any user defined variables
Render::Render()
{
	// specify a default location for the camera
	eye[0] = 0.0f;
	eye[1] = 0.0f;
	eye[2] = 5000.0f;

	// specify default values to the rotational values in the transformation matrix
	rot[0] = 0.0f;
	rot[1] = 0.0f;
	rot[2] = 0.0f;

	LEFT = 0;
	MIDDLE = 1;
	RIGHT = 2;
	mButton = -1;
	
	view = 3;
	roll = 0.0;
	pitch = 0.0;
	yaw = 0.0;
	moveX = 0.0;
	moveY = 0.0;
	moveZ = 0.0;
	align = 0;
	factor = 3.14159/180;
	turnLeft = false;
	turnRight = false;
}

Render::~Render()
{


}

void Render::init(void)
{
	// clear the background to the color values specified in the parentheses
	glClearColor (1.0, 1.0, 1.0, 0.0);
	glShadeModel (GL_SMOOTH);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);

	glClearDepth(1.0);

	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	// Lighting
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
	
	skybox = gluNewQuadric();
	sun = gluNewQuadric();
	mercury = gluNewQuadric();
	venus = gluNewQuadric();
	earth = gluNewQuadric();
	moon = gluNewQuadric();
	mars = gluNewQuadric();
	jupiter = gluNewQuadric();
	saturn = gluNewQuadric();
	saturnRing = gluNewQuadric();
	uranus = gluNewQuadric();
	neptune = gluNewQuadric();

	Image* image = loadBMP("images/space.bmp");
	skybox_tex = loadTexture(image);
	image = loadBMP("images/sun.bmp");
	sun_tex = loadTexture(image);
	image = loadBMP("images/mercury.bmp");
	mercury_tex = loadTexture(image);
	image = loadBMP("images/venus.bmp");
	venus_tex = loadTexture(image);
	image = loadBMP("images/earth.bmp");
	earth_tex = loadTexture(image);
	image = loadBMP("images/moon.bmp");
	moon_tex = loadTexture(image);
	image = loadBMP("images/mars.bmp");
	mars_tex = loadTexture(image);
	image = loadBMP("images/jupiter.bmp");
	jupiter_tex = loadTexture(image);
	image = loadBMP("images/saturn.bmp");
	saturn_tex = loadTexture(image);
	image = loadBMP("images/saturnRings.bmp");
	saturnRing_tex = loadTexture(image);
	image = loadBMP("images/uranus.bmp");
	uranus_tex = loadTexture(image);
	image = loadBMP("images/neptune.bmp");
	neptune_tex = loadTexture(image);

	// Read an obj file and load it
    if (!jet) {
		jet = glmReadOBJ("data/f-16.obj");
        if (!jet)
			exit(0);
        glmUnitize(jet);
        glmFacetNormals(jet);
        glmVertexNormals(jet, 90.0);
    }

	recomputeFrame(0);
}

void Render::reshape(int w, int h)
{
	glViewport(0,0,(GLsizei) w, (GLsizei) h);
	Wx = w;
	Wy = h;
}

void Render::mouseClick(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN) 
	{
		mOldX = x;
		mOldY = y;
        switch (button)  
		{
			case GLUT_LEFT_BUTTON:
				mButton = LEFT;
				break;
			case GLUT_MIDDLE_BUTTON: 
				mButton = MIDDLE; 
				break;
			case GLUT_RIGHT_BUTTON:
				mButton = RIGHT;
				break;
			default:
				break;
		}
	}
	else if (state == GLUT_UP)
	{
		mButton = -1;
	}
}

void Render::mouseMove(int x, int y)
{
	if (mButton == LEFT) 
	{
		rot[0] -= ((mOldY - y) * 180.0f) / 1000.0f;
		rot[1] -= ((mOldX - x) * 180.0f) / 1000.0f;
		clamp(rot[0], rot[1], rot[2]);
	}
	else if (mButton == MIDDLE) 
	{
		eye[2] -= ((mOldY - y) * 180.0f) / 100.0f;
		clamp(rot[0], rot[1], rot[2]);
	}
	else if (mButton == RIGHT)
	{
		eye[2] -= ((mOldY - y) * 180.0f) / 100.0f;
		clamp(rot[0], rot[1], rot[2]);
	}	 
	mOldX = x; 
	mOldY = y;

	glutPostRedisplay();
}

void Render::keyPos(unsigned char key, int x, int y)
{
	switch (key)
	{
		case 'w':
			moveZ += 2.5f;
			align = 0;
		break;

		case 'a':
			pitch += 5.0;
			if (pitch > 360)
				pitch -= 360;
			break;

		case 's':
			moveZ -= 2.5f;
			break;

		case 'd':
			pitch -= 5.0;
			if (pitch < -360)
				pitch += 360;
			break;

		case 'q':
			yaw += 5.0;
			if (yaw > 360)
				yaw -= 360;
			break;

		case 'e':
			yaw -= 5.0;
			if(yaw < -360)
				yaw += 360;
			break;

		case 'p':
			isPaused = !isPaused;
			break;

		case 't':
			toScale = !toScale;
			if (toScale)
				eye[2] = 5000;
			else
				eye[2] = 900;
			break;

		case 'r':
			useLocalRotate = !useLocalRotate;
			break;

		case '+':
			rotMult *= 2;
			orbMult = rotMult / 365.2;
			break;
		case '-':
			rotMult = rotMult / 2.0;
			orbMult = rotMult / 365.2;
			break;

	    case 27:	// this is the 'Esc' key
			exit (0);

		default:
			break;

		glutPostRedisplay();
	}
}

void Render::specialKeyPos(int key, int x, int y)
{
	switch(key)
	{
		case GLUT_KEY_UP:
			// press the up arrow key to move forward
			//moveX += (2.5)*cos(turn*factor);
			//moveZ -= (2.5)*sin(turn*factor);
			//align = 0;
		break;


		case GLUT_KEY_DOWN:
			// press the down arrow key to move backward

			//moveX -= (2.5)*cos(turn*factor);
			//moveZ += (2.5)*sin(turn*factor);
			//align = 0;
		break;


		case GLUT_KEY_LEFT:
			// press the left arrow key to turn left while moving forward
			//turn += 5.0;
			//moveX += (2.5)*cos(turn*factor);
			//moveZ -= (2.5)*sin(turn*factor);
			//if(turn > 360)
			//	turn -= 360;
			//align = -1;
			//turnLeft = true;
		break;


		case GLUT_KEY_RIGHT:
			// press the right arrow key to turn right while moving forward
			//turn -= 5.0;
			//moveX += (2.5)*cos(turn*factor);
			//moveZ -= (2.5)*sin(turn*factor);
			//if(turn < -360)
			//	turn += 360;
			//align = 1;
		break;

		default:
			break;
	}
	glutPostRedisplay();
}

void Render::display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Specifies which matrix stack is the target for subsequent matrix operations
	// In this example, the projection matrix is set to perspective projection matrix stack
	glMatrixMode(GL_PROJECTION);
	// all matrix values from previous frames set to identity
	glLoadIdentity();
	
	// perspective projection loaded with new values for Wx and Wy updated
	gluPerspective(60, (GLfloat) Wx/(GLfloat) Wy, 1.0, 1000000.0);
	// use glOrtho for a 3D orthogonal projection
	//glOrtho(-100, 100, -100, 100, -100, 100);

	// Applies subsequent matrix operations to the modelview matrix stack.
	glMatrixMode(GL_MODELVIEW);
	// Clears all the previously loaded values in the modelview matrix
	glLoadIdentity();

	// Now, add a bunch of transformation operations
	glTranslatef(-eye[0], -eye[1], -eye[2]);
    glRotatef(rot[0], 1.0f, 0.0f, 0.0f);
    glRotatef(rot[1], 0.0f, 1.0f, 0.0f);
    glRotatef(rot[2], 0.0f, 0.0f, 1.0f);

	// draw planetary system
	drawPlanetarySystem();

	// draw f-16
	drawObjJet();
	
	// this allows opengl to wait for the draw buffer to be ready in the background for the next frame
	// therefore, while the current buffer is being drawn in the current frame, a buffer is set ready to draw on frame+1
	// this call is effective only when GL_DOUBLE is enabled in glutInitDisplayMode in the main function
	// It is recommended to use glutSwapBuffers and double buffering always
	glutSwapBuffers();
}

void Render::drawPlanetarySystem(void)
{
	glEnable(GL_LIGHT1);
	glEnable(GL_TEXTURE_2D);
	drawSkybox();
	drawSun();
	drawMercury();
	drawVenus();
	drawEarth();
	drawMars();
	drawJupiter();
	drawSaturn();
	drawUranus();
	drawNeptune();
	glDisable(GL_LIGHT1);
	glDisable(GL_TEXTURE_2D);
}

void Render::drawSkybox() {
	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHT1);

	GLfloat tmpAmbient[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, tmpAmbient);

	glColor3f(1, 1, 1);
	gluQuadricOrientation(skybox, GLU_INSIDE);
	glPolygonMode(GL_BACK, GL_FILL);
	glPushMatrix();
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, skybox_tex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glRotatef(90, 1, 0, 0);

		gluQuadricTexture(skybox, 1);
		gluSphere(skybox, sizeMult * 10000, 25, 25);
	glPopMatrix();
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);
}

void Render::drawObjJet(void)
{
	glDisable(GL_COLOR_MATERIAL);

	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT0);

	glPushMatrix();
	//	Add transformations to jet
		glRotatef(yaw, 0, 1, 0);
		glRotatef(pitch, 1, 0, 0);
		glRotatef(roll, 0, 0, 1);
		glTranslatef(moveX, moveY, moveZ);

	    glmDraw(jet, GLM_SMOOTH | GLM_MATERIAL);
	glPopMatrix();

	glEnable(GL_COLOR_MATERIAL);

	//glDisable(GL_LIGHT0);
	glColor3f (0.8, 0.2, 0.2);
	glDisable(GL_LIGHT1);
	glEnable(GL_LIGHT0);
}

void Render::drawSun(void)
{
	glEnable(GL_LIGHT0);
	glColor3f(1, 1, 1);
	gluQuadricOrientation(sun, GLU_OUTSIDE);
	glPolygonMode(GL_BACK, GL_FILL);
	glPushMatrix();
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, sun_tex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glRotatef(sunRot, 0, 1, 0);
		glRotatef(90, 1, 0, 0);

		gluQuadricTexture(sun, 1);
		if(toScale)
			gluSphere(sun, sizeMult * 109.0781, 25, 25);
		else
			gluSphere(sun, 75, 25, 25);

	GLfloat lightColor1[] = {1.2f, 0.28f, 0.0f, 1.0f};
    GLfloat lightPos1[] = {0.0f, 0.0f, 0.0f, 1.0f}; //Last parameter '0.0' indicates directional light
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor1);
    glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);

	glPopMatrix();
	glDisable(GL_LIGHT0);
}

void Render::drawMercury()
{
	glColor3f(1, 1, 1);
	gluQuadricOrientation(mercury, GLU_OUTSIDE);
	glPolygonMode(GL_BACK, GL_FILL);
	glPushMatrix();
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, mercury_tex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glRotatef(mercuryRot[1], 0, 1, 0);
		glRotatef(90, 1, 0, 0);
		if(toScale)
			glTranslatef(0.4 * distMult + distOffset, 0, 0);
		else
			glTranslatef(90, 0, 0);
		glRotatef(mercuryRot[0], 0, 0, 1);

		gluQuadricTexture(mercury, 1);
		if (toScale)
			gluSphere(mercury, sizeMult / 2.6145, 20, 20);
		else
			gluSphere(mercury, 10, 20, 20);
	glPopMatrix();

}

void Render::drawVenus()
{
	glColor3f(1, 1, 1);
	gluQuadricOrientation(venus, GLU_OUTSIDE);
	glPolygonMode(GL_BACK, GL_FILL);
	glPushMatrix();
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, venus_tex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glRotatef(venusRot[1], 0, 1, 0);
		glRotatef(90, 1, 0, 0);
		if(toScale)
			glTranslatef(0.8 * distMult + distOffset, 0, 0);
		else
			glTranslatef(135, 0, 0);
			
		glRotatef(177.4, 1, 1, 0);
		glRotatef(venusRot[0], 0, 0, 1);

		gluQuadricTexture(venus, 1);
		if (toScale)
			gluSphere(venus, sizeMult / 1.0537, 22, 22);
		else
			gluSphere(venus, 20, 20, 20);
	glPopMatrix();
}

void Render::drawEarth()
{
	glColor3f(1, 1, 1);
	gluQuadricOrientation(earth, GLU_OUTSIDE);
	glPolygonMode(GL_BACK, GL_FILL);
	glPushMatrix();
		glEnable(GL_TEXTURE_2D);
		
		glBindTexture(GL_TEXTURE_2D, earth_tex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glRotatef(earthRot[1], 0, 1, 0);
		glRotatef(90, 1, 0, 0);
		if (toScale)
			glTranslatef(distMult + distOffset, 0, 0);
		else
			glTranslatef(210, 0, 0);
		glRotatef(23.5, 1, 1, 0);
		glRotatef(earthRot[0], 0, 0, 1);

		gluQuadricTexture(earth, 1);
		if (toScale)
			gluSphere(earth, sizeMult, 20, 20);
		else
			gluSphere(earth, 25, 20, 20);

		// Draw the moon with rotation
		glColor3f(1, 1, 1);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, moon_tex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glRotatef(moonRot[1], 0, 0, 1);
		if (toScale)
			glTranslatef(sizeMult, sizeMult, 0);
		else
			glTranslatef(27, 27, 0);
		glRotatef(moonRot[0], 0, 0, 1);

		gluQuadricTexture(moon, 1);
		if (toScale)
			gluSphere(moon, sizeMult / 3.6708, 18, 18);
		else
			gluSphere(moon, 4, 18, 18);
	glPopMatrix();

}

void Render::drawMars() {
	glColor3f(1, 1, 1);
	gluQuadricOrientation(mars, GLU_OUTSIDE);
	glPolygonMode(GL_BACK, GL_FILL);
	glPushMatrix();
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, mars_tex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glRotatef(marsRot[1], 0, 1, 0);
		glRotatef(90, 1, 0, 0);
		if (toScale)
			glTranslatef(1.65 * distMult + distOffset, 0, 0);
		else
			glTranslatef(290, 0, 0);

		glRotatef(25.2, 1, 1, 0);
		glRotatef(marsRot[0], 0, 0, 1);

		gluQuadricTexture(mars, 1);
		if (toScale)
			gluSphere(mars, sizeMult / 1.8781, 20, 20);
		else
			gluSphere(mars, 20, 20, 20);
	glPopMatrix();
}

void Render::drawJupiter() {
	glColor3f(1, 1, 1);
	gluQuadricOrientation(jupiter, GLU_OUTSIDE);
	glPolygonMode(GL_BACK, GL_FILL);
	glPushMatrix();
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, jupiter_tex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glRotatef(jupiterRot[1], 0, 1, 0);
		glRotatef(90, 1, 0, 0);
		if (toScale)
			glTranslatef(5 * distMult + distOffset, 0, 0);
		else
			glTranslatef(375, 0, 0);
		glRotatef(3.1, 1, 1, 0);
		glRotatef(jupiterRot[0], 0, 0, 1);

		gluQuadricTexture(jupiter, 1);
		if (toScale)
			gluSphere(jupiter, sizeMult * 11.2092, 20, 20);
		else
			gluSphere(jupiter, 35, 20, 20);
	glPopMatrix();
}

void Render::drawSaturn() {
	glColor3f(1, 1, 1);
	gluQuadricOrientation(saturn, GLU_OUTSIDE);
	glPolygonMode(GL_BACK, GL_FILL);
	glPushMatrix();
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, saturn_tex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glRotatef(saturnRot[1], 0, 1, 0);
		glRotatef(90, 1, 0, 0);
		if (toScale)
			glTranslatef(9 * distMult + distOffset, 0, 0);
		else
			glTranslatef(490, 0, 0);
		glRotatef(26.7, 1, 1, 0);
		glRotatef(saturnRot[0], 0, 0, 1);

		gluQuadricTexture(saturn, 1);
		if (toScale)
			gluSphere(saturn, sizeMult * 9.4494, 20, 20);
		else
			gluSphere(saturn, 32, 20, 20);

		// Draw the rings with rotation
		glColor3f(1, 1, 1);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, saturnRing_tex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		gluQuadricTexture(saturnRing, 1);
		if (toScale)
			gluDisk(saturnRing, sizeMult * 11, sizeMult * 18, 25, 25);
		else
			gluDisk(saturnRing, 34, 55, 25, 25);
	glPopMatrix();
}

void Render::drawUranus() {
	glColor3f(1, 1, 1);
	gluQuadricOrientation(uranus, GLU_OUTSIDE);
	glPolygonMode(GL_BACK, GL_FILL);
	glPushMatrix();
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, uranus_tex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glRotatef(uranusRot[1], 0, 1, 0);
		glRotatef(90, 1, 0, 0);
		if (toScale)
			glTranslatef(20 * distMult + distOffset, 0, 0);
		else
			glTranslatef(600, 0, 0);
		glRotatef(97.8, 1, 1, 0);
		glRotatef(uranusRot[0], 0, 0, 1);

		gluQuadricTexture(uranus, 1);
		if (toScale)
			gluSphere(uranus, sizeMult * 4.0074, 20, 20);
		else
			gluSphere(uranus, 30, 20, 20);
	glPopMatrix();
}

void Render::drawNeptune() {
	glColor3f(1, 1, 1);
	gluQuadricOrientation(neptune, GLU_OUTSIDE);
	glPolygonMode(GL_BACK, GL_FILL);
	glPushMatrix();
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, neptune_tex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glRotatef(neptuneRot[1], 0, 1, 0);
		glRotatef(90, 1, 0, 0);
		if (toScale)
			glTranslatef(30 * distMult + distOffset, 0, 0);
		else
			glTranslatef(675, 0, 0);

		glRotatef(28.3, 1, 1, 0);
		glRotatef(neptuneRot[0], 0, 0, 1);

		gluQuadricTexture(neptune, 1);
		if (toScale)
			gluSphere(neptune, sizeMult * 3.8827, 20, 20);
		else
			gluSphere(neptune, 29, 20, 20);
	glPopMatrix();
}

// this is for clamping the numbers between 0 & 360. used for rotation values in the mouse move function
void Render::clamp(float v0, float v1, float v2)
{
	if (v0 > 360 || v0 < -360)
		v0 = 0;
	if (v1 > 360 || v1 < -360)
		v1 = 0;
	if (v2 > 360 || v2 < -360)
		v2 = 0;
}

GLuint Render::loadTexture(Image* image) {
	GLuint textureId;

	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
		image->width, image->height, 0, GL_RGB,
		GL_UNSIGNED_BYTE, image->pixels);

	return textureId;
}

