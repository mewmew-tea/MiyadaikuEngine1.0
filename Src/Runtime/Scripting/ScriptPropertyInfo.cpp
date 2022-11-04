#include "ScriptPropertyInfo.h"
#include "Scripting.h"
#include <mono/jit/jit.h>

namespace Miyadaiku
{

bool ScriptPropertyInfo::IsStatic()
{
	return false;
}
void ScriptPropertyInfo::SetValue(MonoObject* _pInstance, void* _pValue)
{

	if (!pSetMethod)
	{
		return;
	}
	MonoObject* ptrExObject = nullptr;

	void* args[1];
	args[0] = _pValue;
	mono_runtime_invoke(pSetMethod, _pInstance, args, &ptrExObject);
	// Report exception
	if (ptrExObject)
	{
		MonoString* exString = mono_object_to_string(ptrExObject, nullptr);
		const char* exCString = mono_string_to_utf8(exString);
		// MessageBoxA(NULL, exCString, "Mono Invoke issue", MB_OK |
		// MB_ICONERROR);
		return;
	}
}
void ScriptPropertyInfo::GetValue(MonoObject* _pInstance, void* _pOutValue)
{
	if (!pGetMethod || !_pOutValue)
	{
		return;
	}
	MonoObject* ptrExObject = nullptr;

	MonoObject* ret =
		mono_runtime_invoke(pGetMethod, _pInstance, nullptr, &ptrExObject);

	// Report exception
	if (ptrExObject)
	{
		MonoString* exString = mono_object_to_string(ptrExObject, nullptr);
		const char* exCString = mono_string_to_utf8(exString);
		// MessageBoxA(NULL, exCString, "Mono Invoke issue", MB_OK |
		// MB_ICONERROR);
		return;
	}

	//
	if (mono_type_is_reference(pMonoType))
	{
		if (type == ScriptValueType::String)
		{
			// stirng:
			_pOutValue = mono_string_to_utf8((MonoString*)ret);
		}
	}
	// if it's a valuetype, mono_runtime_invoke() always boxes the return value
	else
	{
		MonoObject* unboxed = nullptr;
		unboxed = (MonoObject*)mono_object_unbox(ret);
		int size = 0;
		size = Scripting::GetSizeFromValueType(type);
		// size = mono_object_get_size(ret);
		// printf("size: %d\n", size);
		std::memcpy(_pOutValue, unboxed, size);
	}
}
}