#include "anpch.h"
#include "Shader.h"

#include <glad/glad.h>
#include <fstream>
#include <sstream>

#define SHADER_INVALID_ID -1

namespace AN
{
	unsigned int FMaterial::CompileShaderString(const char* InShaderStr, int ShaderType)
	{
		unsigned int VertexShaderId = glCreateShader(ShaderType);
		glShaderSource(VertexShaderId, 1, &InShaderStr, NULL);
		glCompileShader(VertexShaderId);

		int bCompileSuccess = 0;
		char LogBuffer[512];

		glGetShaderiv(VertexShaderId, GL_COMPILE_STATUS, &bCompileSuccess);

		if (!bCompileSuccess)
		{
			glGetShaderInfoLog(VertexShaderId, 512, NULL, LogBuffer);
			spdlog::error("ERROR::SHADER::VERTEX::COMPILATION_FAILED");
			spdlog::error(LogBuffer);

			return SHADER_INVALID_ID;
		}

		return VertexShaderId;
	}

	bool FMaterial::PushShader(const std::string& InPath, unsigned int ShaderType)
	{
		if (shaderProgram == -1)
			shaderProgram = glCreateProgram();

		FShader Shader(InPath, ShaderType);

		glAttachShader(shaderProgram, Shader.ID);
		glLinkProgram(shaderProgram);
		Shaders.push_back(Shader);

		return true;
	}

	void FMaterial::SetBool(const std::string& name, bool value) const
	{
		glUniform1i(glGetUniformLocation(shaderProgram, name.c_str()), (int)value);
	}

	void FMaterial::SetInt(const std::string& name, int value) const
	{
		glUniform1i(glGetUniformLocation(shaderProgram, name.c_str()), value);
	}

	void FMaterial::SetFloat(const std::string& name, float value) const
	{
		glUniform1f(glGetUniformLocation(shaderProgram, name.c_str()), value);
	}

	void FMaterial::SetMat4(const std::string& name, float* value) const
	{
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, GL_FALSE, value);
	}

	std::string FMaterial::GetShaderTypeStr(int InShaderType)
	{
		if (InShaderType == GL_VERTEX_SHADER) return "GL_VERTEX_SHADER";
		if (InShaderType == GL_FRAGMENT_SHADER) return "GL_FRAGMENT_SHADER";

		return "UNKNOWN SHADER TYPE";
	}

	std::string read_string_from_file(const std::string& file_path)
	{
		const std::ifstream input_stream(file_path, std::ios_base::binary);

		if (input_stream.fail()) {
			throw std::runtime_error("Failed to open file");
		}

		std::stringstream buffer;
		buffer << input_stream.rdbuf();

		return buffer.str();
	}

	unsigned int FMaterial::CompileShaderFile(std::string InFilePath, int InShaderType)
	{
		// todo check file
		LOG_INFO("Compiling shader file '{}' with type '{}'", InFilePath, GetShaderTypeStr(InShaderType))
		std::string ShaderStr = read_string_from_file(InFilePath);

		if (ShaderStr.empty())
		{
			spdlog::error("ERROR::SHADER::VERTEX::COMPILATION_FAILED");
			spdlog::error("Source file is empty.");

			return SHADER_INVALID_ID;
		}

		return CompileShaderString(ShaderStr.c_str(), InShaderType);
	}

	FShader::FShader(const std::string& InPath, unsigned int ShaderType)
	{
		ID = FMaterial::CompileShaderFile(InPath, ShaderType);
		Type = ShaderType;
	}

	FMaterial::FMaterial()
	{
	}

	void FMaterial::Bind()
	{
		glUseProgram(shaderProgram);
	}
	void FMaterial::Unbind()
	{
		glUseProgram(0);
	}

}