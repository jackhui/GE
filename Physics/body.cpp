
#include "Sphere.h"
#include "aabb.h"
#include "body.h"
#include "point.h"
#include "ray.h"

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