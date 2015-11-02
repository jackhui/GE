#include "GameObject.h"

GameObject::GameObject(CollidableObject * collObj, MeshInstance * meshObj, Collide* contact, const Matrix4 & transform)
{
	m_Transform = transform;
	m_pCollObj = collObj;
	m_pMeshObj = meshObj;
	m_pContact = contact;
}

void GameObject::Update(float deltaTime)
{
	//m_pMeshObj->Update(deltaTIme);
	//m_pCollObj->Update(deltaTime);
//	m_pCollObj->SetPosition(SIMDVector3(m_Transform.getTranslateX(), m_Transform.getTranslateY(), m_Transform.getTranslateZ())); // temp
	m_pCollObj->translate(getTranslate());
}

void GameObject::collision(const GameObject * gameObj)
{
	if (!m_pContact)
		m_pContact = new Collide();

	m_pContact->collision(this->m_pCollObj, gameObj->m_pCollObj);
}

Collide * GameObject::getContact()
{
	return m_pContact;
}

Vector3 GameObject::getTranslate()
{
	Vector3 translate = Vector3(m_Transform.getTranslateX(), m_Transform.getTranslateY(), m_Transform.getTranslateZ());
	return translate;
}

void GameObject::objTranslate()
{
	
}

bool GameObject::isCollided(const GameObject * gameObj)
{
	Collide contact;
	contact.collision(this->m_pCollObj, gameObj->m_pCollObj);
	return contact.getCollide();
}

void GameObject::setTransform(const Matrix4& transform)
{
	m_Transform = transform;
}
