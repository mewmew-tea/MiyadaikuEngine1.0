#pragma once

#include "../Core/Subsystem.h"

#include <string>
#include <string_view>
void ScriptingAPI_SetPosition(float _x, float _y);
float	  ScriptingAPI_GetPositionX();
float	  ScriptingAPI_GetPositionY();



typedef struct _MonoClass  MonoClass;
typedef struct _MonoImage  MonoImage;
typedef struct MonoMethodDesc MonoMethodDesc;
typedef struct _MonoMethod	  MonoMethod;

typedef struct _MonoObject MonoObject;

//struct MonoClass;
//struct MonoMethodDesc;
//struct MonoMethod;
//
//struct MonoDomain;
//struct MonoAssembly;
//struct MonoObject;

namespace Miyadaiku
{
struct ScriptClassType;
struct ScriptMethod
{
	ScriptMethod(ScriptClassType* _pClassType, std::string_view _name);

	bool Read();

	bool Invoke(void* _instance, void** _params, MonoObject** _ret);

	std::string		 name = "";
	bool			 isRead = false;
	ScriptClassType* pClassType = nullptr;
	MonoMethod*		 pMethod = nullptr;
	MonoMethodDesc*	 pMethodDesc = nullptr;
};

struct ScriptClassType
{
	bool ReadClass(MonoImage* _pImage);

	
	std::string name = "";
	std::string nameSpace = "";
	bool		isRead = false;
	bool		isComponent = false;
	MonoClass*	pMonoClass = nullptr;

	std::shared_ptr<ScriptMethod> initMethod;
	std::shared_ptr<ScriptMethod> updateMethod;
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
	std::list<std::shared_ptr<ScriptClassType>> m_spScriptClasses;
};
} // namespace Miyadaiku