#pragma once

class RenderBuffer
{
public:
	RenderBuffer(int width, int height);
	void Delete();
	const int GetRenderBufferID();
private:
	unsigned int rbo = -1;
	

};