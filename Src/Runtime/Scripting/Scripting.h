#pragma once

#include "../Core/Subsystem.h"

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

typedef struct _MonoClass	  MonoClass;
typedef struct _MonoImage	  MonoImage;
typedef struct MonoMethodDesc MonoMethodDesc;
typedef struct _MonoMethod	  MonoMethod;

typedef struct _MonoObject MonoObject;

typedef struct _MonoDomain	MonoDomain;
typedef struct _MonoJitInfo MonoJitInfo;

typedef struct MonoVTable MonoVTable;

typedef struct _MonoClassField MonoClassField;
typedef struct _MonoProperty   MonoProperty;
typedef struct _MonoEvent	   MonoEvent;
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
struct ScriptClassInstance;

struct ScriptMethod
{
	ScriptMethod(ScriptClassTypeInfo* _pClassType, std::string_view _name);

	bool Read();

	bool Invoke(void* _instance, void** _params, MonoObject** _ret);

	std::string			 name = "";
	bool				 isRead = false;
	ScriptClassTypeInfo* pClassType = nullptr;
	MonoMethod*			 pMethod = nullptr;
	MonoMethodDesc*		 pMethodDesc = nullptr;
};

enum class ScriptVaueType : uint16_t
{
	// built-in types
	Int = 0,
	Float = 10,
	Bool = 20,
	String = 30,
	// MiyadaikuEngine types
	Vector2 = 100,
	Vector3,
	Vector4,
	Quaternion,
	// unsupported types
	Other = UINT16_MAX,
};

struct ScriptFieldInfo
{
	std::string name = "";

	bool isSerializeField = false;

	MonoClassField* pField = nullptr;
	// TODO judge the type
	ScriptVaueType	type = ScriptVaueType::Other;

	bool IsStatic();
	void SetValue(MonoObject* _pInstance, void* _pValue);
	void GetValue(MonoObject* _pInstance, void* _pOutValue);
};

struct ScriptClassTypeInfo
{
	bool ReadClass(MonoImage* _pImage);

	std::shared_ptr<ScriptClassInstance> CreateInstance(MonoDomain* _pDomain);

	std::string name = "";
	std::string nameSpace = "";
	bool		isRead = false;
	bool		isComponent = false;
	MonoClass*	pMonoClass = nullptr;

	std::vector<std::shared_ptr<ScriptFieldInfo>> spFieldInfos;

	std::shared_ptr<ScriptMethod> initMethod;
	std::shared_ptr<ScriptMethod> updateMethod;
	std::shared_ptr<ScriptMethod> imguiUpdateMethod;
};

struct ScriptClassInstance
{
	ScriptClassInstance(MonoObject* _pInst, ScriptClassTypeInfo* _pClassType);
	MonoObject*			 pInstance = nullptr;
	ScriptClassTypeInfo* pClassType = nullptr;

	bool InvokeInitMethod();
	bool InvokeUpdateMethod();
	bool InvokeImGuiUpdateMethod();
};

class GameObject
{
public:
	void AddComponent(std::shared_ptr<ScriptClassTypeInfo>& _spComp);
	void Init();
	void Update();
	void ImGuiUpdate();

private:
	/**
	 * @brief components attached this gameobject.
	 */
	std::list<std::shared_ptr<ScriptClassInstance>> m_spComponents;

	/**
	 * @brief Components attached to this gameobjects with an Init method.
	 */
	std::list<std::shared_ptr<ScriptClassInstance>> m_spInitMethodComponents;

	/**
	 * @brief Components attached to this gameobjects with an Update method.
	 */
	std::list<std::shared_ptr<ScriptClassInstance>> m_spUpdateMethodComponents;

	/**
	 * @brief Components attached to this gameobjects with an ImGuiUpdate method.
	 */
	std::list<std::shared_ptr<ScriptClassInstance>>
		m_spImGuiUpdateMethodComponents;
};

class Scripting final : public Subsystem
{
public:
	// Subsystem起動時
	void OnAwake() override;
	// Subsystem除外時
	void OnShutdown() override;

	void Update();
	void ImGuiUpdate();
	void Release();

	void LoadUserAssembly(std::string_view _path);

private:
	std::list<std::shared_ptr<ScriptClassTypeInfo>> m_spScriptClasses;

	// ScriptClassTypeInfo m_serializeFieldAttribute;
};
} // namespace Miyadaiku