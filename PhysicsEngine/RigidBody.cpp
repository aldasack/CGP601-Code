////////////////////////////////
// Name: RigidBody.cpp		  //
// Author: Alexander Kocourek //
// Date: 06/02/2017			  //
////////////////////////////////

#include "RigidBody.h"

float time = 0.0f;
float threshold = 0.1f;

bool bo = false;

RigidBody::RigidBody()
{
	m_position.x = 0.0f;
	m_position.y = 0.0f;
	m_position.z = 0.0f;

	m_acceleration.x = 0.0f;
	m_acceleration.y = 0.0f;
	m_acceleration.z = 0.0f;

	m_velocity.x = 0.0f;
	m_velocity.y = 0.0f;
	m_velocity.z = 0.0f;

	m_collider.position = m_position;
	m_collider.radius = 1.0f;
}

RigidBody::~RigidBody()
{
}

void RigidBody::Update(float dt)
{
	time += dt;

	// Only considering the y-coordinate, because gravity only affects y-coordinate.

	// 1. Step: Calculate acceleration (a).
	m_acceleration.y = Constants::G * -1.0f; // acceleration goes down on the y-axis and needs to be negative.
	// In this case unnessasary, because a is already known.

	// 2. Step: Calculate velocity (v) in this moment.
	m_velocity.y += m_acceleration.y * dt;
	//if (m_velocity.y <= 0.000001f && m_velocity.y >= -0.000001f) m_velocity.y = 0.0f;
	
	// 3. Step: Calcuate travelled distance during the time step.
	float s = m_velocity.y * dt;

	// 4. Step: Adding resulting distance to the position.
	m_position.y += s;


	if (m_position.y <= -0.5f) // -1.0 is the height of the plane, radius is 0.5m
	{
		m_velocity.y *= -1.0f; // reverse velocity on collision
		m_position.y = -0.5; // pushing sphere out of the ground
	}

	
	 // get time of fall.
	 // s = 6m; a = 9.81 m/s^2 => t ~ 1.1s
	if (m_position.y <= -0.5f && bo == false) // -1.0 is the height of the plane, radius is 0.5m
	{
		bo = true;
		//std::cout << glutGet(GLUT_ELAPSED_TIME) * 0.001f << std::endl;
		std::cout << "Time of fall: " << time << std::endl;
		std::cout << "Velcoity: " << m_velocity.y << std::endl;
	}
	
}

Math::Vector3 RigidBody::GetPosition() const
{
	return m_position;
}

void RigidBody::SetPosition(const Math::Vector3 position)
{
	m_position = position;
}