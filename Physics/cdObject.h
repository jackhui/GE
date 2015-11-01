
#ifndef CDOBJECT_H
#define CDOBJECT_H

#include "cdBody.h"
#include "..\Math\simdmath.h"
//#include "..\Physics\cdCollisionWorld.h"
//class CollisionWorld;

typedef SIMDVector3 Vector3;
typedef SIMDMatrix4 Matrix4;

class CollidableObject
{
public:
	CollidableObject(){}

	CollidableObject(Body* body, const int objectID) :
		m_pPos(body->getCenter()),
		m_pCurrent(body->getCenter()),
//		m_pVelocity(velocity),
		m_pBody(body),
		m_pObjectID(objectID) 
	{
//		CollisionWorld::getInstance()->getObjectList().push_back(*this);
	}



	Vector3 getPosition() const { return m_pPos; }
//	SIMDVector3 getVelocity() const { return m_pVelocity; }
	int getObjectID() const { return m_pObjectID; }
	Body* getBody() const;
	//void translate(const SIMDMatrix4 translate);
	void SetPosition(Vector3 position);

	void simulate(const int cycle);

	void update();

private:
	SIMDVector3 m_pPos;
	SIMDVector3 m_pCurrent;
//	SIMDVector3 m_pVelocity;
	Body* m_pBody;
	int m_pObjectID;
	// int EntityIO;
};




#endif