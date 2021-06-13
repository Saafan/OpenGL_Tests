#include "Material.h"


Material::Material(Shader* shader, float shininess)
{
	this->shininess = shininess;
	this->shader = shader;
}

void Material::SetShininess(float shininess)
{
	this->shininess = shininess;
}

void Material::AddTexture(std::string path, aiTextureType type, unsigned int slot)
{
	Texture* texture = nullptr;
	if (type == aiTextureType_DIFFUSE)
		texture = new Texture(path, type, true);
	else
		texture = new Texture(path, type);
	texture->SetSlot(slot);
	textures.push_back(texture);
}

void Material::BindMaterial(std::string materialDiffuseName, std::string materialSpecularName, std::string materialShininessName)
{
	if (!shader)
		return;
	int diffuseI = 1;
	int specularI = 1;
	int reflectionI = 1;
	for (size_t i = 0; i < textures.size(); i++)
	{
		textures[i]->SetSlot(i + 1);
		if (textures[i]->type == aiTextureType_DIFFUSE)
			shader->SetUniform1i("material.diffuse[" + std::to_string(diffuseI) + "]", textures[i]->slot);
		else if (textures[i]->type == aiTextureType_SPECULAR)
			shader->SetUniform1i("material.specular[" + std::to_string(specularI) + "]", textures[i]->slot);
		else if (textures[i]->type == aiTextureType_REFLECTION)
			shader->SetUniform1i("material.reflection[" + std::to_string(reflectionI) + "]", textures[i]->slot);
		textures[i]->Bind();
	}
	//#TODO Implement storing more than one variable while binding
	if (materialShininessName.empty())
		shader->SetUniform1f("material.shininess", shininess);
	else
		shader->SetUniform1f(materialShininessName, shininess);
	shader->SetUniform1f("material.reflectivity", reflectivity);
}

