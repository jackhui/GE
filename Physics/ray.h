
#ifndef RAY_H
#define RAY_H

#include "../Math/simdmath.h"
#include "collide.h"
#include "body.h"

class Ray : public Body
{
public:
	Ray() {}

	Ray(const SIMDVector3& dir, const SIMDVector3& start)
	{
		m_pDir = dir;
		m_pStart = start;
		Body::setType(typeRAY);
	}

	SIMDVector3 getDir() const { return m_pDir; }
	SIMDVector3 getStart() const { return m_pStart; }
	virtual void translate(const SIMDVector3& translate);

private:
	SIMDVector3 m_pDir;
	SIMDVector3 m_pStart;
};


#endif