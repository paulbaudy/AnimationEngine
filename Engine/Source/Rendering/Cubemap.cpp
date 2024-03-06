#include "anpch.h"
#include "Cubemap.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glad/glad.h>

namespace AN
{
	void FCubeMapTexture::GenBuffer()
	{
		glGenTextures(1, &TextureId);
		glBindTexture(GL_TEXTURE_CUBE_MAP, TextureId);
	}

	void FCubeMapTexture::LoadTextures(const std::vector<std::string>& InFaces)
	{
        int width, height, nrChannels;
        for (unsigned int i = 0; i < InFaces.size(); i++)
        {
            unsigned char* data = stbi_load(InFaces[i].c_str(), &width, &height, &nrChannels, 0);
            if (data)
            {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                stbi_image_free(data);
            }
            else
            {
                LOG_INFO("Cubemap {0} failed to load");

                stbi_image_free(data);
            }
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	}

    void FCubeMapTexture::Bind()
    {
        glBindTexture(GL_TEXTURE_CUBE_MAP, TextureId);

    }

    void FCubeMapTexture::Unbind()
    {
        // todo
    }


}

