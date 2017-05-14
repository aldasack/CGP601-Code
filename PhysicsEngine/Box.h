////////////////////////////////
// Name: Box.h			      //
// Author: Alexander Kocourek //
// Date: 27/03/2017			  //
////////////////////////////////

#pragma once

#ifndef _BOX_H
#define _BOX_H

#include "Defs.h"
#include "GameObject.h"

class Box :
	public GameObject
{
public:
	Box();
	/*
	*	Constructor positioning Box at 'position'.
	*/
	Box(const glm::vec3& position);
	~Box();
	/*
	*	Destructor replacement.
	*/
	void Shutdown() override;
	/*
	*	Draws Box in the scene.
	*/
	void Draw() override;

private:
	/*
	*	Fills the vertices list with vectors, which represent a box.
	*/
	void setVertices();
};

#endif // !_BOX_H