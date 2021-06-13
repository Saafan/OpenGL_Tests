#include "Renderer.h"
#include "Shader.h"

Shader::Shader(const std::string& filePath)
	: m_FilePath(filePath), m_RendererID(0)
{
	ShaderProgramSource source = ParseShader(filePath);
	m_RendererID = CreateShader(source.VertexSource, source.FragmentSource, source.GeometrySource);
}

enum class Shadertype
{
	NONE = -1, VERTEX = 0, FRAGMENT = 1, GEOMETRY = 2
};

Shader::~Shader()
{
	GLCall(glDeleteProgram(m_RendererID));
}

static int vertexLastLine = 0;

ShaderProgramSource Shader::ParseShader(const std::string filePath)
{
	std::ifstream stream(filePath);

	if (!stream)
	{
		std::cout << "Couldn't Open Shader's file." << std::endl;
		ShaderProgramSource s;
		return s;
	}



	std::string line;
	std::stringstream ss[3]; // 2 for vertex and fragment shaders and Geometry
	Shadertype type = Shadertype::NONE;

	while (getline(stream, line))
	{
		// Set the type if #shader is found
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
				type = Shadertype::VERTEX;
			else if (line.find("fragment") != std::string::npos)
				type = Shadertype::FRAGMENT;
			else if (line.find("geometry") != std::string::npos)
				type = Shadertype::GEOMETRY;
		}
		else // Add the line to the source
		{
			if (type == Shadertype::VERTEX)
				vertexLastLine++;
			ss[(int)type] << line << '\n';
		}
	}

	return { ss[0].str(), ss[1].str(), ss[2].str() };
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
	unsigned int id = glCreateShader(type); // Create the shader
	const char* src = source.c_str(); // Return the pointer of the first character of the source
	GLCall(glShaderSource(id, 1, &src, nullptr)); // Specify the shader source code
	GLCall(glCompileShader(id));

	// Error handling
	int result;
	GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result)); // Returns the compile status parameter
	if (result == GL_FALSE)
	{
		int length;
		GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
		char* message = (char*)alloca(length * sizeof(char)); // Allocate this on the stack dynamically because 'char message[length]' is not allowed
		GLCall(glGetShaderInfoLog(id, length, &length, message));
		std::cout << "Failed to Compile " << (type == GL_VERTEX_SHADER ? "Vertex" : type == GL_FRAGMENT_SHADER ? "Fragment" : "Geometry") << " Shader:" << std::endl;

		if (type == GL_FRAGMENT_SHADER)
		{
			std::string LineNumber;
			for (size_t i = 2; i < length; i++)
			{
				if (message[i] == ')')
					break;
				LineNumber += message[i];
			}

			vertexLastLine += std::stoi(LineNumber);
			std::cout << "Line (" << vertexLastLine << "): " << message << std::endl;
		}
		else
		{
			std::cout << message << std::endl;
		}

		GLCall(glDeleteShader(id));
		return 0;
	}

	return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader, const std::string& geometryShader)
{
	unsigned int gs = 0;
	unsigned int program = glCreateProgram(); // Create a shader program to attach shader to
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
	if (!geometryShader.empty())
		gs = CompileShader(GL_GEOMETRY_SHADER, geometryShader);
	// Attach both shaders to the program
	GLCall(glAttachShader(program, vs));
	GLCall(glAttachShader(program, fs));
	if (gs != 0)
	{
		GLCall(glAttachShader(program, gs));
	}
	
	GLCall(glLinkProgram(program)); // Link the program so the shaders are used
	GLCall(glValidateProgram(program)); // Check if the program can be executed

	// The shaders are linked to the program, so the shaders can be deleted
	GLCall(glDeleteShader(vs));
	GLCall(glDeleteShader(fs));
	GLCall(glDeleteShader(gs));

	return program;
}

void Shader::Bind() const
{
	GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
	GLCall(glUseProgram(0));
}

unsigned int Shader::GetProgramID()
{
	return m_RendererID;
}

void Shader::SetUniform1i(const std::string& name, int value)
{
	GLCall(glUniform1i(GetUniformLocation(name), value));
}

void Shader::SetUniform1f(const std::string& name, float value)
{
	GLCall(glUniform1f(GetUniformLocation(name), value));
}

void Shader::SetUniform3f(const std::string& name, float v0, float v1, float v2)
{
	GLCall(glUniform3f(GetUniformLocation(name), v0, v1, v2));
}

void Shader::SetUniform3f(const std::string& name, glm::vec3& vector)
{
	GLCall(glUniform3f(GetUniformLocation(name), vector.x, vector.y, vector.z));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
	GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void Shader::SetUniform4f(const std::string& name, glm::vec4& vector)
{
	GLCall(glUniform4f(GetUniformLocation(name), vector.x, vector.y, vector.z, vector.w));
}

void Shader::SetUniformMat4f(const std::string& name, glm::mat4& matrix)
{
	if (name == DEFAULT_UNIFORM_MODEL_NAME)
		this->view = &matrix;
	if (name == DEFAULT_UNIFORM_PROJ_NAME)
		this->proj = &matrix;

	GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(matrix)));
}

void Shader::SetMVPMatrix(glm::mat4& model, glm::mat4& view, glm::mat4& proj)
{
	SetModelMatrix(model);
	SetViewMatrix(view);
	SetProjMatrix(proj);
	this->view = &view;
	this->proj = &proj;
}

void Shader::SetModelMatrix(glm::mat4& model)
{
	SetUniformMat4f(DEFAULT_UNIFORM_MODEL_NAME, model);
}

void Shader::SetViewMatrix(glm::mat4& view)
{
	SetUniformMat4f(DEFAULT_UNIFORM_VIEW_NAME, view);
	this->view = &view;
}

void Shader::SetProjMatrix(glm::mat4& proj)
{
	SetUniformMat4f(DEFAULT_UNIFORM_PROJ_NAME, proj);
	this->proj = &proj;
}

unsigned int Shader::GetUniformLocation(const std::string& name)
{
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
		return m_UniformLocationCache[name];

	GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
	if (location == -1)
		std::cout << "Warning: uniform " << name << " doesn't exist!" << std::endl;

	m_UniformLocationCache[name] = location;
	return location;
}
