#pragma once
#include <vector>
#include "Math.h"

class Curve2D

{
private:
	std::vector<Vector2> _points;
public:
private:
public:

	Curve2D();

	~Curve2D();

	virtual Vector2 Evaluate(float t);

	virtual void SetStartPoint(const Vector2& point);

	virtual void SetEndPoint(const Vector2& point);

	virtual void AddControlPoint(const Vector2& point);

};
