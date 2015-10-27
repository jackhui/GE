#include "cdObject.h"


Body * CollidableObject::getBody() const
{
	return m_pBody;
}

void CollidableObject::simulate(const int cycle)
{
	for (int i = 0; i < cycle; i++)
	{
		m_pBody->translate(m_pVelocity);
	}
}
