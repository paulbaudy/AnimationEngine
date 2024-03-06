#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>

namespace AN
{
	class FShader
	{
	public:
		FShader(const std::string& InPath, unsigned int ShaderType);

		unsigned int ID = -1;
		unsigned int Type = -1;
	};

	class FMaterial
	{
	public:
		FMaterial();
		// todo destructor unload shader?

		bool PushShader(const std::string& InPath, unsigned int ShaderType);
		void SetBool(const std::string& name, bool value) const;
		void SetInt(const std::string& name, int value) const;
		void SetFloat(const std::string& name, float value) const;
		void SetMat4(const std::string& name, const float* value) const;
		void SetVec3(const std::string& name, glm::vec3& value) const;

		static std::string FMaterial::GetShaderTypeStr(int InShaderType);
		static unsigned int CompileShaderString(const char* InShaderStr, int ShaderType);
		static unsigned int CompileShaderFile(std::string InFilePath, int InShaderType);

		//unsigned int GetID() const { return ID; }
		void Bind();
		void Unbind();

	private:
		std::vector<FShader> Shaders;
		unsigned int shaderProgram = -1;
	};
}