#pragma once
#include <memory>
#include <string>
#include <list>
#include <queue>
#include <unordered_map>

#include "../Subsystem.h"

#define MIYADAIKU_OBJECT_INSTANCE_MAX 8192

typedef struct _MonoObject MonoObject;
typedef struct _MonoArray MonoArray;
typedef struct _MonoType MonoType;
typedef struct _MonoReflectionType MonoReflectionType;

namespace Miyadaiku
{
class Object;
class Scripting;

struct ScriptClassTypeInfo;
class ScriptClassInstance;
class Component;

// InstanceID of a object
using InstanceID = int32_t;

class ObjectManager : public Subsystem
{
public:
	std::shared_ptr<Object> GetInstance(InstanceID _id);
	std::shared_ptr<Object> GetInstance(std::string _guid);

	InstanceID GetInstanceID(std::string _guid);

private:
	std::queue<InstanceID>	m_availableIDs;
	std::shared_ptr<Object> m_instances[MIYADAIKU_OBJECT_INSTANCE_MAX];
	std::unordered_map<std::string, InstanceID> m_guidToInstanceIDMap;
};

class Object
{
public:
	std::shared_ptr<ScriptClassInstance> GetClassInstance();

protected:
	std::string							 m_guid;
	InstanceID							 m_instanceID;
	std::shared_ptr<ScriptClassInstance> m_spClassInstance;
};

class GameObject : public Object, public std::enable_shared_from_this<GameObject>
{
public:
	static void SetUpInternalCall();

	std::shared_ptr<Component>
		 AddComponent(std::shared_ptr<ScriptClassTypeInfo>& _spCompType);
	//void RemoveComponent(std::shared_ptr<ScriptClassTypeInfo>& _spCompType);
	void Init();
	void Update();
	void ImGuiUpdate();

	std::shared_ptr<ScriptClassInstance> GetClassInstance();

	std::shared_ptr<Component> GetTransform();
	std::list <std::shared_ptr<Component>> GetComponents();

	bool GetEnabled()
	{
		return m_enabled;
	}

	friend Scripting;

private:

	bool m_enabled = true;

	//------------------------
	// Components
	//------------------------
	/**
	 * @brief components attached to this gameobject.
	 */
	std::list<std::shared_ptr<Component>> m_spComponents;

	/**
	 * @brief transform attached to this gameobject.
	 */
	std::shared_ptr<Component>			 m_spTransform;
	/**
	 * @brief transform attached to this gameobject.
	 */
	std::shared_ptr<Component>			 m_spRenderable;

	/**
	 * @brief Components attached to this gameobjects with an Init method.
	 */
	std::list<std::shared_ptr<Component>> m_spInitMethodComponents;

	/**
	 * @brief Components attached to this gameobjects with an Update method.
	 */
	std::list<std::shared_ptr<Component>> m_spUpdateMethodComponents;

	/**
	 * @brief Components attached to this gameobjects with an ImGuiUpdate
	 * method.
	 */
	std::list<std::shared_ptr<Component>>
		m_spImGuiUpdateMethodComponents;

	//------------------------
	// Internal call methods
	//  for C# scripts
	//------------------------
	/**
	 * Internal call: Get transform owned by gameObject.
	 * 
	 * @param _chachedPtr native gameObject handling the C# object instance
	 * @return pointer about transform C# object
	 */
	static MonoObject* Internal_GetTransform(GameObject* _chachedPtr);

	/**
	 * Internal call: Get components owned by gameObject.
	 *
	 * @param _chachedPtr native gameObject handling the C# object instance
	 * @return pointer about components array C# object
	 */
	static MonoArray* Internal_GetComponents(GameObject* _chachedPtr);

	/**
	 * Internal call: Get components owned by gameObject.
	 *
	 * @param _chachedPtr native gameObject handling the C# object instance
	 * @return pointer about components array C# object
	 */
	static MonoArray* Internal_GetAll(GameObject* _chachedPtr);


	static bool Internal_GetEnabled(GameObject* _chachedPtr);
	static void Internal_SetEnabled(GameObject* _chachedPtr, bool _enabled);
};
}
