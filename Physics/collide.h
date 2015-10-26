
#ifndef INTERSECTDATA_H
#define INTERSECTDATA_H

#include "../Math/simdmath.h"
#include "object.h"

class Object;

struct Response
{
	SIMDVector3 m_pObjectResponse;
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
	void setResponseObject1(const SIMDVector3& response, const int objectID);
	void setResponseObject2(const SIMDVector3& response, const int objectID);


	// handle collision detection
	void collision(const Object* object1, const Object* object2);
	void boxBoxCollide(const Object* box1, const Object* box2);
	void sphereSphereCollide(const Object* sphere1, const Object* sphere2);
	void boxSphereCollide(const Object* box, const Object* sphere);
	void pointBoxCollide(const Object* point, const Object* box);
	void pointSphereCollide(const Object* point, const Object* sphere);
	void raySphereCollide(const Object* ray, const Object* sphere);
	void rayBoxCollide(const Object* ray, const Object* box);


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