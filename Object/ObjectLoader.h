#pragma once
#include <vector>
#include "../Graphics/D3D11Renderer.h"

#define MODEL_PATH "../3DModel/"

struct MTL {
	std::string material_name;
	float NS;
	Vector3 KA;
	Vector3 KD;
	Vector3 KS;
	Vector3 KE;
	Vector3 NI;
	float D;
	float illum;
	std::string texture_filename;
};

class Object {
public:
	Object() : m_hasUV(false) { }
	~Object() { }

	void addVertexIndex() {
		m_vertexIndices.push_back(m_vertexIndices.size());
	}

	void* getVertices() {
		return m_vertices;
	}

	void setVertices(std::vector<Vertex1P>& vertices) {
		m_vertices = new Vertex1P[vertices.size()];
		for (int i = 0; i < vertices.size(); i++) {
			((Vertex1P*) m_vertices)[i].m_pos = vertices.at(i).m_pos;
		}
		m_numVertices = vertices.size();
	}

	void setVertices(std::vector<Vertex1P1UV>& vertices) {
		m_vertices = new Vertex1P1UV[vertices.size()];
		for (int i = 0; i < vertices.size(); i++) {
			((Vertex1P1UV*) m_vertices)[i].m_pos = vertices.at(i).m_pos;
			((Vertex1P1UV*) m_vertices)[i].m_UV[0] = vertices.at(i).m_UV[0];
			((Vertex1P1UV*) m_vertices)[i].m_UV[1] = vertices.at(i).m_UV[1];
		}
		m_numVertices = vertices.size();
	}

	std::vector<unsigned int>* getVertexIndices() {
		return &m_vertexIndices;
	}

	unsigned int getNumVertices() {
		return m_numVertices;
	}

	std::string getObjectName() {
		return m_name;
	}

	std::string getMaterialName() {
		return m_mtlname;
	}

	bool hasUV() {
		return m_hasUV;
	}

	void setHasUV(const bool hasUV) {
		m_hasUV = hasUV;
	}

	void setObjectName(const char* objName) {
		m_name = objName;
	}

	void setMaterialName(const char* mtlName) {
		m_mtlname = mtlName;
	}


private:
	void* m_vertices;
	unsigned int m_numVertices;
	std::vector<unsigned int> m_vertexIndices;
	std::string m_name;
	std::string m_mtlname;
	bool m_hasUV;
};

class ObjectLoader
{
public:
	ObjectLoader(const char* model_name) : m_model_name(model_name) { }
	~ObjectLoader() { 
		m_objects.clear();
		m_materials.clear();
	}

	bool LoadWaveFrontObject(const float scale = 1.0f);
	bool LoadMTL(const char* _filename_);
	void Draw();

private:
	std::vector<Object> m_objects;
	std::vector<MTL> m_materials;
	std::string m_model_name;
};

