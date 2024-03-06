#pragma once

namespace AN
{
	class FCubeMapTexture
	{
	public:
		void GenBuffer();
		void LoadTextures(const std::vector<std::string>& InFaces);
		void Bind();
		void Unbind();

		unsigned int TextureId = -1;
	};
}