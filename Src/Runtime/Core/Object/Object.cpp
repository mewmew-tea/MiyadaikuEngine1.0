#include "Object.h"
#include "../Engine.h"
#include "../../Scripting/Scripting.h"
#include "../../Scripting/Component.h"
#include "../../Scripting/ScriptFieldInfo.h"

#include <mono/jit/jit.h>

namespace Miyadaiku
{


std::shared_ptr<Object> ObjectManager::GetInstance(InstanceID _id)
{
	return m_instances[_id];
}

std::shared_ptr<Object> ObjectManager::GetInstance(std::string _guid)
{
	if (!m_guidToInstanceIDMap.contains(_guid))
	{
		return nullptr;
	}

	return GetInstance(m_guidToInstanceIDMap[_guid]);
}

InstanceID ObjectManager::GetInstanceID(std::string _guid)
{
	if (!m_guidToInstanceIDMap.contains(_guid))
	{
		return -1;
	}

	return m_guidToInstanceIDMap[_guid];
}


void GameObject::SetUpInternalCall()
{
	mono_add_internal_call("MiyadaikuEngine.GameObject::Internal_GetTransform",
						   &Internal_GetTransform);
	mono_add_internal_call("MiyadaikuEngine.GameObject::Internal_GetComponents",
						   &Internal_GetComponents);
	mono_add_internal_call("MiyadaikuEngine.GameObject::Internal_GetAll",
						   &Internal_GetAll);
	mono_add_internal_call("MiyadaikuEngine.GameObject::Internal_GetEnabled",
						   &Internal_GetEnabled);
	mono_add_internal_call("MiyadaikuEngine.GameObject::Internal_SetEnabled",
						   &Internal_SetEnabled);
}

std::shared_ptr<Component>
GameObject::AddComponent(std::shared_ptr<ScriptClassTypeInfo>& _spCompType)
{
	Engine* pEngine = GetEngine();
	auto	spScripting = pEngine->GetSubsystemLocator().Get<Scripting>();
	auto	pMonoDomain = spScripting->GetMonoDomain();
	if (!pMonoDomain)
	{
		return nullptr;
	}

	auto spClassInst = _spCompType->CreateInstance(pMonoDomain);
	if (spClassInst)
	{
		// setup
		auto spComponent = std::make_shared<Component>();
		spComponent->m_spClassInstance = spClassInst;
		spComponent->m_spOwner = shared_from_this();

		// set chachedPtr
		for (auto field : spClassInst->m_pClassType->spFieldInfos)
		{
			if (field->name == "chachedPtr")
			{
				void* value = spComponent.get();
				field->SetValue(spClassInst->m_pInstance, &value);
			}
		}

		// assign
		m_spComponents.push_back(spComponent);
		if (spClassInst->m_pClassType->initMethod)
		{
			m_spInitMethodComponents.push_back(spComponent);
		}
		if (spClassInst->m_pClassType->updateMethod)
		{
			m_spUpdateMethodComponents.push_back(spComponent);
		}
		if (spClassInst->m_pClassType->imguiUpdateMethod)
		{
			m_spImGuiUpdateMethodComponents.push_back(spComponent);
		}
		return spComponent;
	}
	return nullptr;
}
void GameObject::Init()
{
	for (auto spMethod : m_spInitMethodComponents)
	{
		spMethod->m_spClassInstance->InvokeInitMethod();
	}
}
void GameObject::Update()
{
	for (auto spMethod : m_spUpdateMethodComponents)
	{
		spMethod->m_spClassInstance->InvokeUpdateMethod();
	}
}
void GameObject::ImGuiUpdate()
{
	for (auto spMethod : m_spImGuiUpdateMethodComponents)
	{
		spMethod->m_spClassInstance->InvokeImGuiUpdateMethod();
	}
}

std::shared_ptr<ScriptClassInstance> GameObject::GetClassInstance()
{
	return m_spClassInstance;
}

std::shared_ptr<Component> GameObject::GetTransform()
{
	return m_spTransform;
}

std::list<std::shared_ptr<Component>> GameObject::GetComponents()
{
	return m_spComponents;
}

MonoObject* GameObject::Internal_GetTransform(GameObject* _chachedPtr)
{
	return _chachedPtr->m_spTransform->m_spClassInstance->m_pInstance;
}

MonoArray* GameObject::Internal_GetComponents(GameObject* _chachedPtr)
{
	Engine* pEngine = GetEngine();
	auto	spScripting = pEngine->GetSubsystemLocator().Get<Scripting>();
	auto	pMonoDomain = spScripting->GetMonoDomain();
	auto	pMonoImage = spScripting->GetMonoImage();

	auto spComponentType = std::make_shared<ScriptClassTypeInfo>();
	spComponentType->name = "Component";
	spComponentType->nameSpace = "MiyadaikuEngine";
	spComponentType->ReadClass(pMonoImage);

	//std::vector<MonoObject*> monoObjects;
	//monoObjects.reserve(_chachedPtr->m_spComponents.size());
	MonoArray* result = mono_array_new(pMonoDomain, spComponentType->pMonoClass,
									   _chachedPtr->m_spComponents.size());

	int i = 0;
	for (auto spComp : _chachedPtr->m_spComponents)
	{
		mono_array_set(result, MonoObject*, i,
					   spComp->GetClassInstance()->m_pInstance);
		++i;
	}

	return result;
}
MonoArray* GameObject::Internal_GetAll(GameObject* _chachedPtr)
{
	Engine* pEngine = GetEngine();
	auto	spScripting = pEngine->GetSubsystemLocator().Get<Scripting>();
	auto	pMonoDomain = spScripting->GetMonoDomain();
	auto	pMonoImage = spScripting->GetMonoImage();

	auto spGameObjectType = std::make_shared<ScriptClassTypeInfo>();
	spGameObjectType->name = "GameObject";
	spGameObjectType->nameSpace = "MiyadaikuEngine";
	spGameObjectType->ReadClass(pMonoImage);

	MonoArray* result = mono_array_new(pMonoDomain, spGameObjectType->pMonoClass,
					   spScripting->GetGameObjects().size());

	int i = 0;
	for (auto spGameObject : spScripting->GetGameObjects())
	{
		mono_array_set(result, MonoObject*, i,
					   spGameObject->GetClassInstance()->m_pInstance);
		++i;
	}

	return result;
}

bool GameObject::Internal_GetEnabled(GameObject* _chachedPtr)
{
	return _chachedPtr->m_enabled;
}

void GameObject::Internal_SetEnabled(GameObject* _chachedPtr, bool _enabled)
{
	_chachedPtr->m_enabled = _enabled;
}

std::shared_ptr<ScriptClassInstance> Object::GetClassInstance()
{
	return m_spClassInstance;
}

} // namespace Miyadaiku