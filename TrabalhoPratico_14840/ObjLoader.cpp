//http://www.opengl-tutorial.org/beginners-tutorials/tutorial-7-model-loading/

#include "Header.h"

const char * loadOBJ(
			const char * path,
			std::vector<glm::vec3>& out_vertices,
			std::vector<glm::vec2>& out_uvs,
			std::vector<glm::vec3>& out_normals
		) {
			char mtl[30];
			printf("Loading OBJ file:\n%s...\n", path);

			std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
			std::vector<glm::vec3> temp_vertices;
			std::vector<glm::vec2> temp_uvs;
			std::vector<glm::vec3> temp_normals;


			FILE * file = fopen(path, "r");
			if (file == NULL) {
				printf("Impossible to open the file! File maybe doeesn't exist?\n");
				getchar();
				return false;
			}

			while (1) {
				char lineHeader[128];
				int res = fscanf(file, "%s", lineHeader);				//read the first word of the line
				if (res == EOF)
					break;		//End Of File, quit the loop.

				if (strcmp(lineHeader, "mtllib") == 0) {
					fscanf(file, "%s\n", &mtl);
				}
				else if (strcmp(lineHeader, "v") == 0) {
					glm::vec3 vertex;
					fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
					temp_vertices.push_back(vertex);
				}
				else if (strcmp(lineHeader, "vt") == 0) {
					glm::vec2 uv;
					fscanf(file, "%f %f\n", &uv.x, &uv.y);
					temp_uvs.push_back(uv);
				}
				else if (strcmp(lineHeader, "vn") == 0) {
					glm::vec3 normal;
					fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
					temp_normals.push_back(normal);
				}
				else if (strcmp(lineHeader, "f") == 0) {
					unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
					int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
					if (matches != 9) {
						printf("File can't be read\n");
						fclose(file);
						return false;
					}
					vertexIndices.push_back(vertexIndex[0]);
					vertexIndices.push_back(vertexIndex[1]);
					vertexIndices.push_back(vertexIndex[2]);
					uvIndices.push_back(uvIndex[0]);
					uvIndices.push_back(uvIndex[1]);
					uvIndices.push_back(uvIndex[2]);
					normalIndices.push_back(normalIndex[0]);
					normalIndices.push_back(normalIndex[1]);
					normalIndices.push_back(normalIndex[2]);
				}
				else {
					//Caso apanhe um comment...
					char coise[1000];
					fgets(coise, 1000, file);
				}
			}

			//For each vertex of each triangle
			for (unsigned int i = 0; i < vertexIndices.size(); i++) {

				//Get the indices of its attributes
				unsigned int vertexIndex = vertexIndices[i];
				unsigned int uvIndex = uvIndices[i];
				unsigned int normalIndex = normalIndices[i];

				//Get the attributes thanks to the index
				glm::vec3 vertex = temp_vertices[vertexIndex - 1];
				glm::vec2 uv = temp_uvs[uvIndex - 1];
				glm::vec3 normal = temp_normals[normalIndex - 1];

				//Put the attributes in buffers
				out_vertices.push_back(vertex);
				out_uvs.push_back(uv);
				out_normals.push_back(normal);

			}
			fclose(file);
			return mtl;
		}

bool loadMTL(const char * path, glm::vec3& ka, glm::vec3& kd, glm::vec3& ks, float& ns, float& ni, float& d, int& illum) {
	FILE * file = fopen(path, "r");

	if (file == NULL) {
		printf("Impossible to open the file! File maybe doeesn't exist?\n");
		getchar();
		return NULL;
	}

	while (1) {
		char lineHeader[128];
		int res = fscanf(file, "%s", lineHeader);		//read the first word of the line
		if (res == EOF)
			break;										//End Of File, quit the loop.

		//NS --> Specular exponent (ranges between 0 and 1000)
		//Controls the glossiness in the phong shading model
		if (strcmp(lineHeader, "Ns") == 0) {
			fscanf(file, "%f\n", &ns);
		}
		else if (strcmp(lineHeader, "Ka") == 0) {
			fscanf(file, "%f %f %f\n", &ka.x, &ka.y, &ka.z);
		}
		else if (strcmp(lineHeader, "Kd") == 0) {
			fscanf(file, "%f %f %f\n", &kd.x, &kd.y, &kd.z);
		}
		else if (strcmp(lineHeader, "Ks") == 0) {
			fscanf(file, "%f %f %f\n", &ks.x, &ks.y, &ks.z);
		}
		else if (strcmp(lineHeader, "Ni") == 0) {
			fscanf(file, "%f\n", &ni);
		}
		//d --> dissolve (0.0 - transparent | 1.0 - fully opaque)
		else if (strcmp(lineHeader, "d") == 0) {
			fscanf(file, "%f\n", &d);
		}
		//illum
		//	0. Color on and Ambient off
		//	1. Color on and Ambient on
		//	2. Highlight on
		//	3. Reflection on and Ray trace on
		//	4. Transparency: Glass on, Reflection : Ray trace on
		//	5. Reflection : Fresnel on and Ray trace on
		//	6. Transparency : Refraction on, Reflection : Fresnel off and Ray trace on
		//	7. Transparency : Refraction on, Reflection : Fresnel on and Ray trace on
		//	8. Reflection on and Ray trace off
		//	9. Transparency : Glass on, Reflection : Ray trace off
		//	10. Casts shadows onto invisible surfaces
		else if (strcmp(lineHeader, "illum") == 0) {
			fscanf(file, "%d\n", &illum);
		}
		else if (strcmp(lineHeader, "map_Kd") == 0) {
			//const char * hum;
			//fscanf(file, "%f\n", &lol);		
		}
		else {
			//Caso apanhe um comment...
			char coise[1000];
			fgets(coise, 1000, file);
		}
	}

	fclose(file);
	return true;
}