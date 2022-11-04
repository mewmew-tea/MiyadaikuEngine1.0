#pragma once

#include <string>
#include "ScriptCommon.h"

namespace Miyadaiku
{
/**
 * Field info of script.
 */
struct ScriptFieldInfo
{
	std::string name = "";

	bool isSerializeField = false;

	MonoClassField* pField = nullptr;
	// TODO judge the type
	ScriptValueType type = ScriptValueType::Other;
	MonoType*		pMonoType = nullptr;

	bool IsStatic();
	void SetValue(MonoObject* _pInstance, void* _pValue);
	void GetValue(MonoObject* _pInstance, void* _pOutValue);
};

}