#pragma once
#include "../Math/RectangleI.h"
#include <memory>

namespace Miyadaiku
{
class Window
{
public:
	virtual bool Create() = 0;
	virtual void Destroy() = 0;
	virtual void ProcessSystemEventQueue() = 0;
	virtual bool IsCreated() const = 0;

	RectangleI GetRect() const
	{
		return m_rect;
	}


private:
	RectangleI m_rect{0, 0, 1280, 720};
	bool	   m_isFullscreen = false;
};
} // namespace Miyadaiku
