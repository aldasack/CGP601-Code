////////////////////////////////
// Name: main.h			      //
// Author: Alexander Kocourek //
// Date: 06/02/2017			  //
////////////////////////////////


#include "Defs.h"
#include "GameManager.h"

GameManager* gameManager;
unsigned int windowHeight = 1080;
unsigned int windowWidth = 1920;

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
	GLfloat ambient[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);
	glEnable(GL_COLOR_MATERIAL);
}

void Reshape(int width, int height)
{
	glViewport(0.0f, 0.0f, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(75.0, (GLdouble)width / (GLdouble)height, 1.0, 1000.0);
}

int main(void)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	gameManager = &GameManager::GetInstance();
	
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

	gameManager->InitScene();

	// Main Loop
	glutMainLoop();

	gameManager->Shutdown();
	gameManager = nullptr;
	delete gameManager;

	return EXIT_SUCCESS;
}