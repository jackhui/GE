
#ifndef CDAABB_H
#define CDAABB_H

#include "../Math/simdmath.h"
#include "cdCollide.h"
#include "cdSphere.h"
#include "cdBody.h"

class AABB : public Body
{
public:

	AABB(const Vector3& min, const Vector3& max)
	{
		m_pMin = min;
		m_pMax = max;
		Body::setType(1);
	}

	AABB(const Matrix4& transform, const Vector3& dimension, const Vector3& position);

//	AABB(const SIMDVector3& dimension, const SIMDVector3& position);

	AABB(){}
	// check does two AABBs intersect? return an IntersectData
//	IntersectData collision(const Body& other);

	// two basic getters, return the max and min vector
	Vector3 getMin() const { return m_pMin; }
	Vector3 getMax() const { return m_pMax; }

	const Vector3 getCenter();

	virtual void translate(const Vector3& translate);


private:
	// a vector which contains the minimum point
	Vector3 m_pMin;
	// a vector which contains the maximum point
	Vector3 m_pMax;
};


#endif