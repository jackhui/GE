#include "aabb.h"

AABB::AABB(const SIMDMatrix4 & transform, const SIMDVector3 & dimension, const SIMDVector3 & origin)
{
	float aabb[6];
	aabb[0] = origin.GetX() - dimension.GetX() / 2;
	aabb[1] = origin.GetY() - dimension.GetY() / 2;
	aabb[2] = origin.GetZ() + dimension.GetZ() / 2;
	aabb[3] = origin.GetX() + dimension.GetX() / 2;
	aabb[4] = origin.GetY() + dimension.GetY() / 2;
	aabb[5] = origin.GetZ() - dimension.GetZ() / 2;

	SIMDVector3 min(aabb[0], aabb[1], aabb[2]);
	SIMDVector3 max(aabb[3], aabb[4], aabb[5]);
	SIMDVector3 translate(transform.getTranslateX(), transform.getTranslateY(), transform.getTranslateZ());
	min += translate;
	max += translate;

	m_pMax = max;
	m_pMin = min;
	Body::setType(1);
}

const SIMDVector3 AABB::getCenter()
{
	SIMDVector3 m_pCenter = m_pMin + m_pMax;
	m_pCenter.Set(m_pCenter.GetX() / 2, m_pCenter.GetY() / 2,
		m_pCenter.GetZ() / 2);

	return m_pCenter;
}

void AABB::translate(const SIMDVector3 & translate)
{
	m_pMax += translate;
	m_pMin += translate;
}



