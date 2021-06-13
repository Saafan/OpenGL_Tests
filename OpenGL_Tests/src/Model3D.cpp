#include "Model.h"



Model::Model(std::string path, Shader* shader)
{
	this->shader = shader;
	type = ModelType::Model;
	material = new Material(shader);
	LoadModel(path);

	SetVBAttributes(3, 3, 2);
	ib = new IndexBuffer(&indices[0], indices.size());
	SetVertexBuffer(&vertices[0], vertices.size() * sizeof(float));
	SetTargetBufferObject(GL_TRIANGLES);
}

void Model::LoadModel(std::string& path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_FlipUVs | aiProcess_Triangulate);

	if (!scene || !scene->mRootNode || (scene->mFlags && AI_SCENE_FLAGS_INCOMPLETE))
	{
		std::cout << "Error ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}
	directory = path.substr(0, path.find_last_of('/')) + '/';

	ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
	// process all the node's meshes (if any)
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		ProcessMesh(mesh, scene);
	}
	// then do the same for each of its children
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene);
		std::cout << "INode : " << i << std::endl;

	}
}

void Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	for (size_t i = 0; i < mesh->mNumVertices; i++)
	{
		vertices.push_back(mesh->mVertices[i].x);
		vertices.push_back(mesh->mVertices[i].y);
		vertices.push_back(mesh->mVertices[i].z);
		vertices.push_back(mesh->mNormals[i].x);
		vertices.push_back(mesh->mNormals[i].y);
		vertices.push_back(mesh->mNormals[i].z);
		if (mesh->mTextureCoords[0])
		{
			vertices.push_back(mesh->mTextureCoords[0][i].x);
			vertices.push_back(mesh->mTextureCoords[0][i].y);
		}
		else
		{
			vertices.push_back(0);
			vertices.push_back(0);
		}

	}
	for (size_t i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (size_t j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	if (mesh->mMaterialIndex >= 0)
	{
		ProcessMaterial(scene->mMaterials[mesh->mMaterialIndex], scene);
	}
}

void Model::ProcessMaterial(aiMaterial* mat, const aiScene* scene)
{
	for (size_t currentTypeIndex = 0; currentTypeIndex < 2; currentTypeIndex++)
	{
		aiTextureType type = (aiTextureType)(aiTextureType_DIFFUSE + currentTypeIndex);
		for (size_t i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString str;
			mat->GetTexture(type, i, &str);
			//#TODO Make below more dynamic
			material->AddTexture(directory + str.C_Str(), type, currentTypeIndex * mat->GetTextureCount(type) + i);
		}
	}
}
