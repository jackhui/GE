#include "object.h"


Body * Object::getBody() const
{
	return m_pBody;
}

void Object::simulate(const int cycle)
{
	for (int i = 0; i < cycle; i++)
	{
		m_pBody->translate(m_pVelocity);
	}
}
