#pragma once
#include <string>

namespace Miyadaiku
{
class RHI_Device
{
public:
	RHI_Device();

	~RHI_Device();

	virtual void Init() = 0;
	virtual void Release() = 0;

private:

};
} // namespace Miyadaiku