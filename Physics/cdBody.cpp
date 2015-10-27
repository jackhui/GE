
#include "cdSphere.h"
#include "cdAabb.h"
#include "cdBody.h"
#include "cdPoint.h"
#include "cdRay.h"

SIMDVector3 Body::getCenter() const
{
	SIMDVector3 center;
	if (m_pType == typeAABB)
	{
		AABB* self = (AABB*) this;
		center = (SIMDVector3)self->getCenter();
	}
	else if (m_pType == typeSPHERE)
	{
		Sphere* self = (Sphere*) this;
		center = (SIMDVector3)self->getCenter();
	}
	return center;
}