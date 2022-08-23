#pragma once

namespace Miyadaiku
{
class RectangleI
{
public:
	int left;
	int top;
	int width;
	int height;

	RectangleI() : left(0), top(0), width(0), height(0)
	{
	}
	RectangleI(int _left, int _top, int _width, int _height)
	{
		left = _left;
		top = _top;
		width = _width;
		height = _height;
	}
	~RectangleI()
	{
	}
};
} // namespace Miyadaiku
