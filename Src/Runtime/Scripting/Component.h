#pragma once
#include "ScriptCommon.h"
#include "../Core/Object/Object.h"

namespace Miyadaiku
{
/**
 * Component that handle scripting component
 */
class Component : public Object
{
public:
	static void SetUpInternalCall();

	/**
	 * Owner gameObject of this component
	 */
	std::shared_ptr<GameObject> m_spOwner;

	friend GameObject;
	friend Scripting;

	//------------------------
	// Internal call methods
	//  for C# scripts
	//------------------------
	/**
	 * Internal call: Get owner gameObject.
	 * 
	 * @param _chachedPtr native Component handling the C# object instance
	 * @return pointer about owner gameObject
	 */
	static MonoObject* Internal_GetGameObject(Component* _chachedPtr);
};
}