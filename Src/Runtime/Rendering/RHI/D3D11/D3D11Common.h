#pragma once

#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include <d3d11.h>
#include <d3dcompiler.h>
#include <dxgi.h>

namespace Miyadaiku
{
template <class T> void D3D11_SafeRelease(T*& _ptr)
{
	if (_ptr)
	{
		_ptr->Release();
		_ptr = nullptr;
	}
}

template <class T> void D3D11_SafeCastRelease(void*& _ptr)
{
	if (_ptr)
	{
		static_cast<T*>(_ptr)->Release();
		_ptr = nullptr;
	}
}
} // namespace Miyadaiku
