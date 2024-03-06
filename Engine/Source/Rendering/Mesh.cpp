#include "Mesh.h"

#include <string>

namespace AN
{
	FMesh::FMesh()
	{
		Geometry.GenBuffers();
	}
	void FMesh::Draw(const FRenderContext& InContext)
	{
		BeginDraw();
		Material.SetMat4("projection", &InContext.Projection[0][0]);
		Material.SetMat4("view", &InContext.View[0][0]);
		Material.SetMat4("model", &InContext.Model[0][0]);
		Geometry.Draw();
		EndDraw();
	}
	void FMesh::BeginDraw()
	{
		Geometry.BindBuffers();
		Material.Bind();
	}
	void FMesh::EndDraw()
	{
		Geometry.UnbindBuffers();
		Material.Unbind();
	}
	std::vector<float> FPrimitives::GetTriangle()
	{
		return {
		   -1.0f, -1.0f, 0.0f,
		   1.0f, -1.0f, 0.0f,
		   0.0f,  1.0f, 0.0f,
		};
	}
	std::vector<float> FPrimitives::GetQuad()
	{
		return {
			1.f, 1.f, 0.f,
			-1.f, -1.f, 0.f,
			-1.f, 1.f, 0.f,
			-1.f, -1.f, 0.f,
			1.f, 1.f, 0.f,
			1.f, -1.f, 0.f,
		};
	}

	std::vector<float> FPrimitives::GetPlane()
	{
		return {
			1.f, 0.f, -1.f,
			0.f, 0.f, -1.f,
			0.f, 0.f, 1.f,
			0.f, 0.f, -1.f,
			0.f, 0.f, 1.f,
			-1.f, 0.f, 1.f,
		};
	}

	std::vector<float> FPrimitives::GetSkybox()
	{
		return {    
			-1.0f,  1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			-1.0f,  1.0f, -1.0f,
			 1.0f,  1.0f, -1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			 1.0f, -1.0f,  1.0f
		};
	}

}