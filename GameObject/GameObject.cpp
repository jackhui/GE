#include "GameObject.h"
/**
GameObject::GameObject(CollidableObject * collObj, MeshInstance * meshObj, Collide* contact, const Matrix4 & transform)
{
	m_Transform = transform;
	m_pCollObj = collObj;
	m_pMeshObj = meshObj;
	m_pContact = contact;
}
*/
GameObject::GameObject(Body * collObj, MeshInstance * meshObj, Collide * contact, const Matrix4 & transform, const int& gameObjID)
{
	m_pBody	=			collObj;
	m_pMeshObj =		meshObj;
	m_pContact =		contact;
	m_Transform =		transform;
	m_GameObjectID =	gameObjID;
	GameWorld::GetInstance()->GetGameObjectList().push_back(this);
}

void GameObject::Update(float deltaTime)
{
	//m_pMeshObj->Update(deltaTIme);
	m_pBody->update(deltaTime, getTranslate());
}

void GameObject::collision(const GameObject * gameObj)
{
	if (!m_pContact)
		m_pContact = new Collide();

	m_pContact->collision(this->m_pBody, gameObj->m_pBody);
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
	contact.collision(this->m_pBody, gameObj->m_pBody);
	return contact.getCollide();
}

void GameObject::setTransform(const Matrix4& transform)
{
	m_Transform = transform;
}
