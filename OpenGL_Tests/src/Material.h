#pragma once
#include "Texture.h"
#include "assimp/scene.h"
#include <iostream>

class Material
{
public:
	Material(Shader* shader = nullptr, float shininess = 64.0f);
	void SetShininess(float shininess);

	void AddTexture(std::string path, aiTextureType type, unsigned int slot);
	void BindMaterial(std::string materialDiffuseName = "", std::string materialSpecularName = "", std::string materialShininessName = "");

private:
	std::vector<Texture*> textures;
	Shader* shader = nullptr;
	float shininess = 64.0f;
	float reflectivity = 1.0f;
};
