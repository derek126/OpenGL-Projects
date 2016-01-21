#pragma once

#include "Drawable.h"

/*
* A basic quad primitive that can be drawn
*/
class Quad : public Drawable
{

public:

	Quad();
	virtual ~Quad() override;
	virtual void Draw() override;
};

