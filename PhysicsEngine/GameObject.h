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
	GameObject(Math::Vector3 position);
	~GameObject();
	void Shutdown();
	void Draw();
	void Update(float dt);

	void SetPosition(const Math::Vector3 position);
	void SetScale(const Math::Vector3 scale);
	void SetColor(const Math::Vector3 color);

	Math::Vector3 GetPosition() const;
	Math::Vector3 GetAcceleration() const;
	Math::Vector3 GetVelocity() const;

private:
	RigidBody* m_rigidbody;
	Math::Vector3 m_color;
};

#endif // ! _OBJECT_H