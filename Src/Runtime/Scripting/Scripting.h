#pragma once

#include "../Core/Subsystem.h"
#include "../Core/Object/Object.h"

#include "ScriptClassInstance.h"
#include "ScriptMethod.h"
#include "ScriptClassTypeInfo.h"

#include <string>
#include <string_view>
#include <unordered_map>

// typedef struct MonoString;

void  ScriptingAPI_SetPosition(float _x, float _y);
float ScriptingAPI_GetPositionX();
float ScriptingAPI_GetPositionY();
// bool	  ScriptingAPI_ImGui_Begin(MonoString* _label);
// void	  ScriptingAPI_ImGui_End();
// void	  ScriptingAPI_ImGui_Text(MonoString* _fmt, ...);


// struct MonoClass;
// struct MonoMethodDesc;
// struct MonoMethod;
//
// struct MonoDomain;
// struct MonoAssembly;
// struct MonoObject;

namespace Miyadaiku
{
struct ScriptClassTypeInfo;
class ScriptClassInstance;

/**
 * Scripting in C#
 */
class Scripting final : public Subsystem
{
public:
	// Subsystem起動時
	void OnAwake() override;
	// Subsystem除外時
	void OnShutdown() override;

	/**
	 * Call update method of scripts.
	 * 
	 */
	void Update();

	/**
	 * Call imguiUpdate method of scripts.
	 *
	 */
	void ImGuiUpdate();

	/**
	 * Call release method of scripts.
	 *
	 */
	void Release();

	void LoadUserAssembly(std::string_view _path);

	// MonoType to ScriptVaueType
	static ScriptValueType ConvertTypeMonoToRuntime(MonoType* _pMonoType);

	static unsigned int GetSizeFromValueType(ScriptValueType _type);

	std::string GenerateGUID();

	MonoDomain* GetMonoDomain();
	MonoImage*	GetMonoImage();

	std::list<std::shared_ptr<GameObject>>& GetGameObjects();
	std::shared_ptr<GameObject>				CreateGameObject();


	//GameObject			go;

	const std::list<std::shared_ptr<ScriptClassTypeInfo>>&
	GetScriptClassTypes();



private:
	std::list<std::shared_ptr<ScriptClassTypeInfo>> m_spScriptClasses;
	std::shared_ptr<ScriptClassTypeInfo>			m_spGameObjectTypeInfo;
	std::shared_ptr<ScriptClassTypeInfo>			m_spTransformTypeInfo;
	std::list<std::shared_ptr<GameObject>> m_spGameObjects;

	void LoadMonoAssembly();

	bool DestoryDomainWithAssisisatedAssembly(MonoDomain* domain_to_destroy);
	void ReleaseMonoAssembly();

	


	MonoImage* assemblyImage = nullptr;

	MonoDomain*	  domain = nullptr;
	MonoDomain*	  rootDomain = nullptr;
	MonoAssembly* assembly = nullptr;

	
	ScriptClassTypeInfo guidProviderClassType;
	ScriptMethod		guidProviderMethod_NewGuid;
	// ScriptClassTypeInfo m_serializeFieldAttribute;
};
} // namespace Miyadaiku