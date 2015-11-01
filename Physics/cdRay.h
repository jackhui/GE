
#ifndef CDRAY_H
#define CDRAY_H

#include "../Math/simdmath.h"
#include "cdCollide.h"
#include "cdBody.h"

class Ray : public Body
{
public:
	Ray() {}

	Ray(const Vector3& dir, const Vector3& start)
	{
		m_pDir = dir;
		m_pStart = start;
		Body::setType(typeRAY);
	}

	Vector3 getDir() const { return m_pDir; }
	Vector3 getStart() const { return m_pStart; }
	virtual void translate(const Vector3& translate);

private:
	Vector3 m_pDir;
	Vector3 m_pStart;
};


#endif