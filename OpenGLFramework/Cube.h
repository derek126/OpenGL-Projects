#pragma once

#include "Drawable.h"

/*
* A basic cube primitive that can be drawn
*/
class Cube : public Drawable
{

public:

	Cube();
	virtual ~Cube() override;
	virtual void Draw(const glm::mat4& View, const glm::mat4& Projection) override;
};

