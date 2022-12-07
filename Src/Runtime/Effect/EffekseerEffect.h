#pragma once

#define _WINSOCKAPI_
#include <Effekseer.h>
#include <EffekseerRendererDX11.h>

namespace Miyadaiku
{
class EffekseerEffect
{
public:


private:
	Effekseer::EffectRef effect;
	Effekseer::Handle efkHandle = 0;
};
}