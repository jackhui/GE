#include "cdPoint.h"

void Point::translate(const SIMDVector3 & translate)
{
	m_pPoint += translate;
}

const SIMDVector3 Point::getCenter() 
{
	return m_pPoint;
}
