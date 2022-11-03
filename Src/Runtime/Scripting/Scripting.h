#pragma once

#include "../Core/Subsystem.h"
#include "../Core/Object/Object.h"

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
typedef struct _MonoAssembly  MonoAssembly;
typedef struct _MonoImage	  MonoImage;
typedef struct MonoMethodDesc MonoMethodDesc;
typedef struct _MonoMethod	  MonoMethod;

typedef struct _MonoType MonoType;

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
class ScriptClassInstance;

struct ScriptMethod
{
	bool Read();

	bool Invoke(void* _instance, void** _params, MonoObject** _ret);

	std::string			 name = "";
	bool				 isRead = false;
	ScriptClassTypeInfo* pClassType = nullptr;
	MonoMethod*			 pMethod = nullptr;
	MonoMethodDesc*		 pMethodDesc = nullptr;
};

enum class ScriptValueType : uint16_t
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
	ScriptValueType	type = ScriptValueType::Other;
	MonoType*		pMonoType = nullptr;


	bool IsStatic();
	void SetValue(MonoObject* _pInstance, void* _pValue);
	void GetValue(MonoObject* _pInstance, void* _pOutValue);
};

struct ScriptPropertyInfo
{
	std::string name = "";

	bool isSerializeField = false;

	// the class about property
	MonoProperty*  pProp;
	// get method of this property
	MonoMethod*	   pGetMethod = nullptr;
	// set method of this property
	MonoMethod*	   pSetMethod = nullptr;
	// TODO judge the type
	ScriptValueType type = ScriptValueType::Other;
	MonoType*	   pMonoType = nullptr;

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
	//std::string guid = "";
	bool		isRead = false;
	bool		isComponent = false;
	MonoClass*	pMonoClass = nullptr;

	std::vector<std::shared_ptr<ScriptFieldInfo>> spFieldInfos;
	std::vector<std::shared_ptr<ScriptPropertyInfo>> spPropertyInfos;

	std::shared_ptr<ScriptMethod> initMethod;
	std::shared_ptr<ScriptMethod> updateMethod;
	std::shared_ptr<ScriptMethod> imguiUpdateMethod;
};

class ScriptClassInstance
{
public:
	ScriptClassInstance(MonoObject* _pInst, ScriptClassTypeInfo* _pClassType);
	MonoObject*			 m_pInstance = nullptr;
	ScriptClassTypeInfo* m_pClassType = nullptr;

	bool InvokeInitMethod();
	bool InvokeUpdateMethod();
	bool InvokeImGuiUpdateMethod();
};
class Component : public Object
{
public:
	static void SetUpInternalCall();

	std::shared_ptr<GameObject> m_spOwner;

	friend GameObject;
	friend Scripting;

	static MonoObject* Internal_GetGameObject(Component* _chachedPtr);
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

	// MonoType to ScriptVaueType
	static ScriptValueType ConvertTypeMonoToRuntime(MonoType* _pMonoType);

	static unsigned int GetSizeFromValueType(ScriptValueType _type);

	std::string GenerateGUID();

	MonoDomain* GetMonoDomain();

	std::list<std::shared_ptr<GameObject>>& GetGameObjects();
	std::shared_ptr<GameObject>				CreateGameObject();


	//GameObject			go;


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