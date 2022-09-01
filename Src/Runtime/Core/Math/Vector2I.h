#pragma once

namespace Miyadaiku
{
// Int型の2次元Vector
struct Vector2I
{
public:
	union
	{
		struct
		{
			union
			{
				int32_t x;
				int32_t width;
			};
			union
			{
				int32_t y;
				int32_t height;
			};
		};
		int32_t data[2] = {0};
	};

	Vector2I();
	~Vector2I();
};

Vector2I::Vector2I() 
	: x(0.0f)
	, y(0.0f)
{
}

Vector2I::~Vector2I()
{
}

} // namespace Miyadaiku
