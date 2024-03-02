#pragma once

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
}
