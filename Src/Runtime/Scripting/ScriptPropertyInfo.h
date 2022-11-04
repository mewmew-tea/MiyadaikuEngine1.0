#pragma once
#include "ScriptCommon.h"
#include <string>

namespace Miyadaiku
{
/**
 * Type info about property.
 */
struct ScriptPropertyInfo
{
	std::string name = "";

	bool isSerializeField = false;

	MonoProperty*	pProp;

	// get method of this property
	MonoMethod*		pGetMethod = nullptr;
	// set method of this property
	MonoMethod*		pSetMethod = nullptr;
	
	ScriptValueType type = ScriptValueType::Other;
	MonoType*		pMonoType = nullptr;

	bool IsStatic();
	void SetValue(MonoObject* _pInstance, void* _pValue);
	void GetValue(MonoObject* _pInstance, void* _pOutValue);
};
}