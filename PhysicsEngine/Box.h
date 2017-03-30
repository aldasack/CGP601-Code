#pragma once

////////////////////////////////
// Name: Box.h			      //
// Author: Alexander Kocourek //
// Date: 27/03/2017			  //
////////////////////////////////

#ifndef _BOX_H
#define _BOX_H

#include "Defs.h"
#include "GameObject.h"

class Box :
	public GameObject
{
public:
	Box();
	Box(const glm::vec3& position);
	~Box();
	void Shutdown() override;
	void Draw() override;
};

#endif // !_BOX_H