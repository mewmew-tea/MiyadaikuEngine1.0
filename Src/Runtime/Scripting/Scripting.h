#pragma once

#include "../Core/Subsystem.h"

#include <string>
#include <string_view>
#include <unordered_map>
void ScriptingAPI_SetPosition(float _x, float _y);
float	  ScriptingAPI_GetPositionX();
float	  ScriptingAPI_GetPositionY();



typedef struct _MonoClass  MonoClass;
typedef struct _MonoImage  MonoImage;
typedef struct MonoMethodDesc MonoMethodDesc;
typedef struct _MonoMethod	  MonoMethod;

typedef struct _MonoObject MonoObject;

typedef struct _MonoDomain	MonoDomain;
typedef struct _MonoJitInfo MonoJitInfo;


typedef struct MonoVTable MonoVTable;

typedef struct _MonoClassField MonoClassField;
typedef struct _MonoProperty   MonoProperty;
typedef struct _MonoEvent	   MonoEvent;
//struct MonoClass;
//struct MonoMethodDesc;
//struct MonoMethod;
//
//struct MonoDomain;
//struct MonoAssembly;
//struct MonoObject;

namespace Miyadaiku
{
struct ScriptClassTypeInfo;
struct ScriptClassInstance;

struct ScriptMethod
{
	ScriptMethod(ScriptClassTypeInfo* _pClassType, std::string_view _name);

	bool Read();

	bool Invoke(void* _instance, void** _params, MonoObject** _ret);

	std::string		 name = "";
	bool			 isRead = false;
	ScriptClassTypeInfo* pClassType = nullptr;
	MonoMethod*		 pMethod = nullptr;
	MonoMethodDesc*	 pMethodDesc = nullptr;
};

enum class ScriptVaueType : uint16_t
{
	// primiteve and string
	Int = 0,
	Float,
	Bool,
	String,
	// support types
	Vector2 = 100,
	Vector3,
	Vector4,
	Quaternion,
	// unsupport types
	Other = UINT16_MAX,
};

struct ScriptValue
{
	void*		   value;
	ScriptVaueType type = ScriptVaueType::Other;
};

struct ScriptFieldInfo
{
	std::string name = "";

	bool isSerializeField = false;

	MonoClassField* pField = nullptr;

	//void		SetValue(ScriptValue& _val);
	//ScriptValue GetValue();
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
};

struct ScriptClassInstance
{
	ScriptClassInstance(MonoObject* _pInst, ScriptClassTypeInfo* _pClassType);
	MonoObject*			 pInstance = nullptr;
	ScriptClassTypeInfo* pClassType = nullptr;

	bool InvokeInitMethod();
	bool InvokeUpdateMethod();
};

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

private:
	std::list<std::shared_ptr<ScriptClassTypeInfo>> m_spScriptClasses;

	//ScriptClassTypeInfo m_serializeFieldAttribute;
};
} // namespace Miyadaiku