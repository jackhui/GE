#include "Sphere.h"

Sphere::Sphere(const SIMDMatrix4 & transform, const float & radius, const SIMDVector3 & origin)
{
	float AABB[6];
	SIMDVector3 translate(transform.getTranslateX(), transform.getTranslateY(), transform.getTranslateZ());
	m_pCenter = origin;
	m_pCenter += translate;
	m_pRadius = radius;
	Body::setType(0);
}

void Sphere::translate(const SIMDVector3 & translate)
{
	m_pCenter += translate;
}
