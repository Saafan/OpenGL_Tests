#include "Model.h"
#include "Camera.h"

/// <summary>MyMethod is a method in the MyClass class.
///<para>The Type of the Model You want to create </para>
///<para>The Vertex Data you want to input</para>
///<para>The Byte Size of the Vertex Data (Leave ZERO for auto use of sizeof)</para>
///<para>number of axis in the Vertex Data (1D, 2D or 3D)</para>
///<para>number of axis of normals in the Vertex Data(2D normals or 3D | Leave ZERO if No input exists)</para>
///<para>number of axis of Texture Coordinates (Leave ZERO if No input exists)</para>
/// </summary>
Model::Model(ModelType type, Shader* shader, Material* material, float data[], unsigned int size, unsigned int axisNum, unsigned int normalsNum, unsigned int textureCoordNum, GLenum targetBufferObject)
{
	this->type = type;
	this->material = material;
	this->shader = shader;
	this->targetBufferObject = targetBufferObject;
	if (type == ModelType::Model)
	{
		if (data)
			this->data = data;

		if (size == 0)
			this->dataSize = sizeof(data);
		else
			this->dataSize = size;

		if (axisNum != 0)
		{

			vb = new VertexBuffer(data, this->dataSize);

			vbl = new VertexBufferLayout();
			if (axisNum != 0)
				vbl->Push<float>(axisNum);

			if (normalsNum != 0)
				vbl->Push<float>(normalsNum);

			if (textureCoordNum != 0)
				vbl->Push<float>(textureCoordNum);
			va = new VertexArray();
			va->AddBuffer(*vb, *vbl);
			UpdateIndiciesNum();
			std::cout << "indices Number: " << indicesNum << std::endl;
		}
	}

	switch (this->type)
	{
	case ModelType::Triangle: SetTriangleParam();
		break;
	case ModelType::Plane: SetPlaneParam();
		break;
	case ModelType::Cube: SetCubeParam();
		break;
	case ModelType::Pyramid: SetPyramidParam();
		break;
	default:
		break;
	case ModelType::GroundPlane: SetGroundPlaneParam();
		break;
	}

	if (material)
		material->BindMaterial();
}

void Model::UpdateIndiciesNum()
{
	if (dataSize != 0.0f)
		indicesNum = (dataSize / sizeof(float)) / (axisNum + normalsNum + textureCoordNum);
	else
		std::cout << "There's no Data in the Vertex Buffer!" << std::endl;
}

void Model::SetVertexBuffer(const float data[], const unsigned int size)
{
	if (data == nullptr)
		return;
	this->data = data;
	if (size == 0)
		this->dataSize = sizeof(data);
	else
		this->dataSize = size;

	delete vb;
	delete va;

	vb = new VertexBuffer(data, dataSize);
	va = new VertexArray();

	UpdateIndiciesNum();

	vbl = new VertexBufferLayout();
	vbl->Push<float>(axisNum);
	vbl->Push<float>(normalsNum);
	vbl->Push<float>(textureCoordNum);

	va->AddBuffer(*vb, *vbl);
}

void Model::SetVBData(const float data[])
{
	this->data = data;
}

void Model::SetVBSize(const unsigned int size)
{
	this->dataSize = size;
}

void Model::SetVBAttributes(const unsigned int axisNum, const  unsigned int normalsNum, const  unsigned int textureCoordNum)
{
	this->axisNum = axisNum;
	this->normalsNum = normalsNum;
	this->textureCoordNum = textureCoordNum;
}

void Model::SetShader(Shader& shader)
{
	this->shader = &shader;
}

void Model::SetMaterial(Material& material)
{
	this->material = &material;
}

void Model::ClearMaterial()
{
	this->material = nullptr;
}

void Model::SetTargetBufferObject(const GLenum target)
{
	targetBufferObject = target;
}

glm::mat4& Model::GetModelMatrix()
{
	return model;
}

ModelMatrices& Model::GetDecomposedMatrix()
{
	return *modelMatrices;
}

void Model::RenderLine(const glm::vec3 startPoint, const glm::vec3 endPoint, float lineWidth)
{
	glm::mat4 tmp(1.0f);
	shader->SetModelMatrix(tmp);
	glLineWidth(lineWidth);
	glBegin(GL_LINES);
	glVertex3f(startPoint.x, startPoint.y, startPoint.z);
	glVertex3f(endPoint.x, endPoint.y, endPoint.z);
	glEnd();
	shader->SetModelMatrix(GetModelMatrix());
}

void Model::RenderWireCone(const glm::vec3 startPoint, const glm::vec3 endPoint, float angle, float lineWidth)
{
	glm::mat4 tmp(1.0f);
	shader->SetModelMatrix(tmp);
	glLineWidth(lineWidth);
	glBegin(GL_LINES);
	const int coneWiresNum = 8;
	for (size_t i = 0; i < coneWiresNum; i++)
	{
		const float hypo = glm::distance(startPoint, endPoint);
		const float radius = hypo * std::sin(glm::radians(angle));
		glVertex3f(startPoint.x, startPoint.y, startPoint.z);
		glVertex3f(endPoint.x + std::cos(glm::radians(i * (360.0f / coneWiresNum))) * radius, endPoint.y, endPoint.z + std::sin(glm::radians(i * (360.0f / coneWiresNum))) * radius);

		glVertex3f(endPoint.x + std::cos(glm::radians(i * (360.0f / coneWiresNum))) * radius, endPoint.y, endPoint.z + std::sin(glm::radians(i * (360.0f / coneWiresNum))) * radius);
		glVertex3f(endPoint.x + std::cos(glm::radians((i + 1) * (360.0f / coneWiresNum))) * radius, endPoint.y, endPoint.z + std::sin(glm::radians((i + 1) * (360.0f / coneWiresNum))) * radius);
	}
	glEnd();

	shader->SetModelMatrix(GetModelMatrix());
}



void Model::RenderLine(const float startX, const float startY, const float startZ, const float endX, const float endY, const float endZ, float lineWidth)
{
	RenderLine(glm::vec3(startX, startY, startZ), glm::vec3(endX, endY, endZ), lineWidth);
}

void Model::SetTriangleParam(const float triangleSideLength)
{
	SetTriangleParam(triangleSideLength, triangleSideLength);
}

void Model::SetTriangleParam(const float width, const float length)
{
	float vertices[] = {
		-1.0f * width, -1.0f * length, 0.0f, 0.0f, 0.0f,
		0.0f * width,   1.0f * length, 0.0f, 0.5f, 1.0f,
		1.0f * width,  -1.0f * length, 0.0f, 1.0f, 0.0f
	};
	SetVBAttributes(3, 0, 2);
	SetTargetBufferObject(GL_TRIANGLES);
	SetVertexBuffer(vertices, sizeof(vertices));
}

void Model::SetPlaneParam(const float planeSideLength)
{
	SetPlaneParam(planeSideLength, planeSideLength);
}

void Model::SetPlaneParam(const float width, const float length)
{
	float vertices[] = {
		-1.0f * width, -1.0f * length, 0.0f,  0.0f, 0.0f,
		1.0f * width,   -1.0f * length, 0.0f, 1.0f, 0.0f,
		1.0f * width,  1.0f * length, 0.0f,   1.0f, 1.0f,
		-1.0f * width, 1.0f * length, 0.0f,   0.0f, 1.0f,
	};
	SetVBAttributes(3, 0, 2);
	SetTargetBufferObject(GL_QUADS);
	SetVertexBuffer(vertices, sizeof(vertices));
}

void Model::SetGroundPlaneParam(const float planeSideLength)
{
	SetGroundPlaneParam(planeSideLength, planeSideLength);
}
void Model::SetGroundPlaneParam(const float width, const float length)
{
	float vertices[] = {
	-1.0f * width, 0.0f, -1.0f * length,   0.0f, 1.0f,
	-1.0f * width, 0.0f, 1.0f * length ,   0.0f, 0.0f,
	1.0f * width,  0.0f, 1.0f * length ,   1.0f, 0.0f,
	1.0f * width,  0.0f, -1.0f * length,   1.0f, 1.0f,
	};
	SetVBAttributes(3, 0, 2);
	SetTargetBufferObject(GL_QUADS);
	SetVertexBuffer(vertices, sizeof(vertices));
}

void Model::SetCubeParam(const float cubeSideLength)
{
	SetCubeParam(cubeSideLength, cubeSideLength, cubeSideLength);
}

void Model::SetCubeParam(const float length, const float width, const float height)
{

	float vertices[] = {
		// positions										  Normals       Texture Coords
		-0.5f * length, -0.5f * width, -0.5f * height,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
		 0.5f * length, -0.5f * width, -0.5f * height,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
		 0.5f * length,  0.5f * width, -0.5f * height,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		 0.5f * length,  0.5f * width, -0.5f * height,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		-0.5f * length,  0.5f * width, -0.5f * height,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
		-0.5f * length, -0.5f * width, -0.5f * height,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

		-0.5f * length, -0.5f * width,  0.5f * height,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		 0.5f * length, -0.5f * width,  0.5f * height,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
		 0.5f * length,  0.5f * width,  0.5f * height,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		 0.5f * length,  0.5f * width,  0.5f * height,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		-0.5f * length,  0.5f * width,  0.5f * height,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
		-0.5f * length, -0.5f * width,  0.5f * height,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

		-0.5f * length,  0.5f * width,  0.5f * height, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		-0.5f * length,  0.5f * width, -0.5f * height, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		-0.5f * length, -0.5f * width, -0.5f * height, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f * length, -0.5f * width, -0.5f * height, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f * length, -0.5f * width,  0.5f * height, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		-0.5f * length,  0.5f * width,  0.5f * height, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		 0.5f * length,  0.5f * width,  0.5f * height,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		 0.5f * length,  0.5f * width, -0.5f * height,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		 0.5f * length, -0.5f * width, -0.5f * height,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f * length, -0.5f * width, -0.5f * height,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f * length, -0.5f * width,  0.5f * height,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		 0.5f * length,  0.5f * width,  0.5f * height,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		-0.5f * length, -0.5f * width, -0.5f * height,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f * length, -0.5f * width, -0.5f * height,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f * length, -0.5f * width,  0.5f * height,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f * length, -0.5f * width,  0.5f * height,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f * length, -0.5f * width,  0.5f * height,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f * length, -0.5f * width, -0.5f * height,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

		-0.5f * length,  0.5f * width, -0.5f * height,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f * length,  0.5f * width, -0.5f * height,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f * length,  0.5f * width,  0.5f * height,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f * length,  0.5f * width,  0.5f * height,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f * length,  0.5f * width,  0.5f * height,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f * length,  0.5f * width, -0.5f * height,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
	};
	SetVBAttributes(3, 3, 2);
	SetTargetBufferObject(GL_TRIANGLES);
	SetVertexBuffer(vertices, sizeof(vertices));
}

void Model::SetPyramidParam(const float pyramidSideLength)
{
	SetPyramidParam(pyramidSideLength, pyramidSideLength, pyramidSideLength);
}

void Model::SetPyramidParam(const float length, const float width, const float height)
{
	float vertices[] = {
		//Position												Normals			Texture Coords
		-0.5f * length, 0.0f * width, 0.5f * height,	0.0f, 0.25f, 0.5f,		0.0f, 0.0f,
		 0.0f * length, 1.0f * width, 0.0f * height,	0.0f, 0.25f, 0.5f,		0.5f, 1.0f,	//Front
		0.5f * length, 0.0f * width, 0.5f * height,     0.0f, 0.25f, 0.5f,		1.0f, 0.0f,

		-0.5f * length, 0.0f * width, 0.5f * height,	-0.5f, 0.25f, 0.0f,		0.0f, 0.0f,
		 0.0f * length, 1.0f * width, 0.0f * height,	-0.5f, 0.25f, 0.0f,		0.5f, 1.0f,	//Left
		-0.5f * length, 0.0f * width, -0.5f * height,	-0.5f, 0.25f, 0.0f,		1.0f, 0.0f,

		 0.5f * length, 0.0f * width, -0.5f * height,	 0.0f, 0.25f, -0.5f,	0.0f, 0.0f,
		 0.0f * length, 1.0f * width,  0.0f * height,	 0.0f, 0.25f, -0.5f,	0.5f, 1.0f,	//Back
		-0.5f * length, 0.0f * width, -0.5f * height,	 0.0f, 0.25f, -0.5f,	1.0f, 0.0f,

		0.5f * length, 0.0f * width, -0.5f * height,	 0.5f, 0.25f, 0.0f,		0.0f, 0.0f,
		0.0f * length, 1.0f * width,  0.0f * height,	 0.5f, 0.25f, 0.0f,		0.5f, 1.0f,	//Right
		0.5f * length, 0.0f * width,  0.5f * height,	 0.5f, 0.25f, 0.0f,		1.0f, 0.0f,

		-0.5f * length, 0.0f * width, -0.5f * height,	 0.0f, -1.0f, 0.0f,		0.0f, 0.0f,
		-0.5f * length, 0.0f * width,  0.5f * height,	 0.0f, -1.0f, 0.0f,		0.0f, 1.0f,	//Bottom Square 1
		 0.5f * length, 0.0f * width,  0.5f * height,	 0.0f, -1.0f, 0.0f,		1.0f, 1.0f,

		-0.5f * length, 0.0f * width,  -0.5f * height,	 0.0f, -1.0f, 0.0f,		0.0f, 0.0f,
		0.5f * length, 0.0f * width, -0.5f * height,	 0.0f, -1.0f, 0.0f,		1.0f, 0.0f,	//Bottom Square 2
		 0.5f * length, 0.0f * width,  0.5f * height,	 0.0f, -1.0f, 0.0f,		1.0f, 1.0f
	};

	SetVBAttributes(3, 3, 2);
	SetTargetBufferObject(GL_TRIANGLES);
	SetVertexBuffer(vertices, sizeof(vertices));
}

glm::mat4& Model::Translate(float x, float y, float z)
{
	model = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
	return model;
}

glm::mat4& Model::Translate(glm::vec3 pos)
{
	model = glm::translate(glm::mat4(1.0f), pos);
	return model;
}

glm::mat4& Model::Rotate(float angle, float xAxis, float yAxis, float zAxis)
{
	model = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(xAxis, yAxis, zAxis));
	return model;
}

glm::mat4& Model::Rotate(float angle, glm::vec3 axis)
{
	model = glm::rotate(glm::mat4(1.0f), glm::radians(angle), axis);
	return model;
}

glm::mat4& Model::Scale(float x, float y, float z)
{
	model = glm::scale(glm::mat4(1.0f), glm::vec3(x, y, z));
	return model;
}

glm::mat4& Model::Scale(glm::vec3 factor)
{
	model = glm::scale(glm::mat4(1.0f), factor);
	return model;
}

glm::mat4& Model::Scale(float uniformScaleFactor)
{
	model = glm::scale(glm::mat4(1.0f), glm::vec3(uniformScaleFactor));
	return model;
}

glm::mat4& Model::TranslateAccum(float x, float y, float z)
{
	model = glm::translate(model, glm::vec3(x, y, z));
	return model;
}

glm::mat4& Model::TranslateAccum(glm::vec3 pos)
{
	model = glm::translate(model, pos);
	return model;
}

glm::mat4& Model::RotateAccum(float angle, float xAxis, float yAxis, float zAxis)
{
	model = glm::rotate(model, angle, glm::vec3(xAxis, yAxis, zAxis));
	return model;
}

glm::mat4& Model::RotateAccum(float angle, glm::vec3 axis)
{
	model = glm::rotate(model, angle, axis);
	return model;
}

glm::mat4& Model::ScaleAccum(float x, float y, float z)
{
	model = glm::scale(model, glm::vec3(x, y, z));
	return model;
}

glm::mat4& Model::ScaleAccum(glm::vec3 factor)
{
	model = glm::scale(model, factor);
	return model;
}

void Model::LookAt(glm::vec3 center)
{
	Decompose();
	model = glm::inverse(glm::lookAt(modelMatrices->translation, center, glm::vec3(0, 1, 0)));
}

void Model::LookAt(Model& centerObj)
{
	centerObj.Decompose();
	LookAt(centerObj.modelMatrices->translation);
}

void Model::Decompose(glm::mat4& model)
{
	modelMatrices = new ModelMatrices();
	glm::decompose(model, modelMatrices->scale, modelMatrices->orientation, modelMatrices->translation, modelMatrices->skew, modelMatrices->perspective);
}

void Model::Decompose()
{
	Decompose(model);
}

void Model::Render(std::string modelUniformName)
{
	if (shader)
	{
		va->Bind();
		shader->Bind();
		shader->SetUniformMat4f(modelUniformName, model);
		if (material)
			material->BindMaterial();
		if (type == ModelType::Model)
		{
			ib->Bind();
			glDrawElements(targetBufferObject, ib->GetCount(), GL_UNSIGNED_INT, nullptr);
			return;
		}
	}


	glDrawArrays(targetBufferObject, 0, indicesNum);
}

void Model::RenderInstanced(int instanceNum, std::vector<glm::vec3>& offsets, std::string modelUniformName /*= DEFAULT_UNIFORM_MODEL_NAME*/)
{

	static bool firstTime = true;
	if (firstTime)
	{
		VertexBuffer vb(&offsets[0], sizeof(float) * offsets.size() * 3);
		VertexBufferLayout vbl;
		vbl.Push<float>(3);
		va->AddBuffer(vb, vbl);
		firstTime = false;
	}


	if (shader)
	{

		va->Bind();
		shader->Bind();
		shader->SetUniformMat4f(modelUniformName, model);
		if (material)
			material->BindMaterial();
		if (type == ModelType::Model)
		{
			ib->Bind();
			glDrawElementsInstanced(targetBufferObject, ib->GetCount(), GL_UNSIGNED_INT, nullptr, instanceNum);
			return;
		}
	}


	GLCall(glDrawArraysInstanced(targetBufferObject, 0, indicesNum, instanceNum));
}

