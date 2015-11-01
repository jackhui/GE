#include "GameObject.h"

GameObject::GameObject(CollidableObject * collObj, MeshInstance * meshObj, const Matrix4 & transform)
{
	m_Transform = transform;
	m_pCollObj = collObj;
	m_pMeshObj = meshObj;
}

void GameObject::Update(float deltaTime)
{
	//m_pMeshObj->Update(deltaTIme);
	//m_pCollObj->Update(deltaTime);
//	m_pCollObj->SetPosition(SIMDVector3(m_Transform.getTranslateX(), m_Transform.getTranslateY(), m_Transform.getTranslateZ())); // temp
}

Vector3 GameObject::getTranslate()
{
	Vector3 translate = Vector3(m_Transform.getTranslateX(), m_Transform.getTranslateY(), m_Transform.getTranslateZ());
	return translate;
}

void GameObject::objTranslate()
{
	
}
