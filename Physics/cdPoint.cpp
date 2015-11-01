#include "cdPoint.h"

void Point::translate(const Vector3 & translate)
{
	m_pPoint += translate;
}

const Vector3 Point::getCenter()
{
	return m_pPoint;
}
