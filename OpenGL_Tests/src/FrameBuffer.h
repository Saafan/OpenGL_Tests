#pragma once
#include "RenderBuffer.h"
#include "Texture.h"


class FrameBuffer
{
public:
	FrameBuffer(int width, int height);

	void Bind();
	void UnBind();
	void Delete();

	void BindTexture();

	unsigned int fbo = -1;
	unsigned int texture = -1;

	RenderBuffer* rb = nullptr;
	Texture* tex = nullptr;
};