#include "cdRay.h"

void Ray::translate(const Vector3 & translate)
{
	m_Start += translate;
}
