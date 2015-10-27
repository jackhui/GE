
#ifndef CDAABB_H
#define CDAABB_H

#include "../Math/simdmath.h"
#include "cdCollide.h"
#include "cdSphere.h"
#include "cdBody.h"

class AABB : public Body
{
public:

	AABB(const SIMDVector3& min, const SIMDVector3& max)
	{
		m_pMin = min;
		m_pMax = max;
		Body::setType(1);
	}

	AABB(const SIMDMatrix4& transform, const SIMDVector3& dimension, const SIMDVector3& position);


	AABB(){}
	// check does two AABBs intersect? return an IntersectData
//	IntersectData collision(const Body& other);

	// two basic getters, return the max and min vector
	SIMDVector3 getMin() const { return m_pMin; }
	SIMDVector3 getMax() const { return m_pMax; }

	const SIMDVector3 getCenter();

	virtual void translate(const SIMDVector3& translate);


private:
	// a vector which contains the minimum point
	SIMDVector3 m_pMin;
	// a vector which contains the maximum point
	SIMDVector3 m_pMax;
};


#endif