////////////////////////////////
// Name: GameObject.h		  //
// Author: Alexander Kocourek //
// Date: 16/02/2017			  //
////////////////////////////////

#pragma once

#ifndef _OBJECT_H
#define _OBJECT_H


#include "Defs.h"

class GameObject
{
public:
	GameObject();
	/*
	*	Constructor creates Box.
	*	'position' is the position of the GameObject.
	*/
	GameObject(glm::vec3 position);
	/*
	*	Constructor creates Box.
	*	'position' is the position of the GameObject.
	*	'rotation' is the rotation of the Gameobject. Rotation is in Euler Angles and radians.
	*/
	GameObject(glm::vec3 position, glm::vec3 rotation);
	~GameObject();
	virtual void Shutdown() = 0;
	virtual void Draw() = 0;
	/*
	*	Sets position of GameObject at 'position'.
	*/
	void SetPosition(const glm::vec3& position);
	/*
	*	Scales the GameObject with 'scale'.
	*/
	void SetScale(const glm::vec3& scale);
	/*
	*	Sets rotation of GameObject with 'rotation'. 'rotation' is in Euler Angles and radians.
	*/
	void SetRotation(const glm::vec3& rotation);
	/*
	*	Sets velocity of assosiated rigidbody.
	*/
	void SetVelocity(const glm::vec3& velocity);
	/*
	*	Sets the mass of assosiated rigidbody.
	*/
	void SetMass(const float mass);
	/*
	*	Sets color of GameObject. 'color' is in RGB where 0 is 0% and 1 is 100%.
	*/
	void SetColor(const glm::vec3& color);

	/*
	*	Returns position of GameObject.
	*/
	glm::vec3 GetPosition() const;
	/*
	*	Returns scale of GameObject.
	*/
	glm::vec3 GetScale() const;
	/*
	*	Returns velocity of assosiated rigidbody.
	*/
	glm::vec3 GetVelocity() const;
	/*
	*	Returns mass of assosiated rigidbody.
	*/
	float GetMass() const;
	/*
	*	Returns list of vertices.
	*/
	std::vector<glm::vec3> GetVertices() const;

	/*
	*	Returns if GameObject is static.
	*/
	bool IsStatic() const;
	/*
	*	Sets if GameObject is static.
	*/
	void IsStatic(const bool isStatic);

	/*
	*	Returns if GameObject is affected by gravity.
	*/
	bool UseGravity() const;
	/*
	*	Sets if GameObject is affected by gravity.
	*/
	void UseGravity(const bool useGravity);

	/*
	*	Adds a force to the assosiated rigidbody.
	*/
	void AddForce(const glm::vec3& force);
	/*
	*	Adds a force at an offset to the assosiated rigidbody.
	*/
	void AddForce(const glm::vec3& force, const glm::vec3& position);
	/*
	*	Adds a torque to the assosiated rigidbody.
	*/
	void AddTorque(const glm::vec3& torque);

protected:
	/*
	*	Destructor replacement.
	*/
	void shutdown();
	/*
	*	Prepares the draw of the GameObject.
	*	Pass position, rotation, scale and color to OpenGL API.
	*/
	void prepareDraw();
	/*
	*	RigidBody component which enables physical simulation of the GameObject.
	*/
	RigidBody* m_pRigidbody;

	/*
	*	stores vertices, gets sets by child class. Needs to be accsessed by RigidBody to generate Collider
	*/
	std::vector<glm::vec3> m_vertices;

private:
	/*
	*	Color of GameObject in RGB
	*/
	glm::vec3 m_color;
	/*
	*	Scale of GameObject
	*/
	glm::vec3 m_scale;
};

#endif // ! _OBJECT_H