#include "RenderBuffer.h"
#include "Renderer.h"

RenderBuffer::RenderBuffer(int width, int height)
{
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

}

void RenderBuffer::Delete()
{
	glDeleteRenderbuffers(1, &rbo);
}

const int RenderBuffer::GetRenderBufferID()
{
	return rbo;
}
