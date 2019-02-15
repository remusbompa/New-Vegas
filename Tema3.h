#pragma once
#include <Component/SimpleScene.h>
#include <Core/GPU/Mesh.h>
#include <Component/Transform/Transform.h>
#include "LabCamera.h"

class Intersection {
public:
	glm::vec3 P;
	glm::vec3 dir;
	Intersection(glm::vec3 P, glm::vec3 dir) {
		this->P = P;
		this->dir = dir;
	}
};
class Tema3 : public SimpleScene
{
public:
	Tema3();
	~Tema3();

	void Init() override;

private:
	void FrameStart() override;
	void Update(float deltaTimeSeconds) override;
	void FrameEnd() override;

	void OnInputUpdate(float deltaTime, int mods) override;
	void OnKeyPress(int key, int mods) override;
	void OnKeyRelease(int key, int mods) override;
	void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
	void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
	void OnWindowResize(int width, int height) override;

	void RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 & modelMatrix, int obj, Texture2D* textureStreet, Texture2D* textureBuilding,
		bool light, std::vector<glm::vec3> spotLight_position, glm::vec3 dirLight_position, glm::vec3 lightDirection);
	Texture2D* CreateWindowsTexture();

public:
	Tema3Camera::Camera* camera;
	float heightCamera = 8.0f;
	glm::mat4 projectionMatrix;
	glm::mat4 View;
	float FOV = RADIANS(60);
	std::unordered_map<std::string, Texture2D*> mapTextures;
	GLuint randomTextureID;

	unsigned int pixels_height = 1024;//in numar de pixeli
	unsigned int pixels_width = 1024;

	unsigned int resX;
	unsigned int resY;
	float minStreetLength = 200.0f;
	float maxStreetLength = 600.0f;
	float minStreetWidth = 3.0f;
	float maxStreetWidth = 6.0f;
	int nrIntersections = 25;
	float street_depth = 0.25f;
	static const unsigned int precision = 1000;

	float minBetweenInters = 50.0f;

	std::vector<Mesh*> streetQuads = {};
	std::vector<glm::vec3> intersections = {};
	Mesh* generateStreetQuad(glm::vec3 Inters, glm::vec3 oldDir, glm::vec3* newDir);
	Texture2D* CreateStreetTexture();
	glm::uvec3 streetColor = glm::uvec3(105,105,105);

	glm::uvec3 windowNoise = glm::vec3(30, 30, 30);

	float minStreetTheta = 80.0f;
	float maxStreetTheta = 90.0f;

	static float RandomFloat(float min, float max);

	std::vector<std::vector<Mesh*>> buildingQuads = {};
	float distanceStreetBuilding = 10.0f;
	float distanceInterBuilding = 1.0f;

	bool Tema3::intersStreet(glm::vec3 P, Mesh* mesh, float dist);
	int nrBuildings = 800;

	static glm::vec3 RandomPoint(glm::vec3 P1, glm::vec3 P2);
	static glm::vec3 RandomPoint(glm::vec3 P1, glm::vec3 P2, glm::vec3 P3, glm::vec3 P4);

	bool intersBuilding(glm::vec3 P, std::vector<Mesh*> block, float radius);

	bool outOfRange(glm::vec3 P);

	void intersStreets(Mesh* s1, glm::vec3 Ps21, glm::vec3 Ps22, glm::vec3 Ps23, glm::vec3 Ps24, glm::vec3* P, bool* vid);
	unsigned int nrIncercari_per_el_stiva = 30;

	bool intersTree(glm::vec3 P);
	float distBuildingTree = 9.0f;
	float distStreetTree = 0.25f;

	int nrTrees = 800;
	std::vector<glm::mat4> treeList = {};

	unsigned int materialShininess = 40;
	float materialKd = 0.5f;
	float materialKs = 0.5f;

	float spotAngle = 60.0f;

	std::vector<glm::vec3> getLightPosition(std::vector<Mesh*> block, int type);
	float spotHeight = 18.0f;

	glm::vec3 dirLight_position = glm::vec3(1, 1, 1);

	std::vector<std::vector<glm::vec3>> spotLightPositions = {};

	int nrLights = 10;

	std::vector<glm::mat4> semaforList = {};
	int nrSemafoare = 15;

	void RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 & modelMatrix);
	std::vector<glm::vec3>semaforPoints = {};
};
