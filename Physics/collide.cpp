
#include "collide.h"
#include "Sphere.h"
#include "body.h"
#include "aabb.h"
#include "point.h"
#include "ray.h"


void Collide::setResponseObject1(const SIMDVector3& response, const int objectID)
{
	m_pResponseObject1.m_pObjectResponse = response;
	m_pResponseObject1.m_pObjectID = objectID;
}

void Collide::setResponseObject2(const SIMDVector3& response, const int objectID)
{
	m_pResponseObject2.m_pObjectResponse = response;
	m_pResponseObject2.m_pObjectID = objectID;
}

void Collide::collision(const Object * object1, const Object * object2)
{
	if (object1->getBody()->getType() == typeAABB && object2->getBody()->getType() == typeAABB)
		boxBoxCollide(object1, object2);
	else if (object1->getBody()->getType() == typeSPHERE && object2->getBody()->getType() == typeSPHERE)
		sphereSphereCollide(object1, object2);
	// box vs sphere OR sphere vs box
	else if (object1->getBody()->getType() == typeAABB && object2->getBody()->getType() == typeSPHERE)
		boxSphereCollide(object1, object2);
	else if (object1->getBody()->getType() == typeSPHERE && object2->getBody()->getType() == typeAABB)
		boxSphereCollide(object2, object1);
	// point vs box OR box vs point
	else if (object1->getBody()->getType() == typePOINT && object2->getBody()->getType() == typeAABB)
		pointBoxCollide(object1, object2);
	else if (object1->getBody()->getType() == typeAABB && object2->getBody()->getType() == typePOINT)
		pointBoxCollide(object2, object1);
	// point vs sphere OR sphere vs point
	else if (object1->getBody()->getType() == typePOINT && object2->getBody()->getType() == typeSPHERE)
		pointSphereCollide(object1, object2);
	else if (object1->getBody()->getType() == typeSPHERE && object2->getBody()->getType() == typePOINT)
		pointSphereCollide(object2, object1);
	// ray vs sphere OR sphere vs ray
	else if (object1->getBody()->getType() == typeRAY && object2->getBody()->getType() == typeSPHERE)
		raySphereCollide(object1, object2);
	else if (object1->getBody()->getType() == typeSPHERE && object2->getBody()->getType() == typeRAY)
		raySphereCollide(object2, object1);
	// ray vs box OR box vs ray
	else if (object1->getBody()->getType() == typeRAY && object2->getBody()->getType() == typeAABB)
		rayBoxCollide(object1, object2);
	else if (object1->getBody()->getType() == typeAABB && object2->getBody()->getType() == typeRAY)
		rayBoxCollide(object2, object1);
	else
		printf("no match object type!");

}


void Collide::boxBoxCollide(const Object * box1, const Object * box2)
{
	AABB *aabb1 = (AABB*)box1->getBody();
	AABB *aabb2 = (AABB*)box2->getBody();
	float maxDistance = 0.0f;
	SIMDVector3 dist1 = aabb2->getMin() - aabb1->getMax();
	SIMDVector3 dist2 = aabb1->getMin() - aabb2->getMax();

	// create the float array to store the maximum X, Y and Z
	float max[3];
	// create a float to store the maximum float among the
	// values of float array max[3]
	maxDistance = max[0];

	// here is the calculation to generate max[3]
	if (dist1.GetX() > dist2.GetX())
		max[0] = dist1.GetX();
	else
		max[0] = dist2.GetX();
	if (dist1.GetY() > dist2.GetY())
		max[1] = dist1.GetY();
	else
		max[1] = dist1.GetY();
	if (dist1.GetZ() > dist2.GetZ())
		max[2] = dist1.GetZ();
	else
		max[2] = dist1.GetZ();

	// pick up the maximum value from max[3]
	for (int i = 1; i < 3; i++)
	{
		if (maxDistance < max[i])
			maxDistance = max[i];
	}

	setCollide(maxDistance < 0);
	setDistance(maxDistance);

	// compute the response vectors
	SIMDVector3 responseObject1 = box1->getPosition() - box2->getPosition();
	SIMDVector3 responseObject2 = box2->getPosition() - box1->getPosition();
	setResponseObject1(responseObject1.Normalize(), box1->getObjectID());
	setResponseObject2(responseObject2.Normalize(), box2->getObjectID());
}

void Collide::sphereSphereCollide(const Object * sphere_1, const Object * sphere_2)
{
	Sphere *sphere1 = (Sphere*)sphere_1->getBody();
	Sphere *sphere2 = (Sphere*)sphere_2->getBody();
	float realDist = 0.0f;
	float radiusDist = sphere1->getRadius() + sphere2->getRadius();
	float centerDist = (sphere1->getCenter() - sphere2->getCenter()).Length();
	realDist = centerDist - radiusDist;
	setCollide(realDist < 0);
	setDistance(realDist);

	// compute the response vectors
	SIMDVector3 responseObject1 = sphere_1->getPosition() - sphere_2->getPosition();
	SIMDVector3 responseObject2 = sphere_2->getPosition() - sphere_1->getPosition();
	setResponseObject1(responseObject1.Normalize(), sphere_1->getObjectID());
	setResponseObject2(responseObject2.Normalize(), sphere_2->getObjectID());
}

void Collide::boxSphereCollide(const Object * box, const Object * sphere)
{
}

void Collide::pointBoxCollide(const Object * point_, const Object * box)
{
	Point *point = (Point*)point_->getBody();
	AABB *aabb = (AABB*)box->getBody();

	if (point->getPoint().GetX() > aabb->getMin().GetX() && point->getPoint().GetX() < aabb->getMax().GetX() &&
		point->getPoint().GetY() > aabb->getMin().GetY() && point->getPoint().GetY() < aabb->getMax().GetY() &&
		point->getPoint().GetZ() > aabb->getMin().GetZ() && point->getPoint().GetZ() < aabb->getMax().GetZ())
	{
		setCollide(true);
		// haven't calculate the distance
		setDistance(-1);
	}
	else
	{
		setCollide(false);
		// haven't calculate the distance
		setDistance(1);
	}

	// compute the response vectors
	SIMDVector3 responseObject1 = point_->getPosition() - box->getPosition();
	SIMDVector3 responseObject2 = box->getPosition() - point_->getPosition();
	setResponseObject1(responseObject1, point_->getObjectID());
	setResponseObject2(responseObject2, box->getObjectID());
}

void Collide::pointSphereCollide(const Object * point_, const Object * sphere_)
{
	Point *point = (Point*)point_->getBody();
	Sphere *sphere = (Sphere*)sphere_->getBody();

	float distance = 0.0f;
	distance = (point->getPoint() - sphere->getCenter()).Length();

	setCollide(distance < sphere->getRadius());
	setDistance(distance - sphere->getRadius());

	// compute the response vectors
	SIMDVector3 responseObject1 = point_->getPosition() - sphere_->getPosition();
	SIMDVector3 responseObject2 = sphere_->getPosition() - point_->getPosition();
	setResponseObject1(responseObject1, point_->getObjectID());
	setResponseObject2(responseObject2, sphere_->getObjectID());
}

void Collide::raySphereCollide(const Object * ray_, const Object * sphere_)
{
	Ray *ray = (Ray*)ray_->getBody();
	Sphere *sphere = (Sphere*)sphere_->getBody();
	SIMDVector3 vec = ray->getStart() - sphere->getCenter();
	setCollide(true);
	setDistance(0.0f);
	float fB = vec.Dot(ray->getDir());
	float fC = vec.Dot(vec) - sphere->getRadius()*sphere->getRadius();
	if (fC > 0.0f && fB > 0.0f)
		setCollide(false);
	float fDisc = fB*fB - fC;
	if (fDisc < 0.0f)
		setCollide(false);

	// compute the response vectors
	SIMDVector3 responseObject1 = ray_->getPosition() - sphere_->getPosition();
	SIMDVector3 responseObject2 = sphere_->getPosition() - ray_->getPosition();
	setResponseObject1(responseObject1, ray_->getObjectID());
	setResponseObject2(responseObject2, sphere_->getObjectID());
}

void Collide::rayBoxCollide(const Object * ray, const Object * box)
{
}
