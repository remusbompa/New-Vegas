#include "Tema3Object3D.h"
#include <Core/Engine.h>
#include "Tema3.h"

#include <vector>
#include <string>

#include <iostream>
#include <time.h> 


//contruire paralelipiped cu fetele colorate diferit
Mesh* Tema3Object3D::CreateParaleliped(std::string name, glm::vec3 corner, float height, float depth, float width){
	std::vector<glm::vec3> vertices = {};

	vertices.push_back(corner + glm::vec3(0, 0, width));
	vertices.push_back(corner + glm::vec3(height, 0, width));
	vertices.push_back(corner + glm::vec3(0, depth, width));
	vertices.push_back(corner + glm::vec3(height, depth, width));
	vertices.push_back(corner + glm::vec3(0, 0, 0));
	vertices.push_back(corner + glm::vec3(height, 0, 0));
	vertices.push_back(corner + glm::vec3(0, depth, 0));
	vertices.push_back(corner + glm::vec3(height, depth, 0));

	vertices.push_back(corner + glm::vec3(0, 0, width));
	vertices.push_back(corner + glm::vec3(height, 0, width));
	vertices.push_back(corner + glm::vec3(0, depth, width));
	vertices.push_back(corner + glm::vec3(height, depth, width));
	vertices.push_back(corner + glm::vec3(0, 0, 0));
	vertices.push_back(corner + glm::vec3(height, 0, 0));
	vertices.push_back(corner + glm::vec3(0, depth, 0));
	vertices.push_back(corner + glm::vec3(height, depth, 0));

	vertices.push_back(corner + glm::vec3(0, 0, width));
	vertices.push_back(corner + glm::vec3(height, 0, width));
	vertices.push_back(corner + glm::vec3(0, depth, width));
	vertices.push_back(corner + glm::vec3(height, depth, width));
	vertices.push_back(corner + glm::vec3(0, 0, 0));
	vertices.push_back(corner + glm::vec3(height, 0, 0));
	vertices.push_back(corner + glm::vec3(0, depth, 0));
	vertices.push_back(corner + glm::vec3(height, depth, 0));

	std::vector<unsigned short> indices =
	{
		0,9,2, 9,11,2,
		18,19,23, 18,23,22,
		1,15,3, 1,13,15,
		14,7,12, 7,5,12,
		16,20,17, 17,20,21,
		10,6,4, 8,10,4,
	};

	std::vector<glm::vec2> textureCoords = {
		glm::vec2(0,0),
		glm::vec2(0,0),
		glm::vec2(0, depth * pixels_p_unit_h),
		glm::vec2(0, depth * pixels_p_unit_h),
		glm::vec2(0,0),
		glm::vec2(0,0),
		glm::vec2(0, depth * pixels_p_unit_h),
		glm::vec2(0, depth * pixels_p_unit_h),

		glm::vec2(width * pixels_p_unit_w,0),
		glm::vec2(height * pixels_p_unit_w,0),
		glm::vec2(width * pixels_p_unit_w, depth * pixels_p_unit_h),
		glm::vec2(height * pixels_p_unit_w, depth * pixels_p_unit_h),
		glm::vec2(height * pixels_p_unit_w,0),
		glm::vec2(width * pixels_p_unit_w,0),
		glm::vec2(height * pixels_p_unit_w, depth * pixels_p_unit_h),
		glm::vec2(width * pixels_p_unit_w, depth * pixels_p_unit_h),

		glm::vec2(0.5f,0.5f),
		glm::vec2(0.5f,0.5f),
		glm::vec2(0.5f,0.5f),
		glm::vec2(0.5f,0.5f),
		glm::vec2(0.5f,0.5f),
		glm::vec2(0.5f,0.5f),
		glm::vec2(0.5f,0.5f),
		glm::vec2(0.5f,0.5f)
		
	};
	
	std::vector<glm::vec3> normals = {};
	glm::vec3 center = (vertices[0] + vertices[1] + vertices[4] + vertices[5]) / 4.0f + glm::vec3(0, depth / 2, 0);

	for (int i = 0; i < vertices.size(); i++) {
		glm::vec3 position = vertices[i];
		glm::vec3 dif = position - center;
		normals.push_back(glm::normalize(glm::vec3(dif.x, 0, dif.z)));
	}

	Mesh * mesh = new Mesh(name);
	mesh->InitFromData(vertices, normals, textureCoords, indices);
	return mesh;
}

Mesh* Tema3Object3D::CreateQuad(glm::vec3 P1, glm::vec3 P2, glm::vec3 P3, glm::vec3 P4, float depth) {
	std::vector<glm::vec3> vertices = {};

	vertices.push_back(P3);
	vertices.push_back(P1);
	vertices.push_back(P3 + glm::vec3(0, depth, 0));
	vertices.push_back(P1 + glm::vec3(0, depth, 0));

	vertices.push_back(P4);
	vertices.push_back(P2);
	vertices.push_back(P4 + glm::vec3(0, depth, 0));
	vertices.push_back(P2 + glm::vec3(0, depth, 0));

	std::vector<unsigned short> indices =
	{
		0, 1, 2,		1, 3, 2,
		2, 6, 7,		2, 7, 3,
		1, 7, 3,		1, 5, 7,
		6, 7, 4,		7, 5, 4,
		0, 4, 1,		1, 4, 5,
		2, 6, 4,		0, 2, 4,
	};

	std::vector<glm::vec2> textureCoords = {
		glm::vec2(0,0),
		glm::vec2(0,pixels_p_unit_street_y * glm::distance(P3, P1)),
		glm::vec2(0,0),
		glm::vec2(0,pixels_p_unit_street_y * glm::distance(P3, P1)),

		glm::vec2(pixels_p_unit_street_x * glm::distance(P3,P4),0),
		glm::vec2(pixels_p_unit_street_x * glm::distance(P3,P4),pixels_p_unit_street_y * glm::distance(P3, P1)),
		glm::vec2(pixels_p_unit_street_x * glm::distance(P3,P4),0),
		glm::vec2(pixels_p_unit_street_x * glm::distance(P3,P4),pixels_p_unit_street_y * glm::distance(P3, P1)),
	};

	std::vector<glm::vec3> normals = {};

	for (int i = 0; i < vertices.size(); i++) {
		glm::vec3 position = vertices[i];
		normals.push_back(glm::vec3(0,1,0));
	}
	

	Mesh * mesh = new Mesh("");
	mesh->InitFromData(vertices, normals, textureCoords, indices);
	return mesh;
}

//construire sfera
Mesh* Tema3Object3D::CreateSphere(std::string name, glm::vec3 center, float radius, glm::vec3 color) {
	int nrFragments = 100;
	std::vector<VertexFormat> vertices = { VertexFormat(center, color) };
	for (int i = 0; i < nrFragments; i++) {
		float theta = RADIANS((float)(360 * i) / nrFragments);
		
		for (int j = 0; j < nrFragments; j++) {
			float phi = RADIANS((float)(360 * j) / nrFragments);
			float x = radius * cos(phi) * cos(theta);
			float y = radius * cos(phi) * sin(theta);
			float z = radius * sin(phi);
			vertices.push_back(VertexFormat(glm::vec3(center.x + x, center.y + y, center.z + z), color));
		}
		
	}

	std::vector<unsigned short> indices = {};

	//fetele cu acelasi theta
	for (int i = 1; i <= nrFragments; i++) {
		for (int j = 1; j < nrFragments; j++) {
			int nr = (i - 1) * nrFragments + j;
			indices.push_back(0);
			indices.push_back(nr);
			indices.push_back(nr + 1);
		}

		indices.push_back(0);
		indices.push_back(i * nrFragments);
		indices.push_back((i-1) * nrFragments + 1);
	}

	//fetele cu acelasi phi
	for (int j = 1; j <= nrFragments; j++) {
		for (int i = 1; i < nrFragments; i++) {
			int nr = (i - 1) * nrFragments + j;
			indices.push_back(0);
			indices.push_back(nr);
			indices.push_back(nr + nrFragments);
		}

		indices.push_back(0);
		indices.push_back(nrFragments * (nrFragments-1) +j);
		indices.push_back(j);
	}

	Mesh * mesh = new Mesh(name);
	mesh->InitFromData(vertices, indices);
	return mesh;
}

//construire cilindru, cu bottomCenter centrul bazei mici iar radius1 raza mare si radius2 baza mica

Mesh* Tema3Object3D::CreateCylinder(std::string name, glm::vec3 bottomCenter, float radius1, float radius2, float height, int nrFragments, int obj) {

	int cuts = rand() % nrCuts;

	std::vector<int> cutIndices = {};
	for (int i = 0; i < cuts;i++) {
		int ind = 1 + rand() % (nrFragments-1);
		cutIndices.push_back(ind);
	}

	int cutFragments = (int)((float)nrFragments / 360.0f * cutDegree);
	int realNrFragments = 0;

	std::vector<int> realIndices = {0};
	float realStepDown = 2.0f * PI * radius1 / nrFragments ;
	float realStepUp = 2.0f * PI * radius2 / nrFragments;

	//cercul de jos
	std::vector<glm::vec3> vertices = { bottomCenter };
	for (int i = 0; i < nrFragments; i++){
		bool invalid = false;
		for (int j = 0; j < cutIndices.size(); j++) {
			if (i > cutIndices[j] && i < cutIndices[j] + cutFragments) {
				invalid = true;
				break;
			}
		}
		if (invalid) continue;
		float theta = RADIANS((float)(360 * i) / nrFragments);
		float x = radius1 * cos(theta), z = radius1 * sin(theta);
		glm::vec3 newPos = glm::vec3(bottomCenter.x + x, bottomCenter.y, bottomCenter.z + z);
		vertices.push_back(newPos);
		vertices.push_back(newPos);
		vertices.push_back(newPos);

		realIndices.push_back(i);
		realIndices.push_back(i);
		realIndices.push_back(i);
		realNrFragments++;
	 }
	
	//cercul de sus
	glm::vec3 upperCenter = bottomCenter + glm::vec3(0, height, 0);

	
	for (int i = 0; i < nrFragments; i++) {
		bool invalid = false;
		for (int j = 0; j < cutIndices.size(); j++) {
			if (i > cutIndices[j] && i < cutIndices[j] + cutFragments) {
				invalid = true;
				break;
			}
		}
		if (invalid) continue;
		float theta = RADIANS((float)(360 * i) / nrFragments);
		float x = radius2 * cos(theta), z = radius2 * sin(theta);
		glm::vec3 newPos = glm::vec3(upperCenter.x + x, upperCenter.y, upperCenter.z + z);
		vertices.push_back(newPos);
		vertices.push_back(newPos);
		vertices.push_back(newPos);

		realIndices.push_back(nrFragments + i);
		realIndices.push_back(nrFragments + i);
		realIndices.push_back(nrFragments + i);
	}

	vertices.push_back(upperCenter);


	std::vector<glm::vec2> textureCoords = { glm::vec2(0.5f, 0.5f) };

	float dist = 0;
	for (int i = 0; i < realNrFragments; i++){
		textureCoords.push_back(glm::vec2(0.5f, 0.5f));
		
		
		float difDist;
		if(i == 0) difDist = 0;
		else difDist = getRealDistance(realIndices, i - 1, i, realStepDown);
		

		dist += difDist;
		textureCoords.push_back(glm::vec2(Tema3Object3D::pixels_p_unit_w * dist, 0));
		textureCoords.push_back(glm::vec2(Tema3Object3D::pixels_p_unit_w * dist, 0));
	}

	{
		float difDist = (nrFragments - 1 - realIndices[3 * (realNrFragments - 1) + 1]) + realIndices[1] + 1;
		difDist *= realStepDown;
		dist += difDist;
		textureCoords[3] = glm::vec2(Tema3Object3D::pixels_p_unit_w * dist, 0);
	}
	
	dist = 0;
	for (int i = 0; i < realNrFragments; i++) {
		textureCoords.push_back(glm::vec2(0.5f, 0.5f));

		float height1 = glm::distance(vertices[3 * (realNrFragments + i ) + 1], vertices[3 * i  + 1]);

		

		float difDist;
		if (i == 0) difDist = 0;
		else difDist = getRealDistance(realIndices, realNrFragments + i - 1, realNrFragments + i, realStepUp);
		dist += difDist;

		textureCoords.push_back(glm::vec2(Tema3Object3D::pixels_p_unit_w * dist, height1 * Tema3Object3D::pixels_p_unit_h));
		textureCoords.push_back(glm::vec2(Tema3Object3D::pixels_p_unit_w * dist, height1 * Tema3Object3D::pixels_p_unit_h));
	}

	{
		float height1 = glm::distance(vertices[3 * realNrFragments + 1], vertices[1]);
		float difDist = (2 * nrFragments - 1 - realIndices[3 * (2 * realNrFragments - 1)+ 1]) + realIndices[3 * realNrFragments + 1] - nrFragments + 1;
		difDist *= realStepUp;
		dist += difDist;
		textureCoords[3 * realNrFragments + 3] = glm::vec2(Tema3Object3D::pixels_p_unit_w * dist, height1 * Tema3Object3D::pixels_p_unit_h);
	}

	textureCoords.push_back(glm::vec2(0.5f, 0.5f));

	std::vector<unsigned short> indices = {};

	//primul cerc
	for (int i = 1; i < realNrFragments; i++){
		indices.push_back(0);
		indices.push_back(3 * (i-1) + 1);
		indices.push_back(3 * i + 1);
	}

	indices.push_back(0);
	indices.push_back(3 * (realNrFragments -1) +1);
	indices.push_back(1);

	//al doilea cerc
	for (int i = 1; i < realNrFragments; i++){
		indices.push_back(6 * realNrFragments + 1);
		indices.push_back(3 * (realNrFragments + i - 1) + 1);
		indices.push_back(3 * (realNrFragments + i) + 1);
	}
	indices.push_back(6 * realNrFragments + 1);
	indices.push_back(3 * (2 * realNrFragments - 1) + 1);
	indices.push_back(3 * realNrFragments + 1);

	//fete laterale

	

	for (int i = 1; i < realNrFragments; i++) {
		indices.push_back(3 * (realNrFragments + i - 1) +2);
		indices.push_back(3 * (i-1) + 2);
		indices.push_back(3 * i + 3);

		indices.push_back(3 * i + 3);
		indices.push_back(3 * (realNrFragments + i) + 3);
		indices.push_back(3 * (realNrFragments + i - 1) + 2);

	}

	indices.push_back(3 * (2 * realNrFragments - 1) + 2);
	indices.push_back(3 * (realNrFragments - 1) + 2);
	indices.push_back(3);

	indices.push_back(3);
	indices.push_back(3 * realNrFragments + 3);
	indices.push_back(3 * (2 * realNrFragments - 1) + 2);

	std::vector<glm::vec3> normals = {};
	float center_height = (radius1 * radius1 - radius2 * radius2 + height * height) / (2 * height);
	glm::vec3 center = bottomCenter + glm::vec3(0, center_height, 0);

	for (int i = 0; i < vertices.size(); i++) {
		glm::vec3 position = vertices[i];
		glm::vec3 dif = position - center;
		normals.push_back(glm::normalize(glm::vec3(dif.x, 0, dif.z)));
	}
	Mesh * mesh = new Mesh(name);
	mesh->InitFromData(vertices, normals, textureCoords, indices);
	return mesh;
}

//creare dreptunghi, de-a lungul axelor: normal si tangent
Mesh* Tema3Object3D::CreateRect(std::string name, glm::vec3 leftBottomCorner, glm::vec3 normal, glm::vec3 tang, float lengthN, float lengthT, glm::vec3 color)
{
	glm::vec3 corner = leftBottomCorner;

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner, color),
		VertexFormat(corner + tang * lengthT, color),
		VertexFormat(corner + tang * lengthT + normal * lengthN, color),
		VertexFormat(corner - tang * lengthT + normal * lengthN, color)
	};

	Mesh* rectangle = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2, 3 };


	// draw 2 triangles. Add the remaining 2 indices
	indices.push_back(0);
	indices.push_back(2);

	rectangle->InitFromData(vertices, indices);
	return rectangle;
}

std::vector<Mesh*> Tema3Object3D::CreateBlockyBuilding(glm::vec3 P) {

	std::vector<Mesh*> meshes = {};
	Mesh* first = CreateParaleliped("", P + glm::vec3(-radius, 0, -radius), radius, maxHeight, radius);
	meshes.push_back(first);
	glm::vec3 P1 = first->positions[0];
	glm::vec3 P2 = first->positions[1];
	glm::vec3 P3 = first->positions[5];
	glm::vec3 P4 = first->positions[4];

	int nr = rand() % maxBlocks;

	for (int i = 0; i < nr; i++) {
		glm::vec3 sel = Tema3::RandomPoint(P1, P2, P3, P4);
		int m = i % 4;
		if (m == 0) {
			float height = Tema3::RandomFloat(minHeight, maxHeight);
			Mesh* mesh = CreateParaleliped("", sel, widthBlock, height, -bigRadius);
			meshes.push_back(mesh);
		}else if (m == 1) {
			float height = Tema3::RandomFloat(minHeight, maxHeight);
			Mesh* mesh = CreateParaleliped("", sel, bigRadius, height, widthBlock);
			meshes.push_back(mesh);
		}
		else if (m == 2) {
			float height = Tema3::RandomFloat(minHeight, maxHeight);
			Mesh* mesh = CreateParaleliped("", sel, widthBlock, height, bigRadius);
			meshes.push_back(mesh);
		}
		else if (m == 3) {
			float height = Tema3::RandomFloat(minHeight, maxHeight);
			Mesh* mesh = CreateParaleliped("", sel, -bigRadius, height, widthBlock);
			meshes.push_back(mesh);
		}
	}
	return meshes;
}

float Tema3Object3D::getRealDistance(std::vector<int> realIndices, int prevInd, int nextInd, float realStep) {
	int difInd;

	difInd = realIndices[3 * prevInd + 1] - realIndices[3 * nextInd + 1];

	return realStep * difInd;
}

Mesh* Tema3Object3D::CreateCylinderNormal(std::string name, glm::vec3 bottomCenter, float radius1, float radius2, float height, glm::vec3 color) {
	int nrFragments = 100;

	//cercul de jos
	std::vector<VertexFormat> vertices = { VertexFormat(bottomCenter, color) };
	for (int i = 0; i < nrFragments; i++) {
		float theta = RADIANS((float)(360 * i) / nrFragments);
		float y = radius2 * cos(theta), z = radius2 * sin(theta);
		vertices.push_back(VertexFormat(glm::vec3(bottomCenter.x, bottomCenter.y + y, bottomCenter.z + z), color));
	}
	//cercul de sus
	glm::vec3 upperCenter = bottomCenter + glm::vec3(-height, 0, 0);

	for (int i = 0; i < nrFragments; i++) {
		float theta = RADIANS((float)(360 * i) / nrFragments);
		float y = radius1 * cos(theta), z = radius1 * sin(theta);
		vertices.push_back(VertexFormat(glm::vec3(upperCenter.x, upperCenter.y + y, upperCenter.z + z), color));
	}

	vertices.push_back(VertexFormat(upperCenter, color));

	std::vector<unsigned short> indices = {};

	//primul cerc
	for (int i = 1; i < nrFragments; i++) {
		indices.push_back(0);
		indices.push_back(i);
		indices.push_back(i + 1);
	}

	indices.push_back(0);
	indices.push_back(nrFragments);
	indices.push_back(1);

	//al doilea cerc
	for (int i = 1; i < nrFragments; i++) {
		indices.push_back(2 * nrFragments + 1);
		indices.push_back(nrFragments + i);
		indices.push_back(nrFragments + i + 1);
	}

	indices.push_back(2 * nrFragments + 1);
	indices.push_back(2 * nrFragments);
	indices.push_back(nrFragments + 1);

	//fete laterale

	for (int i = 1; i < nrFragments; i++) {
		indices.push_back(nrFragments + i);
		indices.push_back(i);
		indices.push_back(i + 1);

		indices.push_back(i + 1);
		indices.push_back(nrFragments + i + 1);
		indices.push_back(nrFragments + i);
	}

	indices.push_back(2 * nrFragments);
	indices.push_back(nrFragments);
	indices.push_back(1);

	indices.push_back(1);
	indices.push_back(nrFragments + 1);
	indices.push_back(2 * nrFragments);

	Mesh * mesh = new Mesh(name);
	mesh->InitFromData(vertices, indices);
	return mesh;
}