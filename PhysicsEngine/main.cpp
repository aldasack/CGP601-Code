////////////////////////////////
// Name: main.h			      //
// Author: Alexander Kocourek //
// Date: 06/02/2017			  //
////////////////////////////////

#include <memory>

#include "Defs.h"
#include "GameManager.h"

GameManager* gameManager;
unsigned int windowHeight = 768;
unsigned int windowWidth = 1024;

void Display()
{
	gameManager->Loop();
}

void KeyboardHandle(unsigned char key, int x, int y)
{
	gameManager->KeyboardHandle(key, x, y);
}

void InitRendering()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	GLfloat diffuse[] = { 1.0, 1.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glEnable(GL_COLOR_MATERIAL);
}

void Reshape(int width, int height)
{
	glViewport(0.0f, 0.0f, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(75.0, (GLdouble)width / (GLdouble)height, 1.0, 100.0);
}

int main(void)
{
	gameManager = new GameManager();
	
	// Initialize GLUT
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(windowWidth, windowHeight);

	// Create Window
	glutCreateWindow("Physics");
	InitRendering();

	// Handle Functions
	glutDisplayFunc(Display);
	glutIdleFunc(Display);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(KeyboardHandle);

	gameManager->InitEngine();

	// Main Loop
	glutMainLoop();
	
	
	//int exitCode = gameManager->Run();

	gameManager->Shutdown();
	gameManager = nullptr;
	delete gameManager;

	return EXIT_SUCCESS;
}