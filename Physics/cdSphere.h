
#ifndef CDSPHERE_H
#define CDSPHERE_H

#include "../Math/simdmath.h"
#include "cdCollide.h"
#include "cdBody.h"

class Sphere : public Body
{
public:

	Sphere(){}

	Sphere(const SIMDVector3& center, float radius)
	{
		m_pCenter = center;
		m_pRadius = radius;
		Body::setType(0);
	}

	Sphere(const SIMDMatrix4& transform, const float& radius, const SIMDVector3& origin);

	// check does two shperes intersect? return an IntersectData
//	IntersectData collision(const Body& other);

	// two basic getters
	SIMDVector3  getCenter() const { return m_pCenter; }
	float getRadius() const { return m_pRadius; }

	virtual void translate(const SIMDVector3 & translate);

private:
	// the position of the center of the sphere
	SIMDVector3		m_pCenter;
	// the distance of the radius of the sphere
	float			m_pRadius;
};

#endif