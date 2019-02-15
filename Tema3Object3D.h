#pragma once

#include <string>

#include <include/glm.h>
#include <Core/GPU/Mesh.h>
#include "Tema3.h"

/*
	Fisierul defineste functiile de construire a obiectelor si de detectare a coliziunilor.
*/
namespace Tema3Object3D
{
	Mesh* CreateParaleliped(std::string name, glm::vec3 corner, float height, float depth, float width);

	Mesh* CreateSphere(std::string name, glm::vec3 center, float radius, glm::vec3 color);

	Mesh* CreateCylinder(std::string name, glm::vec3 bottomCenter, float radius1, float radius2, float height, int nrFragments, int obj);

	Mesh* CreateRect(std::string name, glm::vec3 leftBottomCorner, glm::vec3 normal, glm::vec3 tang, float lengthN, float lengthT, glm::vec3 color);

	Mesh* CreateQuad(glm::vec3 P1, glm::vec3 P2, glm::vec3 P3, glm::vec3 P4, float depth);

	static float min_height = 0.5f;
	static float pixels_p_unit_w = 1.0f;
	static float pixels_p_unit_h = 2.0f;

	static float pixels_p_unit_street_x = 3.0f;
	static float pixels_p_unit_street_y = 15.0f;

	std::vector<Mesh*> CreateBlockyBuilding(glm::vec3 P);
	static float maxHeight = 30.0f;
	static float minHeight = 10.0f;
	static float radius = 3.0f;
	static float bigRadius = 6.0f;
	static float widthBlock = 2.0f;
	static int maxBlocks = 5;

	static int nrCuts = 4;
	static float cutDegree = 90.0f;

	float getRealDistance(std::vector<int> realIndices, int prevInd, int nextInd, float realStep);

	constexpr float PI = 3.1415926535f;

	Mesh* CreateCylinderNormal(std::string name, glm::vec3 bottomCenter, float radius1, float radius2, float height, glm::vec3 color);
}

