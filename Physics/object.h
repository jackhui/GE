
#ifndef OBJECT_H
#define OBJECT_H

#include "body.h"
#include "aabb.h"
#include "../Math/simdmath.h"

class Object
{
public:
	Object(){}

	Object(Body* body, const SIMDVector3& velocity, const int objectID) :
		m_pPos(body->getCenter()),
		m_pCurrent(body->getCenter()),
		m_pVelocity(velocity),
		m_pBody(body),
		m_pObjectID(objectID) {}



	SIMDVector3 getPosition() const { return m_pPos; }
	SIMDVector3 getVelocity() const { return m_pVelocity; }
	int getObjectID() const { return m_pObjectID; }
	Body* getBody() const;

	void simulate(const int cycle);

private:
	SIMDVector3 m_pPos;
	SIMDVector3 m_pCurrent;
	SIMDVector3 m_pVelocity;
	Body* m_pBody;
	int m_pObjectID;
};




#endif