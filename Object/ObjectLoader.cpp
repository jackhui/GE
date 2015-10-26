#include "ObjectLoader.h"

bool ObjectLoader::LoadMTL(const char* _filename_) {
	std::string placeholder = (std::string(MODEL_PATH) + m_model_name + std::string("/") + std::string(_filename_));
	const char* filename = placeholder.c_str();
	FILE * file = fopen(filename, "r");
	if (file == NULL) {
		printf("Error in open the file !\n");
		return false;
	}

	char lineHeader[128], mtl_name[128], temps[128];
	float tempf[3];

	while (true) {
		if (strcmp(lineHeader, "newmtl") == 0) {
			fscanf(file, "%s\n", mtl_name);

			MTL mtl;
			mtl.material_name = mtl_name;

			while (fscanf(file, "%s", lineHeader) != EOF && strcmp(lineHeader, "newmtl") != 0) {
				if (strcmp(lineHeader, "Ns") == 0) {
					fscanf(file, "%f\n", &mtl.NS);
				}
				else if (strcmp(lineHeader, "Ka") == 0) {
					fscanf(file, "%f %f %f\n", &tempf[0], &tempf[1], &tempf[2]);
					mtl.KA = Vector3(tempf[0], tempf[1], tempf[2]);
				}
				else if (strcmp(lineHeader, "Kd") == 0) {
					fscanf(file, "%f %f %f\n", &tempf[0], &tempf[1], &tempf[2]);
					mtl.KD = Vector3(tempf[0], tempf[1], tempf[2]);
				}
				else if (strcmp(lineHeader, "Ks") == 0) {
					fscanf(file, "%f %f %f\n", &tempf[0], &tempf[1], &tempf[2]);
					mtl.KS = Vector3(tempf[0], tempf[1], tempf[2]);
				}
				else if (strcmp(lineHeader, "Ke") == 0) {
					fscanf(file, "%f %f %f\n", &tempf[0], &tempf[1], &tempf[2]);
					mtl.KE = Vector3(tempf[0], tempf[1], tempf[2]);
				}
				else if (strcmp(lineHeader, "Ni") == 0) {
					fscanf(file, "%f\n", &mtl.NI);
				}
				else if (strcmp(lineHeader, "d") == 0) {
					fscanf(file, "%f\n", &mtl.D);
				}
				else if (strcmp(lineHeader, "illum") == 0) {
					fscanf(file, "%f\n", &mtl.illum);
				}
				else if (strcmp(lineHeader, "map_Kd") == 0) {
					fscanf(file, "%s", temps);
					int found = std::string(temps).find_last_of("/\\");
					if (found == -1) {
						mtl.texture_filename = temps;
					}
					else {
						mtl.texture_filename = std::string(temps).substr(found + 1);
					}
				}
			}

			m_materials.push_back(mtl);
		}
		else if (fscanf(file, "%s", lineHeader) == EOF) {
			break;
		}
	}

	return m_materials.size() > 0;
}

bool ObjectLoader::LoadWaveFrontObject(const float scale) {
	std::string placeholder = std::string(MODEL_PATH) + std::string(m_model_name) + std::string("/") + std::string(m_model_name) + std::string(".obj");
	const char* filename = placeholder.c_str();
	FILE * file = fopen(filename, "r");
	if (file == NULL) {
		printf("Error in open the file !\n");
		return false;
	}

	std::vector<Vertex1P> tempVertices1P;
	std::vector<Vertex1P1UV> tempVertices1P1UV;
	std::vector<Vector3> tempVertices;
	std::vector<Vector3> tempNormals;
	std::vector<std::vector<float>> tempUVs;

	char lineHeader[128], temps[128];
	float tempf[3];
	unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
	unsigned int vertex_offset = 0; 
	unsigned int uv_offset = 0; 
	unsigned int normal_offset = 0;
	unsigned int vertexCount = 0;

	while (true) {
		if (strcmp(lineHeader, "mtllib") == 0) {
			fscanf(file, "%s\n", temps);
			if (LoadMTL(temps) == false) {
				break;
			}
			lineHeader[0] = 'a';
		}
		else if (strcmp(lineHeader, "o") == 0) {
			fscanf(file, "%s\n", temps);
			Object object;
			object.setObjectName(temps);

			while (fscanf(file, "%s", lineHeader) != EOF && strcmp(lineHeader, "o") != 0 ) {
				if (strcmp(lineHeader, "v") == 0) {
					fscanf(file, "%f %f %f\n", &tempf[0], &tempf[1], &tempf[2]);
					tempVertices.push_back(Vector3(tempf[0] * scale, tempf[1] * scale, tempf[2] * scale));
				}
				else if (strcmp(lineHeader, "vt") == 0) {
					fscanf(file, "%f %f %f\n", &tempf[0], &tempf[1], &tempf[2]);
					std::vector<float> temp;
					temp.push_back(tempf[0]);
					temp.push_back(1.0f - tempf[1]);	//Transform the origin from bottom left to top left
					tempUVs.push_back(temp);
					object.setHasUV(true);
				}
				else if (strcmp(lineHeader, "vn") == 0) {
					fscanf(file, "%f %f %f\n", &tempf[0], &tempf[1], &tempf[2]);
					tempNormals.push_back(Vector3(tempf[0], tempf[1], tempf[2]));
				}
				else if (strcmp(lineHeader, "usemtl") == 0) {
					fscanf(file, "%s\n", temps);
					object.setMaterialName(temps);
				}
				else if (strcmp(lineHeader, "f") == 0) {
					bool is_pattern_match = false;
					if (object.hasUV() > 0) {
						is_pattern_match = (fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]) == 9);
					} else {
						is_pattern_match = (fscanf(file, "%d//%d %d//%d %d//%d\n", &vertexIndex[0], &normalIndex[0], &vertexIndex[1], &normalIndex[1], &vertexIndex[2], &normalIndex[2]) == 6);
					}

					if (!is_pattern_match) {
						printf("File can't be read by parser");
						return false;
					}

					//C++ indexing starts at 0 and OBJ indexing starts at 1
					
					for (int i = 0; i < 3; i++) {
						if (object.hasUV()) {
							float uv[2] = {
								tempUVs.at(uvIndex[i] - uv_offset - 1).at(0),
								tempUVs.at(uvIndex[i] - uv_offset - 1).at(1)
							};
							tempVertices1P1UV.push_back(Vertex1P1UV{
								tempVertices.at(vertexIndex[i] - vertex_offset - 1),
								uv
							});
						}
						else {
							tempVertices1P.push_back(Vertex1P {
								tempVertices.at(vertexIndex[i] - vertex_offset - 1)
							});
						}

						object.addVertexIndex();
					}					
				}
			}
			
			if (object.hasUV()) {
				object.setVertices(tempVertices1P1UV);
			} else {
				object.setVertices(tempVertices1P);
			}

			vertex_offset += tempVertices.size();
			normal_offset += tempNormals.size();
			uv_offset += tempUVs.size();
			m_objects.push_back(object);
			OutputDebugString((object.getObjectName() + "\n").c_str());

			tempVertices1P1UV.clear();
			tempVertices1P.clear();
			tempVertices.clear();
			tempNormals.clear();
			tempUVs.clear();
			
		} else if (fscanf(file, "%s", lineHeader) == EOF) {
			break;
		}
	}

	return true;
}

void ObjectLoader::Draw() {

	std::string texture_path;

	for (std::vector<Object>::iterator itr_o = m_objects.begin(); itr_o != m_objects.end(); itr_o++) {
		for (std::vector<MTL>::iterator itr_m = m_materials.begin(); itr_m != m_materials.end(); itr_m++) {
			if (itr_o->getMaterialName() == itr_m->material_name) {
				texture_path = std::string(MODEL_PATH) + m_model_name + std::string("/Texture/") + std::string(itr_m->texture_filename);

				D3D11Renderer::GetInstance()->GetMeshInstanceList().push_back(new MeshInstance(
					itr_o->getVertices(),
					itr_o->getNumVertices(),
					itr_o->getVertexIndices()->data(),
					itr_o->getVertexIndices()->size(),
					itr_o->hasUV() ? RenderType::V1P1UV : RenderType::V1P,
					D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
					itr_o->hasUV() ? texture_path.c_str() : NULL
				));
			}
		}
	}
}