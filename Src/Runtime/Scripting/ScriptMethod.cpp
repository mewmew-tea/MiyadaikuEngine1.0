#include "ScriptMethod.h"
#include "ScriptClassTypeInfo.h"
#include <mono/jit/jit.h>
#include <mono/metadata/debug-helpers.h>

namespace Miyadaiku
{

//==============================================
// ScriptMethod
//==============================================
// ScriptMethod::ScriptMethod(ScriptClassTypeInfo* _pClassType,
//						   std::string_view		_name)
//{
//	pClassType = _pClassType;
//	name = _name;
//}

bool ScriptMethod::Read()
{
	if (!pClassType || !pClassType->isRead || name == "")
	{
		return false;
	}

	std::string methodName = pClassType->name + ":" + name;
	pMethodDesc = mono_method_desc_new(methodName.c_str(), false);
	if (!pMethodDesc)
	{
		// MessageBoxA(NULL, "", "Faild loading method desc.", MB_OK);
		return false;
	}

	pMethod =
		mono_method_desc_search_in_class(pMethodDesc, pClassType->pMonoClass);
	if (!pMethod)
	{
		// MessageBoxA(NULL, "", "Faild loading method.", MB_OK);
		return false;
	}

	isRead = true;
	return true;
}
bool ScriptMethod::Invoke(void* _instance, void** _params, MonoObject** _ret)
{
	if (!pMethod)
	{
		return false;
	}
	MonoObject* ptrExObject = nullptr;

	if (!_ret)
	{
		mono_runtime_invoke(pMethod, _instance, _params, &ptrExObject);
	}
	else
	{
		*_ret = mono_runtime_invoke(pMethod, _instance, _params, &ptrExObject);
	}
	// Report exception
	if (ptrExObject)
	{
		MonoString* exString = mono_object_to_string(ptrExObject, nullptr);
		const char* exCString = mono_string_to_utf8(exString);
		// MessageBoxA(NULL, exCString, "Mono Invoke issue", MB_OK |
		// MB_ICONERROR);
		return false;
	}

	return true;
}

}