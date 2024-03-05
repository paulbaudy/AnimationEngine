#pragma once

#include "glm/vec3.hpp"
#include <vector>

namespace AN
{
	class FFrameBuffer
	{
	public:
		FFrameBuffer() {}
		FFrameBuffer(float width, float height);
		~FFrameBuffer();
		unsigned int getFrameTexture();
		void RescaleFrameBuffer(float width, float height);
		void Bind() const;
		void Unbind() const;
	private:
		unsigned int fbo;
		unsigned int texture;
		unsigned int rbo;
	};

	class FVertexBuffer
	{
	public:

		void GenBuffers();
		void Draw();
		void BindBuffers();
		void UnbindBuffers();

		std::vector<float> Vertices;
		std::vector<int> Indices;
		std::vector<glm::vec3> Normals;
		unsigned int VBO, VAO, EBO, NormalBuffer;
	};
}
