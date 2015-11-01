
#ifndef CDSPHERE_H
#define CDSPHERE_H

#include "../Math/simdmath.h"
#include "cdCollide.h"
#include "cdBody.h"

class Sphere : public Body
{
public:

	Sphere(){}

	Sphere(const Vector3& center, float radius)
	{
		m_pCenter = center;
		m_pRadius = radius;
		Body::setType(0);
	}


	// check does two shperes intersect? return an IntersectData
//	IntersectData collision(const Body& other);

	// two basic getters
	Vector3  getCenter() const { return m_pCenter; }
	float getRadius() const { return m_pRadius; }

	virtual void translate(const Vector3 & translate);

private:
	// the position of the center of the sphere
	Vector3		m_pCenter;
	// the distance of the radius of the sphere
	float			m_pRadius;
};

#endif