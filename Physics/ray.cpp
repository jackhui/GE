#include "ray.h"

void Ray::translate(const SIMDVector3 & translate)
{
	m_pStart += translate;
}
