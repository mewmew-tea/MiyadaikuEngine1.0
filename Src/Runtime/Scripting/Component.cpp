#include "Component.h"
#include <mono/jit/jit.h>
#include "ScriptClassInstance.h"

namespace Miyadaiku
{

void Component::SetUpInternalCall()
{
	mono_add_internal_call("MiyadaikuEngine.Component::Internal_GetGameObject",
						   &Internal_GetGameObject);
}
MonoObject* Component::Internal_GetGameObject(Component* _chachedPtr)
{
	return _chachedPtr->m_spOwner->GetClassInstance()->m_pInstance;
}
}