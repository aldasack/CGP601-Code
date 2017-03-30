////////////////////////////////
// Name: Box.cpp		      //
// Author: Alexander Kocourek //
// Date: 27/03/2017			  //
////////////////////////////////

#include "Box.h"

#include "RigidBody.h"

Box::Box() : GameObject(Collision::ColliderType::Box)
{	
}

Box::Box(const glm::vec3& position) : GameObject(Collision::ColliderType::Box, position)
{
}

Box::~Box(){}

void Box::Shutdown()
{
	shutdown();
}

void Box::Draw()
{
	prepareDraw();

	glBegin(GL_QUADS);

	//front face (normal pointing +z)
	glVertex3f(-0.5f, 1.0f, 0.5f);
	glVertex3f(0.5f, 1.0f, 0.5f);
	glVertex3f(0.5f, 0.0f, 0.5f);
	glVertex3f(-0.5f, 0.0f, 0.5f);

	// right face (normal pointing +x)
	glVertex3f(0.5f, 1.0f, 0.5f);
	glVertex3f(0.5f, 1.0f, -0.5f);
	glVertex3f(0.5f, 0.0f, -0.5f);
	glVertex3f(0.5f, 0.0f, 0.5f);

	// left face (normal pointing -x)
	glVertex3f(-0.5f, 1.0f, 0.5f);
	glVertex3f(-0.5f, 1.0f, -0.5f);
	glVertex3f(-0.5f, 0.0f, -0.5f);
	glVertex3f(-0.5f, 0.0f, 0.5f);

	//back face (normal pointing -z)
	glVertex3f(0.5f, 1.0f, -0.5f);
	glVertex3f(-0.5f, 1.0f, -0.5f);
	glVertex3f(-0.5f, 0.0f, -0.5f);
	glVertex3f(0.5f, 0.0f, -0.5f);

	// top face (normal pointing +y)
	glVertex3f(-0.5f, 1.0f, -0.5f);
	glVertex3f(0.5f, 1.0f, -0.5f);
	glVertex3f(0.5f, 1.0f, 0.5f);
	glVertex3f(-0.5f, 1.0f, 0.5f);

	// bottom face (normal pointing -y)
	glVertex3f(0.5f, 0.0f, 0.5f);
	glVertex3f(-0.5f, 0.0f, 0.5f);
	glVertex3f(-0.5f, 0.0f, -0.5f);
	glVertex3f(0.5f, 0.0f, -0.5f);

	glEnd();

	glPopMatrix();
}