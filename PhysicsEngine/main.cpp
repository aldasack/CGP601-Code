////////////////////////////////
// Name: main.h			      //
// Author: Alexander Kocourek //
// Date: 06/02/2017			  //
////////////////////////////////


#include <chrono>
#include <iostream>
#include <Windows.h>
#include <vector>
#include <cstdlib>
#include <ctime>

#include "Defs.h"
#include "Sphere.h"

GLfloat diffuse[] = { 1.0, 1.0, 1.0 };
Sphere* sphere = nullptr;
std::vector<Sphere*> objects;
bool isRunning = false;
bool isRendering = false;

unsigned int windowHeight = 768;
unsigned int windowWidth = 1024;

std::chrono::high_resolution_clock g_timer;
using ms = std::chrono::duration<float, std::milli>;

auto oldTime = g_timer.now();
auto newTime = g_timer.now();
auto loopOldTime = g_timer.now();
auto loopNewTime = g_timer.now();

double GameTime = 0.0;
// in seconds
float TargetFrameTime = (1.0f / 60.0f);
float g_deltaTime = 0.0f;
int g_ObjectCount = 10;

void drawText(Math::Vector2 position, Math::Vector3 color, std::string text);


void initRendering()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glEnable(GL_COLOR_MATERIAL);
}

void PlaneDraw()
{
	glLoadIdentity();
	glColor3f(1.0f, 1.0f, 1.0f);
	
	glBegin(GL_QUADS);
		glVertex3f(-100.0f, -1.0f, -100.0f);
		glVertex3f(-100.0f, -1.0f, 100.0f);
		glVertex3f(100.0f, -1.0f, 100.0f);
		glVertex3f(100.0f, -1.0f, -100.0f);
	glEnd();
}

void KeyboardHandle(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27 : // Escape
		isRunning = false;
		// exit program, only way using GLUT
		std::exit(EXIT_SUCCESS);
		break;
		
	case 32 : // Space
		isRunning = !isRunning;
		break;

	case 114 : // R
		isRendering = !isRendering;
		break;
	}
}

void Reshape(int width, int height)
{
	glViewport(0.0f, 0.0f, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(75.0, (GLdouble) width / (GLdouble) height, 1.0, 100.0);
}

void Intitialize()
{
	Math::Vector3 v;
	v.x = 0.0f;
	v.y = 5.5f;
	v.z = -5.0;

	for (int i = 0; i < g_ObjectCount; i++)
	{
		v.x = static_cast<float>(rand()) / static_cast<float>(RAND_MAX / 10.0f);
		v.y = static_cast<float>(rand()) / static_cast<float>(RAND_MAX / 10.0f);
		//v.z = static_cast<float>(rand()) / static_cast<float>(RAND_MAX / 10.0f);
		objects.push_back(new Sphere(v));
	}
}

void Draw()
{
	glMatrixMode(GL_MODELVIEW);
	glClearDepth(1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (isRendering)
	{
		for (int i = 0; i < objects.size(); i++)
		{
			objects[i]->Draw();
		}
		PlaneDraw();
	}

	Math::Vector2 pos{ 10.0f, 750.0f };
	Math::Vector3 color{ 1.0f, 1.0f ,1.0f };
	float fps = 1.0f / g_deltaTime;
	std::string text = "FPS: " + std::to_string(fps);
	text = text.substr(0, 7);
	drawText(pos, color, text);
	pos.y = 720.0f;
	text = "Spheres count: " + std::to_string(g_ObjectCount);
	drawText(pos, color, text);

	glutSwapBuffers();
	//glutPostRedisplay();
}

void Update()
{
	for (int i = 0; i < objects.size(); i++)
	{
		objects[i]->Update(TargetFrameTime);
	}
	//sphere->Update(TargetFrameTime);
}

void drawText(Math::Vector2 position, Math::Vector3 color, std::string text)
{
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, windowWidth, 0, windowHeight, -1.0f, 1.0f);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glPushAttrib(GL_DEPTH_TEST);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glRasterPos2i(position.x, position.y);
	for (int i = 0; i < text.size(); i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, text[i]);
	}
	glPopAttrib();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
}

void Display()
{
	if (!isRunning)
		return;
	loopOldTime = loopNewTime;
	loopNewTime = g_timer.now();
	g_deltaTime = std::chrono::duration_cast<ms>(loopNewTime - loopOldTime).count() * 0.001f;

	oldTime = g_timer.now();

	Draw();
	Update();

	newTime = g_timer.now();
	float dt = std::chrono::duration_cast<ms>(newTime - oldTime).count();
	dt *= 0.001f;
	if (dt < TargetFrameTime)
	{
		float wasteTime = (TargetFrameTime - dt) * 1000.0f;
		Sleep(wasteTime);
	}
}

int main(void)
{
	// Initialize GLUT
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(windowWidth, windowHeight);

	// Create Window
	glutCreateWindow("Physics");
	initRendering();

	// Handle Functions
	glutDisplayFunc(Display);
	glutIdleFunc(Display);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(KeyboardHandle);

	// Initialize 3DObjects
	Intitialize();
	isRunning = true;
	isRendering = true;
	srand(static_cast<unsigned>(time(0)));

	// Main Loop
	glutMainLoop();


	return EXIT_SUCCESS;
}


///////////////////////////////////////////////////////////////
// Sources:
// https://www.youtube.com/watch?v=D9B_cxUB_LU
// http://www.swiftless.com/tutorials/opengl/lighting.html
// http://www.swiftless.com/tutorials/opengl/material_lighting.html
