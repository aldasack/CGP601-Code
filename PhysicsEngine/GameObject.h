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
	~GameObject();
	virtual void Shutdown() = 0;
	virtual void Draw() = 0;
	void Update(float dt);

	void SetPosition(const glm::vec3 position);
	void SetVelocity(const glm::vec3 velocity);
	void SetScale(const glm::vec3 scale);
	void SetColor(const glm::vec3 color);

	glm::vec3 GetPosition() const;
	glm::vec3 GetAcceleration() const;
	glm::vec3 GetVelocity() const;
	float GetMass() const;
	Collider::SphereCollider GetCollider() const;

	void AddForce(const glm::vec3 force);

protected:
	void shutdown();
	void prepareDraw();
	Collider::SphereCollider m_collider;
	RigidBody* m_pRigidbody;

private:
	glm::vec3 m_color;
};

#endif // ! _OBJECT_H