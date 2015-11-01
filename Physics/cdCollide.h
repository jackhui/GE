
#ifndef CDCOLLIDE_H
#define CDCOLLIDE_H

#include "../Math/simdmath.h"
#include "cdObject.h"

class CollidableObject;
typedef SIMDVector3 Vector3;
typedef SIMDMatrix4 Matrix4;

struct Response
{
	Vector3 m_pObjectResponse;
	int m_pObjectID;
};

class Collide
{
public:

	Collide(){}

	// two basic getters
	const bool  getCollide() const { return m_pCollide; }
	const float getDistance() const { return m_pDistance; }
	const Response getResponseObject1() const { return m_pResponseObject1; }
	const Response getResponseObject2() const { return m_pResponseObject2; }
	void setCollide(bool collide) { m_pCollide = collide; }
	void setDistance(float distance) { m_pDistance = distance; }
	void setResponseObject1(const Vector3& response, const int objectID);
	void setResponseObject2(const Vector3& response, const int objectID);


	// handle collision detection
	void collision(const CollidableObject* object1, const CollidableObject* object2);
	void boxBoxCollide(const CollidableObject* box1, const CollidableObject* box2);
	void sphereSphereCollide(const CollidableObject* sphere1, const CollidableObject* sphere2);
	void boxSphereCollide(const CollidableObject* box, const CollidableObject* sphere);
	void pointBoxCollide(const CollidableObject* point, const CollidableObject* box);
	void pointSphereCollide(const CollidableObject* point, const CollidableObject* sphere);
	void raySphereCollide(const CollidableObject* ray, const CollidableObject* sphere);
	void rayBoxCollide(const CollidableObject* ray, const CollidableObject* box);


private:
	// if collide, return true; otherwise, return false
	bool	m_pCollide;
	// if two bodies collide, return the distance of the overlap betwenn two spheres
	float	m_pDistance;
	// response of object1 after collided
	Response m_pResponseObject1;
	// response of object2 after collided
	Response m_pResponseObject2;
};


#endif