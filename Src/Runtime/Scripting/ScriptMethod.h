#pragma once
#include <string>
#include "ScriptCommon.h"

namespace Miyadaiku
{
struct ScriptClassTypeInfo;

/**
 * Method info of script.
 */
struct ScriptMethod
{
	/**
	 * read from assembly.
	 * 
	 * @return Is successful reading method
	 */
	bool Read();

	/**
	 * Invoke this method.
	 *
	 * @return Is successful invoke method
	 */
	bool Invoke(void* _instance, void** _params, MonoObject** _ret);

	std::string			 name = "";
	bool				 isRead = false;
	ScriptClassTypeInfo* pClassType = nullptr;
	MonoMethod*			 pMethod = nullptr;
	MonoMethodDesc*		 pMethodDesc = nullptr;
};
}