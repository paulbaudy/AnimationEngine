#pragma once

#include "Shader.h"
#include "FrameBuffer.h"

namespace AN
{
	struct FRenderContext
	{
		glm::mat4 Projection;
		glm::mat4 View;
		glm::mat4 Model;
	};

	class FPrimitives
	{
	public:
		static std::vector<float> GetTriangle();
		static std::vector<float> GetQuad();
		static std::vector<float> GetSkybox();
		static std::vector<float> GetPlane();

	private:
		FPrimitives() = default;
	};

	class FMesh
	{
	public:
		FMesh();

		void Draw(const FRenderContext& InContext);
	private:
		void BeginDraw();
		void EndDraw();

	public:
		FVertexBuffer Geometry;
		FMaterial Material;
	};
}