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
	GameObject(glm::vec3 position);
	GameObject(glm::vec3 position, glm::vec3 rotation);
	~GameObject();
	virtual void Shutdown() = 0;
	virtual void Draw() = 0;
	void Update(float dt);

	void SetPosition(const glm::vec3& position);
	void SetScale(const glm::vec3& scale);
	void SetRotation(const glm::vec3& rotation);
	void SetVelocity(const glm::vec3& velocity);
	void SetMass(const float mass);
	void SetColor(const glm::vec3& color);

	glm::vec3 GetPosition() const;
	glm::vec3 GetRotation() const;
	glm::vec3 GetScale() const;
	glm::vec3 GetAcceleration() const;
	glm::vec3 GetVelocity() const;
	float GetMass() const;
	std::vector<glm::vec3> GetVertices() const;
	//Collision::SphereCollider GetCollider() const;

	bool IsStatic() const;
	void IsStatic(const bool isStatic);

	bool UseGravity() const;
	void UseGravity(const bool useGravity);

	void AddForce(const glm::vec3& force);
	void AddForce(const glm::vec3& force, const glm::vec3& position);
	void AddTorque(const glm::vec3& torque);

protected:
	void shutdown();
	void prepareDraw();
	//Collision::SphereCollider m_collider;
	RigidBody* m_pRigidbody;

	// stores vertices, gets sets by child class. Needs to be accsessed by RigidBody to generate Collider
	std::vector<glm::vec3> m_vertices;

private:
	//virtual void setInertiaTensor() = 0;
	glm::vec3 m_color;
	glm::vec3 m_scale;
};

#endif // ! _OBJECT_H