#include "Tema3.h"
#include "Tema3Object3D.h"

#include <vector>
#include <string>
#include <iostream>

#include <Core/Engine.h>
#include "Transform3D.h"
#include<time.h>
#include<stack>
#include<random>

using namespace std;

Tema3::Tema3()
{
}

Tema3::~Tema3()
{
}

void Tema3::Init()
{
	glm::ivec2 resolution = window->GetResolution();
	resX = resolution.x;
	resY = resolution.y;

	{Mesh* mesh = new Mesh("tree");
	mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives/ALan", "AlanTree.fbx");
	meshes[mesh->GetMeshID()] = mesh;
	}
	
	{Mesh* mesh = new Mesh("moon");
	mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives/Moon_3D_Model", "moon.obj");
	meshes[mesh->GetMeshID()] = mesh;
	}

	{Mesh* mesh = new Mesh("semafor");
	mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives","street-light-traffic.3ds.obj");
	meshes[mesh->GetMeshID()] = mesh;
	}

	{
		float max = 100;
		glm::vec3 P1 = glm::vec3(600, max, -600);
		glm::vec3 P2 = glm::vec3(600, max, 600);
		glm::vec3 P3 = glm::vec3(-600, max, -600);
		glm::vec3 P4 = glm::vec3(-600, max, 600);
		Mesh* mesh = Tema3Object3D::CreateQuad(P1, P2, P3, P4, 20.0f);
		meshes["horizon"] = mesh;
	}

	mapTextures["street"] = CreateStreetTexture();
	mapTextures["windows"] = CreateWindowsTexture();
	
	// Create a shader program for drawing face polygon with the color of the normal
	{
		Shader *shader = new Shader("ShaderTema3");
		shader->AddShader("Source/Laboratoare/Tema3/Shaders/VertexShader.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/Tema3/Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	//creeate streets network
	
	srand((unsigned int)time(0));
	
	int nr = 1;
	glm::vec3 P = glm::vec3(0, 0, 0);
	glm::vec3 dir = glm::normalize(glm::vec3(RandomFloat(1.0f, 5.0f), 0, RandomFloat(1.0f, 5.0f)));

	glm::vec3 newDir;
	Mesh* ales = generateStreetQuad(P, dir, &newDir);
	dir = newDir;
	GetSceneCamera()->SetPosition(P + glm::vec3(0, street_depth + 0.25f, 0));

	std::stack<Intersection> oldInfo = {};
	
	
	while (nr <= nrIntersections) {
		unsigned int nrIncercari = nrIncercari_per_el_stiva;
		cout << "Intersectie: " << P << "\n";
		intersections.push_back(P);
		
		oldInfo.push(Intersection(P, dir));

		bool oprireFortata = false;
		while (true) {
			if (oldInfo.size() == 0) {
				cout << "S au incercat puncte pe toate diretiile si s-a iesit din rezolutie!\n";
				oprireFortata = true;
				break;
			}

			bool primaInfo = true;
			
			Intersection inter = oldInfo.top();

			nrIncercari--;
			cout << "---Incercarea nr " << (nrIncercari_per_el_stiva - nrIncercari) << " cu " << inter.P << " si " << inter.dir << "\n";
			if (nrIncercari == 0) {
				oldInfo.pop();
				nrIncercari = nrIncercari_per_el_stiva;
			}
			P = inter.P;
			dir = inter.dir;

			Mesh* ales = generateStreetQuad(P, dir, &newDir);
			if (ales == NULL) continue;
			dir = newDir;


			glm::vec3 P1 = ales->positions[1];
			glm::vec3 P2 = ales->positions[5];
			glm::vec3 P3 = ales->positions[0];
			glm::vec3 P4 = ales->positions[4];

			dir = glm::normalize(P1 - P3);
			glm::vec3 tg = glm::normalize(P1 - P2);

			int nrIncercariP = 20;
			bool ok;
			do {
				ok = true;
				float x = RandomFloat(0, glm::distance(P1, P3));
				float y = RandomFloat(0, glm::distance(P1, P2));
				P = P3 + x * dir + y * tg;

				cout << "Selectat punct: " << P << "\n";

				if (outOfRange(P)) {
					primaInfo = false;
					ok = false;
					cout << "fac break primaInfo !\n";
					break;
				}

				for (int i = 0; i < intersections.size(); i++) {
					if (glm::distance(intersections[i], P) < minBetweenInters) {
						
						nrIncercariP--;
						if (nrIncercariP == 0) {
							ok = false;
						}
						cout << "fac break ok!\n";
						break;
					}
				}

				
			} while (ok);
			if (primaInfo) break;
		}

		if (oprireFortata) break;
		nr++;
	}

	for (int i = 0; i < nrBuildings;i++) {
		float x, y;
		while (true) {
			x = RandomFloat(-(float)resX / 2, (float)resX / 2);
			y = RandomFloat(-(float)resY / 2, (float)resY / 2);

			cout << "Cladire " <<(i+1) << ": "<< glm::vec3(x, 0, y) << "\n";
			bool ok = true;
			for (int j = 0; j < streetQuads.size();j++) {
				if (intersStreet(glm::vec3(x, 0, y), streetQuads[j], distanceStreetBuilding)) {
					ok = false;
					break;
				}
			}

			if (ok) {
				for (int j = 0; j < buildingQuads.size();j++) {
					if (intersBuilding(glm::vec3(x, 0, y), buildingQuads[j], Tema3Object3D::radius + Tema3Object3D::bigRadius)) {
						ok = false;
						break;
					}
				}
			}

			if (ok) break;
		}

		if (i == 0) {
			//GetSceneCamera()->SetPosition(glm::vec3(x, 0, y) + glm::vec3(5.0f, 0, 0));

		}

		int random = rand() % 2;
		if (random == 0) {
			std::vector<Mesh*> list = Tema3Object3D::CreateBlockyBuilding(glm::vec3(x, 0, y));
			std::vector<glm::vec3> lightPosition = getLightPosition(list, 0);
			spotLightPositions.push_back(lightPosition);
			buildingQuads.push_back(list);
		}
		else if (random == 1) {
			float radius = RandomFloat(Tema3Object3D::radius, Tema3Object3D::bigRadius);
			Mesh* mesh = Tema3Object3D::CreateCylinder("", glm::vec3(x, 0, y), radius, radius, Tema3Object3D::maxHeight, 360, 0);
			std::vector<Mesh*> list = { mesh };
			std::vector<glm::vec3> lightPosition = getLightPosition(list, 1);
			spotLightPositions.push_back(lightPosition);
			buildingQuads.push_back(list);
		}
		
	}

	float x, y, z;
	for (int i = 0; i < nrTrees; i++) {
		cout << "Copac " << i << "\n";
		glm::mat4 matrix = glm::mat4(1);
		glm::vec3 P;
		while (true) {
			//cout << "odata\n";
			x = RandomFloat(-(float)resX / 2, (float)resX / 2);
			y = RandomFloat(-(float)resY / 2, (float)resY / 2);
			P = glm::vec3(x, 0, y);

			if (!intersTree(P)) break;
		}
		matrix *= Transform3D::Translate(P.x, P.y, P.z);
		float theta = RandomFloat(0, 360);
		matrix *= Transform3D::RotateOX(RADIANS(-90));
		matrix *= Transform3D::Scale(0.25f, 0.25f, 0.25f);
		//matrix *= Transform3D::RotateOY(RADIANS(theta));
		treeList.push_back(matrix);
	}

	{
		for (int i = 0; i < nrSemafoare; i++) {
			cout << "Semafor " << i << "\n";
			glm::mat4 matrix = glm::mat4(1);
			glm::vec3 P;
			//while (true) {
				//cout << "odata\n";
			int nr = intersections.size();
			int random = rand() % nr;
			float theta = RandomFloat(0, 360);

			P = intersections[i] + glm::vec3(cos(RADIANS(theta)), 0, sin(RADIANS(theta)));

			//if (!intersS(P)) break;
		//}
			semaforPoints.push_back(P);
			matrix *= Transform3D::Translate(P.x, P.y, P.z);
			theta = RandomFloat(0, 360);
			//matrix *= Transform3D::RotateOX(RADIANS(-90));
			//matrix *= Transform3D::RotateOY(RADIANS(theta));
			matrix *= Transform3D::Scale(0.1f, 0.1f, 0.1f);
			semaforList.push_back(matrix);
		}
	}

	glm::vec3 P1 = glm::vec3((float)resX / 2, -street_depth, (float)resY / 2);
	glm::vec3 P2 = glm::vec3((float)resX / 2, -street_depth, -(float)resY / 2);
	glm::vec3 P3 = glm::vec3(-(float)resX / 2, -street_depth, (float)resY / 2);
	glm::vec3 P4 = glm::vec3(-(float)resX / 2, -street_depth, -(float)resY / 2);
	Mesh* grass = Tema3Object3D::CreateQuad(P1, P2, P3, P4, street_depth);
	meshes["grass"] = grass;

	{
		const string textureLoc = "Source/Laboratoare/Laborator9/Textures/";
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "grass_bilboard.png").c_str(), GL_REPEAT);
		mapTextures["tree"] = texture;
	}

	{
		const string textureLoc = "Resources/Textures/";
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "grass.jpg").c_str(), GL_REPEAT);
		mapTextures["grass"] = texture;
	}

	{
		const string textureLoc = "Resources/Textures/";
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "moonmap2k.jpg").c_str(), GL_REPEAT);
		mapTextures["moon"] = texture;
	}

	{
		const string textureLoc = "Resources/Textures/";
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "milky-way.jpg").c_str(), GL_REPEAT);
		mapTextures["horizon"] = texture;
	}
}

void Tema3::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	
	glViewport(0, 0, resolution.x, resolution.y);
}

void Tema3::Update(float deltaTimeSeconds)
{
	{
		for (int i = 0; i < buildingQuads.size(); i++) {
			std::vector<Mesh*> list = buildingQuads[i];
			std::vector<glm::vec3> lightPos = spotLightPositions[i];
			for (int i = 0; i < list.size(); i++) {
				RenderSimpleMesh(list[i], shaders["ShaderTema3"], glm::mat4(1), 1, mapTextures["street"], mapTextures["windows"],
					true, lightPos, dirLight_position, glm::vec3(0, -1, 0));
			}
		}
	}
	
	for (int i = 0; i < streetQuads.size(); i++) {
		RenderSimpleMesh(streetQuads[i], shaders["ShaderTema3"], glm::mat4(1), 0, mapTextures["street"], mapTextures["windows"],
			false, {}, dirLight_position, glm::vec3(0, 0, 0));
	}

	for (int i = 0; i < treeList.size(); i++) {
		//RenderSimpleMesh(meshes["tree"], shaders["VertexNormal"], treeList[i]);
		RenderSimpleMesh(meshes["tree"], shaders["ShaderTema3"], treeList[i], 0, mapTextures["tree"], mapTextures["windows"],
			false, {}, dirLight_position, glm::vec3(0, 0, 0));
	}

	RenderSimpleMesh(meshes["grass"], shaders["ShaderTema3"], glm::mat4(1), 0, mapTextures["grass"], mapTextures["windows"],
		false, {}, dirLight_position, glm::vec3(0, 0, 0));

	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix *= Transform3D::Translate(50.0f, 100.0f, 50.0f);
		modelMatrix *= Transform3D::RotateOX(RADIANS(180));
		modelMatrix *= Transform3D::Scale(0.1f, 0.1f, 0.1f);
		RenderSimpleMesh(meshes["moon"], shaders["ShaderTema3"], modelMatrix, 0, mapTextures["moon"], mapTextures["windows"],
			false, {}, dirLight_position, glm::vec3(0, 0, 0));
	}

	{
		glm::mat4 modelMatrix = glm::mat4(1);
		RenderSimpleMesh(meshes["horizon"], shaders["ShaderTema3"], modelMatrix, 3, mapTextures["horizon"], mapTextures["windows"],
			false, {}, dirLight_position, glm::vec3(0, 0, 0));
	}

	for (int i = 0; i < nrSemafoare; i++) {
		glm::mat4 matrix = glm::mat4(1);
		glm::vec3 P = semaforPoints[i];
		matrix *= Transform3D::Translate(P.x, P.y, P.z);
		matrix *= Transform3D::RotateOZ(-RADIANS(90));
		Mesh* cyl = Tema3Object3D::CreateCylinderNormal("", glm::vec3(0,0,0), 0.10f, 0.10f, 8.5f, glm::vec3(0,1,0));
		RenderSimpleMesh(cyl, shaders["ShaderTema3"], matrix, 4, mapTextures["street"], mapTextures["windows"],
			false, {}, dirLight_position, glm::vec3(0, 0, 0));
		RenderSimpleMesh(meshes["semafor"], shaders["VertexNormal"], semaforList[i]);
	}
}

void Tema3::FrameEnd()
{
	DrawCoordinatSystem();
}

void Tema3::OnInputUpdate(float deltaTime, int mods)
{
	float speed = 2;

	if (!window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
	{
		glm::vec3 up = glm::vec3(0, 1, 0);
		glm::vec3 right = GetSceneCamera()->transform->GetLocalOXVector();
		glm::vec3 forward = GetSceneCamera()->transform->GetLocalOZVector();
		forward = glm::normalize(glm::vec3(forward.x, 0, forward.z));
	}
}

void Tema3::OnKeyPress(int key, int mods)
{
	
}

void Tema3::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void Tema3::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	
}

void Tema3::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	
}

void Tema3::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	
}

void Tema3::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema3::OnWindowResize(int width, int height)
{
}

void Tema3::RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 & modelMatrix, int obj, Texture2D* textureStreet, Texture2D* textureBuilding,
	bool light, std::vector<glm::vec3> spotLightPosition, glm::vec3 dirLight_position , glm::vec3 lightDirection)
{

	if (!mesh || !shader || !shader->GetProgramID()) {
		if (!mesh) std::cout << "no mesh \n";
		if (!shader) std::cout << "no shader \n";
		return;
	}


	// render an object using the specified shader and the specified position
	glUseProgram(shader->program);

	// Bind model matrix
	GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
	glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	// Bind view matrix
	glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
	int loc_view_matrix = glGetUniformLocation(shader->program, "View");
	glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	// Bind projection matrix
	glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
	int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
	glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	glUniform1i(glGetUniformLocation(shader->program, "obj"), obj);


	if (textureStreet)
	{
		//TODO : activate texture location 0
		glActiveTexture(GL_TEXTURE0);
		//TODO : Bind the texture1 ID
		glBindTexture(GL_TEXTURE_2D, textureStreet->GetTextureID());
		//TODO : Send texture uniform value
		glUniform1i(glGetUniformLocation(shader->program, "textureStreet"), 0);
	}

	if (textureBuilding)
	{
		//TODO : activate texture location 1
		glActiveTexture(GL_TEXTURE1);
		//TODO : Bind the texture2 ID
		glBindTexture(GL_TEXTURE_2D, textureBuilding->GetTextureID());
		//TODO : Send texture uniform value
		glUniform1i(glGetUniformLocation(shader->program, "textureBuilding"), 1);
	}

	// Set shader uniforms for light & material properties
	// TODO: Set light position uniform
	int loc;

	if (light) {
		loc = glGetUniformLocation(shader->program, "spotLight_position");
		glUniform3fv(loc, spotLightPosition.size(), reinterpret_cast<GLfloat *>(&spotLightPosition[0]));

		loc = glGetUniformLocation(shader->program, "nr_spots");
		glUniform1i(loc, spotLightPosition.size());
	}

	

	loc = glGetUniformLocation(shader->program, "dirLight_position");
	glUniform3f(loc, dirLight_position.x, dirLight_position.y, dirLight_position.z);

	loc = glGetUniformLocation(shader->program, "light_direction");
	glUniform3f(loc, lightDirection.x, lightDirection.y, lightDirection.z);

	// TODO: Set eye position (camera position) uniform
	glm::vec3 eyePosition = GetSceneCamera()->transform->GetWorldPosition();
	int eye_position = glGetUniformLocation(shader->program, "eye_position");
	glUniform3f(eye_position, eyePosition.x, eyePosition.y, eyePosition.z);

	// TODO: Set material property uniforms (shininess, kd, ks, object color) 
	int material_shininess = glGetUniformLocation(shader->program, "material_shininess");
	glUniform1i(material_shininess, materialShininess);

	int material_kd = glGetUniformLocation(shader->program, "material_kd");
	glUniform1f(material_kd, materialKd);

	int material_ks = glGetUniformLocation(shader->program, "material_ks");
	glUniform1f(material_ks, materialKs);

	int ans = glGetUniformLocation(shader->program, "spot");
	glUniform1i(ans, (light == true ? 1 : 0));

	ans = glGetUniformLocation(shader->program, "spot_angle");
	glUniform1f(ans, spotAngle);

	// Draw the object
	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}

Texture2D* Tema3::CreateWindowsTexture()
{
	unsigned int width = pixels_width;
	unsigned int height = pixels_height;
	GLuint textureID = 0;
	unsigned int channels = 3;
	unsigned int size = width * height * channels;
	unsigned char* data = new unsigned char[size];

	for (unsigned int nr = 0; nr < size; nr += 3) {
		unsigned int i = nr / height;
		unsigned int j = nr % height;
		int probab = rand() % 100;

		if (i >= height / 5 &&
			j >= width / 5) {
			

			if (probab <= 50) {
				data[nr] = windowNoise.r;
				data[nr + 1] = windowNoise.g;
				data[nr + 2] = windowNoise.b;
			}
			else if (probab <= 75) {
				data[nr] = 255;
				data[nr + 1] = 255;
				data[nr + 2] = 255;
			}
			else {
				int nuanta_gri = rand() % 255;
				data[nr] = nuanta_gri;
				data[nr + 1] = nuanta_gri;
				data[nr + 2] = nuanta_gri;
			}
			
		}
		else {
			data[nr] = 0;
			data[nr + 1] = 0;
			data[nr + 2] = 0;
		}
	}

	// Generate and bind the new texture ID
	unsigned int gl_texture_object;
	glGenTextures(1, &gl_texture_object);
	glBindTexture(GL_TEXTURE_2D, textureID);

	//cout << "Textura mea building:" << gl_texture_object << "\n";

	// TODO: Set the texture parameters (MIN_FILTER, MAG_FILTER and WRAPPING MODE) using glTexParameteri
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4);

	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	CheckOpenGLError();

	// TODO: Use glTextImage2D to set the texture data
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	// TODO: Generate texture mip-maps
	glGenerateMipmap(GL_TEXTURE_2D);
	//glGenerateTextureMipmap(textureID);

	CheckOpenGLError();

	// Save the texture into a wrapper Texture2D class for using easier later during rendering phase
	Texture2D* texture = new Texture2D();

	//textureID = gl_texture_object;

	texture->Init(textureID, width, height, channels);

	SAFE_FREE_ARRAY(data);
	glDisable(GL_TEXTURE_2D);
	return texture;
}

Mesh* Tema3::generateStreetQuad(glm::vec3 Inters, glm::vec3 oldDir, glm::vec3* newDir) {

	glm::vec3 P1, P2, P3, P4;
	int nrIncercari = 20;
	while (true) {
		bool ok = true;

		nrIncercari--;
		if (nrIncercari == 0) {
			return NULL;
		}
		float theta = RandomFloat(minStreetTheta, maxStreetTheta);
		//cout << "theta: " << theta << "\n";

		float d1 = RandomFloat(minStreetLength, maxStreetLength);

		float d2 = RandomFloat(minStreetLength, maxStreetLength);

		float w = RandomFloat(minStreetWidth, maxStreetWidth);

		glm::vec3 dir = glm::mat3(Transform3D::RotateOY(RADIANS(theta))) * oldDir;
		*newDir = dir;
		//cout << "oldDir dir: :" << oldDir << " " << dir << "\n";
		glm::vec3 ox1 = glm::vec3(1, 0, 0);
		glm::vec3 tang = ox1 - dir * glm::dot(ox1, dir);
		if (tang.x < 1.0f / precision && tang.z < 1.0f / precision) {
			tang = glm::vec3(0, 0, -1);
		}
		else {
			tang = glm::normalize(tang);
		}

		P1 = Inters + dir * d1 + (w / 2) * tang;
		P2 = Inters + dir * d1 - (w / 2) * tang;
		P3 = Inters - dir * d2 + (w / 2) * tang;
		P4 = Inters - dir * d2 - (w / 2) * tang;

		for (int i = 0; i < streetQuads.size();i++) {
			glm::vec3 Inters2;
			bool vid;
			intersStreets(streetQuads[i], P1, P2, P3, P4, &Inters2, &vid);
			if (!vid) {
				intersections.push_back(Inters2);
				if (glm::distance(Inters2, Inters) < minBetweenInters)
					ok = false;
			}
		}

		if (ok) break;
	}
	Mesh* mesh = Tema3Object3D::CreateQuad(P1, P2, P3, P4, street_depth);
	streetQuads.push_back(mesh);
	return mesh;
 }

Texture2D* Tema3::CreateStreetTexture()
{
	unsigned int width = pixels_width;
	unsigned int height = pixels_height;
	GLuint textureID = 1;
	unsigned int channels = 3;
	unsigned int size = width * height * channels;
	unsigned char* data = new unsigned char[size];

	for (unsigned int nr = 0; nr < size; nr += 3) {
		unsigned int i = nr / height;
		unsigned int j = nr % height;

		if ( (i >= height / 5 ) &&
			(j >= width / 20 ) ) {
			data[nr] = streetColor.x;
			data[nr + 1] = streetColor.y;
			data[nr + 2] = streetColor.z;
		}
		else {
			data[nr] = 0;
			data[nr + 1] = 0;
			data[nr + 2] = 0;
		}
	}

	// Generate and bind the new texture ID
	unsigned int gl_texture_object;
	glGenTextures(1, &gl_texture_object);
	//cout << "Textura mea street:" << gl_texture_object << "\n";

	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4);


	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	CheckOpenGLError();

	// TODO: Use glTextImage2D to set the texture data
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	// TODO: Generate texture mip-maps
	glGenerateMipmap(GL_TEXTURE_2D);
	//glGenerateTextureMipmap(textureID);

	CheckOpenGLError();

	// Save the texture into a wrapper Texture2D class for using easier later during rendering phase
	Texture2D* texture = new Texture2D();

	//textureID = gl_texture_object;

	texture->Init(textureID, width, height, channels);

	SAFE_FREE_ARRAY(data);
	glDisable(GL_TEXTURE_2D);
	return texture;
}

float Tema3::RandomFloat(float min, float max) {
	std::mt19937 rng;
	rng.seed(std::random_device()());
	//cout << "min " << min << "max " << max << "\n";
	std::uniform_int_distribution<long> dis(0, (long)((max - min) * precision)); // distribution in range [1, 6]
	float a;
	float x = (float)((a = dis(rng)) / precision) + min;
	//cout << "a = " << a << "\n";
	return x;
}

glm::vec3 Tema3::RandomPoint(glm::vec3 P1, glm::vec3 P2) {
	float minX = MIN(P1.x, P2.x);
	float maxX = MAX(P1.x, P2.x);

	float minY = MIN(P1.y, P2.y);
	float maxY = MAX(P1.y, P2.y);

	float minZ = MIN(P1.z, P2.z);
	float maxZ = MAX(P1.z, P2.z);

	float x;
	if (minX == maxX) x = minX;
	else x = RandomFloat(minX, maxX);

	float y;
	if (minY == maxY) y = minY;
	else y = RandomFloat(minY, maxY);

	float z;
	if (minZ == maxZ) z = minZ;
	else z = RandomFloat(minZ, maxZ);
	glm::vec3 P = glm::vec3(x, y, z);
	//cout << "s-a ales punctul: " << P << "\n";
	return P;
}

glm::vec3 Tema3::RandomPoint(glm::vec3 P1, glm::vec3 P2, glm::vec3 P3, glm::vec3 P4) {
	glm::vec3 first = RandomPoint(P1, P2);
	glm::vec3 second = RandomPoint(P3, P4);
	glm::vec3 res = RandomPoint(first, second);
	return res;
}

bool Tema3::intersStreet(glm::vec3 P, Mesh* mesh, float dist) {
	glm::vec3 P1 = mesh->positions[1];
	glm::vec3 P2 = mesh->positions[5];
	glm::vec3 P3 = mesh->positions[0];
	glm::vec3 P4 = mesh->positions[4];

	glm::vec3 dir = glm::normalize(P3 - P1);
	glm::vec3 tg = glm::normalize(P2 - P1);

	glm::vec3 vect = P - P1;
	float vect_tg = glm::dot(vect, tg);
	float vect_dir = glm::dot(vect, dir);

	float dist_dir = glm::distance(P1, P3);
	float dist_tg = glm::distance(P1, P2);

	if (vect_dir >= -dist && vect_dir <= dist_dir + dist) {
		if (vect_tg >= -dist && vect_tg <= dist_tg + dist) {
			return true;
		}
	}

	return false;
}

bool Tema3::intersBuilding(glm::vec3 P, std::vector<Mesh*> block, float radius) {
	Mesh* base = block[0];

	glm::vec3 P1 = base->positions[0];
	glm::vec3 P2 = base->positions[1];
	glm::vec3 P3 = base->positions[5];
	glm::vec3 P4 = base->positions[4];

	glm::vec3 M = (P1 + P2 + P3 + P4) / 4.0f;
	if (glm::distance(M, P) <= 2.0f * (radius)) {
		return true;
	}

	return false;
}

 void Tema3::intersStreets(Mesh* s1, glm::vec3 Ps21, glm::vec3 Ps22, glm::vec3 Ps23, glm::vec3 Ps24, glm::vec3* P, bool* vid) {
	glm::vec3 Ps1 = s1->positions[1];
	glm::vec3 Ps2 = s1->positions[5];
	glm::vec3 Ps3 = s1->positions[0];
	glm::vec3 Ps4 = s1->positions[4];

	glm::vec3 P1 = (Ps1 + Ps2) / 2.0f;
	glm::vec3 Pn2 = (Ps3 + Ps4) / 2.0f;
	glm::vec3 dir1 = glm::normalize(P1 - Pn2);


	glm::vec3 P2 = (Ps21 + Ps22) / 2.0f;
	Pn2 = (Ps23 + Ps24) / 2.0f;
	glm::vec3 dir2 = glm::normalize(P2 - Pn2);

	glm::vec3 dP = (P2 - P1);

	if (dir1.x * dir2.z == dir1.z * dir2.x) {
		*vid = true;
	}
	else {
		float t1 = (dir1.z * dP.x - dir1.x * dP.z) / (dir1.x * dir2.z - dir1.z * dir2.x);
		*P = P1 + t1 * dir1;
		*vid = false;
	}
}

bool Tema3::outOfRange(glm::vec3 P) {
	if (P.x < (float)resX / 2 && P.x > -(float)resX / 2) {
		if (P.y < (float)resY / 2 && P.y > -(float)resY / 2) {
			return false;
		}
	}
	return true;
}

bool Tema3::intersTree(glm::vec3 P) {
	for (int i = 0; i < buildingQuads.size(); i++) {
		if (intersBuilding(P, buildingQuads[i], distBuildingTree)) return true;
	}

	for (int i = 0; i < streetQuads.size(); i++) {
		if (intersStreet(P, streetQuads[i], distStreetTree)) return true;
	}

	return false;
}

std::vector<glm::vec3> Tema3::getLightPosition(std::vector<Mesh*> block, int type) {
	Mesh* base = block[0];

	glm::vec3 P1 = base->positions[0];
	glm::vec3 P2 = base->positions[1];
	glm::vec3 P3 = base->positions[5];
	glm::vec3 P4 = base->positions[4];

	glm::vec3 M;

	if (type == 0) {
		M = (P1 + P2 + P3 + P4) / 4.0f;
	}
	else {
		M = P1;
	}

	std::vector<glm::vec3> list = {};

	int nr = 1 + rand() % nrLights;
	for (int i = 0; i < nr; i++) {
		int random = rand() % 4;
		glm::vec3 P;

		float height = RandomFloat(0, Tema3Object3D::maxHeight + 2.0f);
		if (random == 0) {
			P = M + Tema3Object3D::bigRadius * glm::vec3(1, height, 0);
		}
		else if (random == 1) {
			P = M + Tema3Object3D::bigRadius * glm::vec3(-1, height, 0);
		}
		else if (random == 2) {
			P = M + Tema3Object3D::bigRadius * glm::vec3(0, height, 1);
		}
		else if (random == 3) {
			P = M + Tema3Object3D::bigRadius * glm::vec3(0, height, -1);
		}
		list.push_back(P);
	}

	//P = M + glm::vec3(0, Tema3Object3D::maxHeight + 2.0f, 0);
	//cout << P1 << " " << P2 << " " << P3 << " " << P4 << " " << P << "\n";

	return list;
}

void Tema3::RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 & modelMatrix)
{
	if (!mesh || !shader || !shader->GetProgramID())
		return;

	// render an object using the specified shader and the specified position
	glUseProgram(shader->program);

	// TODO : get shader location for uniform mat4 "Model"
	int location = glGetUniformLocation(shader->GetProgramID(), "Model");


	// TODO : set shader uniform "Model" to modelMatrix
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	// TODO : get shader location for uniform mat4 "View"
	location = glGetUniformLocation(shader->GetProgramID(), "View");

	// TODO : set shader uniform "View" to viewMatrix
	glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	// TODO : get shader location for uniform mat4 "Projection"
	location = glGetUniformLocation(shader->GetProgramID(), "Projection");

	// TODO : set shader uniform "Projection" to projectionMatrix
	glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	location = glGetUniformLocation(shader->GetProgramID(), "time");
	float time = (float)Engine::GetElapsedTime();
	glUniform1f(location, time);

	// Draw the object
	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}