//#pragma once
//#include <string>
//#include <vector>
//#include <sstream>
//#include <string>
//#include <fstream>
//#include <iostream>
//
//struct Vertex
//{
//	float x = 0.0f;
//	float y = 0.0f;
//	float z = 0.0f;
//};
//
//struct Index
//{
//	unsigned int x = 0.0f;
//	unsigned int y = 0.0f;
//	unsigned int z = 0.0f;
//	unsigned int w = 0.0f;
//};
//
//void ParseOBJFile(std::string path)
//{
//	std::ifstream objFile(path);
//	std::string line;
//	std::vector<Vertex> vertices;
//	std::vector<Vertex> verticesNormal;
//	std::vector<Vertex> verticesTexture;
//	std::vector<Index> indices;
//	while (std::getline(objFile, line))
//	{
//		std::vector<std::string> vertexEntity(3);
//		std::vector<std::string> indexEntity(4);
//
//		static int j = 0;
//
//		if (line[0] == 'f')
//		{
//
//			bool start = true;
//			static std::string currentIndex;
//			for (size_t i = 2; i < line.length() - 1; i++)
//			{
//				if (line[i] == '/')
//				{
//					if (start)
//						indexEntity[j] = currentIndex;
//					start = false;
//					currentIndex = "";
//				}
//				if (line[i] == ' ')
//				{
//					start = true;
//					j++;
//					continue;
//				}
//				if (start)
//					currentIndex += line[i];
//			}
//			j = 0;
//			indices.push_back(Index{ std::stoul(indexEntity[0]),std::stoul(indexEntity[1]),std::stoul(indexEntity[2]),((indexEntity[3]).empty()) ? 0 : std::stoul(indexEntity[3]) });
//		}
//
//		if (line[0] == 'v')
//		{
//			//Vertex
//			for (size_t i = 3; i < line.length(); i++)
//			{
//				static std::string currentFloat;
//				if (line[i] == ' ' || i == line.length() - 1)
//				{
//					vertexEntity[j] = currentFloat;
//					j++;
//					currentFloat = "";
//					continue;
//				}
//				currentFloat += line[i];
//			}
//
//			if (line[1] == 't')
//				verticesTexture.push_back(Vertex{ std::stof(vertexEntity[0]), std::stof(vertexEntity[1]) });
//			else if (line[1] == 'n')
//				verticesNormal.push_back(Vertex{ std::stof(vertexEntity[0]), std::stof(vertexEntity[1]), std::stof(vertexEntity[2]) });
//			else
//				vertices.push_back(Vertex{ std::stof(vertexEntity[0]), std::stof(vertexEntity[1]), std::stof(vertexEntity[2]) });
//			j = 0;
//		}
//	}
//	for (size_t i = 0; i < verticesTexture.size(); i++)
//	{
//		std::cout << vertices[i].x << ", " << vertices[i].y << ", " << vertices[i].z << ", " << verticesTexture[i].x << ", " << verticesTexture[i].y << ", " << verticesNormal[i].z << ", " << verticesTexture[i].x << ", " << verticesNormal[i].y << ", " << verticesNormal[i].z << ", " << std::endl;
//	}
//	std::cout << "\n\n\n\n\n Index:" << std::endl;
//	for (size_t i = 0; i < indices.size(); i++)
//	{
//		std::string wIndex = (indices[i].w != 0) ? (std::to_string(indices[i].w - 1)) + ", " : "";
//		std::cout << indices[i].x - 1 << ", " << indices[i].y - 1 << ", " << indices[i].z - 1 << ", " << wIndex << std::endl;
//	}
//}
