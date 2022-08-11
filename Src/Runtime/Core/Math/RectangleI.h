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

	RectangleI();
	~RectangleI()
	{
	}
};

RectangleI::RectangleI() 
	: 
	left(0),
	top(0),
	width(0),
	height(0)
{
}
} // namespace Miyadaiku
