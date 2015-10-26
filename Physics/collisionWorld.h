
#ifndef COLLISIONWORLD_H
#define COLLISIONWORLD_H
#include <vector>
#include "object.h"
#include "collide.h"


class CollisionWorld
{
public:
	CollisionWorld() {}
	void addObject(const Object& object);
	void addCollide(const Collide& collide);
	int getObjectSize() const { return m_pObjects.size(); }
	int getCollideSize() const { return m_pCollide.size(); }
	void computeCollision();

private:
	std::vector<Object> m_pObjects;
	std::vector<Collide> m_pCollide;
};


#endif