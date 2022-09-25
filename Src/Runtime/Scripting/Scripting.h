#pragma once

#include "../Core/Subsystem.h"

#include <string_view>
void ScriptingAPI_SetPosition(float _x, float _y);
float	  ScriptingAPI_GetPositionX();
float	  ScriptingAPI_GetPositionY();
namespace Miyadaiku
{
class Scripting final : public Subsystem
{
public:
	// Subsystem起動時
	void OnAwake() override;
	// Subsystem除外時
	void OnShutdown() override;


	void Update();
	void Release();

	void LoadUserAssembly(std::string_view _path);
	
};
} // namespace Miyadaiku