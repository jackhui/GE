
#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include "..\Graphics\MeshInstance.h"
#include "..\Physics\cdObject.h"
#include "..\Math\simdmath.h"


typedef SIMDMatrix4 Matrix4;
typedef SIMDVector3 Vector3;
// constructor
// create a game object with physics component and mesh component

// MeshComponent* meshObj = nullptr;
// ColliableObject* collObj = new ColliableObject(..);
// GameObject obj(Position/Transform, collObj, meshObj);
// debug drawing if necessary

class GameObject
{
public:
	GameObject(){}

	GameObject(CollidableObject* collObj, MeshInstance* meshObj, const Matrix4& transform);

	void Update(float deltaTime);

	Vector3 getTranslate();
	void objTranslate();

private:
	MeshInstance* m_pMeshObj;
	CollidableObject* m_pCollObj;
	SIMDMatrix4 m_Transform;
};





#endif