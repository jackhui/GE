#include "cdObject.h"


Body * CollidableObject::getBody() const
{
	return m_pBody;
}

void CollidableObject::SetPosition(Vector3 position)
{
	m_pPos = position;
}

void CollidableObject::simulate(const int cycle)
{
	/**
	for (int i = 0; i < cycle; i++)
	{
		m_pBody->translate(m_pVelocity);
	}
	*/
}

void CollidableObject::update()
{

}

//gameobjectarray[ID]->getTransform
