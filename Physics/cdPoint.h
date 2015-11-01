#ifndef CDPOINT_H
#define CDPOINT_H

#include "../Math/simdmath.h"
#include "cdCollide.h"
#include "cdBody.h"

class Point : public Body
{
public:
	Point(){}

	Point(const Vector3& point)
	{
		m_pPoint = point;
		Body::setType(typePOINT);
	}

	Vector3 getPoint() const { return m_pPoint; }
	virtual void translate(const Vector3& translate);
	const Vector3 getCenter();

private:
	Vector3 m_pPoint;
};

#endif POINT_H