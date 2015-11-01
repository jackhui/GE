
#include "cdSphere.h"
#include "cdAabb.h"
#include "cdBody.h"
#include "cdPoint.h"
#include "cdRay.h"

Vector3 Body::getCenter() const
{
	Vector3 center;
	if (m_pType == typeAABB)
	{
		AABB* self = (AABB*) this;
		center = (Vector3)self->getCenter();
	}
	else if (m_pType == typeSPHERE)
	{
		Sphere* self = (Sphere*) this;
		center = (Vector3)self->getCenter();
	}
	return center;
}