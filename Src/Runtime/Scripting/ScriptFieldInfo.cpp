#include "ScriptFieldInfo.h"
#include "Scripting.h"

#include <mono/jit/jit.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/attrdefs.h>
#include "../../Core/Engine.h"
namespace Miyadaiku
{

//==============================================
// ScriptFieldInfo
//==============================================
bool ScriptFieldInfo::IsStatic()
{
	return (mono_field_get_flags(pField) & MONO_FIELD_ATTR_STATIC) != 0;
}

void ScriptFieldInfo::SetValue(MonoObject* _pInstance, void* _pValue)
{
	if (!_pInstance || !_pValue)
	{
		return;
	}
	auto spScripting = GetEngine()->GetSubsystemLocator().Get<Scripting>();
	auto pMonoDomain = spScripting->GetMonoDomain();

	MonoType* pMonoType = mono_field_get_type(pField);
	if (mono_type_is_reference(pMonoType))
	{
		MonoString* str = mono_string_new(pMonoDomain, (char*)_pValue);
		mono_field_set_value(_pInstance, pField, str);
	}
	else
	{
		mono_field_set_value(_pInstance, pField, _pValue);
	}
}

void ScriptFieldInfo::GetValue(MonoObject* _pInstance, void* _pOutValue)
{
	if (!_pInstance || !_pOutValue)
	{
		return;
	}

	MonoObject* ret;
	pMonoType = mono_field_get_type(pField);
	mono_field_get_value(_pInstance, pField, _pOutValue);

	//
	// if (mono_type_is_reference(pMonoType))
	//{
	//	if (type == ScriptVaueType::String)
	//	{
	//		// stirng:
	//		_pOutValue = mono_string_to_utf8((MonoString*)ret);
	//	}
	//}
	// else
	if (mono_type_is_reference(pMonoType))
	{
		assert(0
			   && "Assertion: Reference types aren't unsupported. Do for each "
				  "member of the class.");
	}
	// value type
	else
	{
		//_pOutValue = ret;
	}
}

}