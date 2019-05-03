#pragma once

namespace rt
{

class RenderOutput
{
public:
	virtual void SetPixel(int x, int y, int red, int green, int blue) = 0;

}; // RenderOutput

}
