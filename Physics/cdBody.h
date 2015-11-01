

#ifndef CDBODY_H
#define CDBODY_H

#include "cdCollide.h"
#include "../Math/simdmath.h"

typedef SIMDVector3 Vector3;
typedef SIMDMatrix4 Matrix4;

enum
{
	typeSPHERE,
	typeAABB,
	typePOINT,
	typeRAY,
	typePLANE,

	typeCount
};

class Body
{
public:
	Body()
	{
		m_pType = -1;
	}

	Body(const int type)
	{
		m_pType = type;
	}

	int getType() const { return m_pType; }
	void setType(const int type) { m_pType = type; }
	Vector3 getCenter() const;
	virtual void computeAABB(const Matrix4& transform) {}
	virtual void translate(const Vector3& translate) {}

private:
	int m_pType;
};



#endif