#include "cdCollisionWorld.h"

void CollisionWorld::addObject(const Object & object)
{
	m_pObjects.push_back(object);
}

void CollisionWorld::addCollide(const Collide & collide)
{
	m_pCollide.push_back(collide);
}

void CollisionWorld::computeCollision()
{
	Collide collide;
	for (int i = 0; i < m_pObjects.size(); i++)
	{
		for (int j = i + 1; j < m_pObjects.size(); j++)
		{
			collide.collision(&m_pObjects[i], &m_pObjects[j]);
			// collide.getCollide() returns a boolean value, true means collide, false means not collide
			if(collide.getCollide())
				addCollide(collide);
		}
	}
}
