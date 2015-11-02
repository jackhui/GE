
#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include "..\Graphics\MeshInstance.h"
#include "..\Physics\cdObject.h"
#include "..\Math\simdmath.h"
#include "..\Physics\cdCollide.h"


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

	GameObject(CollidableObject* collObj, MeshInstance* meshObj, Collide* contact, const Matrix4& transform);

	void		Update(float deltaTime);

	void		collision(const GameObject* gameObj);

	Collide*	getContact();

	Vector3		getTranslate();
	void		objTranslate();

	bool		isCollided(const GameObject* gameObj);

	void		setTransform(const Matrix4& transform);

private:
	MeshInstance*			m_pMeshObj;
	CollidableObject*		m_pCollObj;
	Collide*				m_pContact;
	SIMDMatrix4				m_Transform;
};





#endif