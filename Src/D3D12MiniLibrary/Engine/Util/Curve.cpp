#include "Curve.h"

Curve2D::Curve2D()
{
}

Curve2D::~Curve2D()
{
	this->_points.clear();
	this->_points.shrink_to_fit();
}


Vector2 Curve2D::Evaluate(float t)
{
	if (_points.size() <= 2) {
		return Vector2(0.0f, 0.0f);
	}
	Vector2 result = Vector2::zero;
	unsigned int size = static_cast<unsigned int >(_points.size());
	for (unsigned int i = 0; i < size; i++) {
		result += _points[i] * Bernstein(size - 1, i, t);
	}
	return result;
}

void Curve2D::SetStartPoint(const Vector2 & point)
{
	_points.insert(_points.begin(), point);
}

void Curve2D::SetEndPoint(const Vector2 & point)
{
	_points.insert(_points.end(), point);

}

void Curve2D::AddControlPoint(const Vector2 & point)
{
	_points.push_back(point);
}
