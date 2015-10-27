

#ifndef CDBODY_H
#define CDBODY_H

#include "cdCollide.h"
#include "../Math/simdmath.h"

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
	SIMDVector3 getCenter() const;
	virtual void computeAABB(const SIMDMatrix4& transform) {}
	virtual void translate(const SIMDVector3& translate) {}

private:
	int m_pType;
};



#endif