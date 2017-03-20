#pragma once

#ifndef _OBJECT_H
#define _OBJECT_H

////////////////////////////////
// Name: GameObject.h		  //
// Author: Alexander Kocourek //
// Date: 16/02/2017			  //
////////////////////////////////

#include "Defs.h"
#include "RigidBody.h"

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
	void SetColor(const glm::vec3& color);

	glm::vec3 GetPosition() const;
	glm::vec3 GetRotation() const;
	glm::vec3 GetAcceleration() const;
	glm::vec3 GetVelocity() const;
	float GetMass() const;
	Collision::SphereCollider GetCollider() const;

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
	Collision::SphereCollider m_collider;
	RigidBody* m_pRigidbody;

private:
	//virtual void setInertiaTensor() = 0;
	glm::vec3 m_color;
};

#endif // ! _OBJECT_H