#pragma once
#include <memory>
#include <unordered_map>

namespace Miyadaiku
{
class Subsystem
{
public:
	// Subsystem起動時
	virtual void OnAwake()
	{
	}
	// Subsystem除外時
	virtual void OnShutdown()
	{
	}
};

class SubsystemLocator
{
public:
	SubsystemLocator() = default;
	~SubsystemLocator()
	{
	}

	template <class T> void Add()
	{
		CertificateSubsystemType<T>();
		std::shared_ptr<T> spSubsystem = std::make_shared<T>();
		if (!spSubsystem)
		{
			return;
		}
		spSubsystem->OnAwake();
		m_subsystems[typeid(T).hash_code()] = spSubsystem;
	}

	template <class T> std::shared_ptr<T> Get()
	{
		CertificateSubsystemType<T>();
		if (m_subsystems.find(typeid(T).hash_code()) == m_subsystems.end())
		{
			return nullptr;
		}
		return std::static_pointer_cast<T>(m_subsystems[typeid(T).hash_code()]);
	}

	template <class T> void Remove()
	{
		CertificateSubsystemType<T>();
		auto it = m_subsystems.find(typeid(T).hash_code());
		if (it == m_subsystems.end())
		{
			return;
		}
		(*it).second->OnShutdown();
		m_subsystems.erase(it);
	}

private:
	std::unordered_map<size_t, std::shared_ptr<Subsystem>> m_subsystems;

	template <class T>
	void CertificateSubsystemType()
	{
		static_assert(std::is_base_of<Subsystem, T>::value,
					  "Subsystemではない型が指定されました。Subsystemを継承した型を指定してください。");
	}
};
} // namespace Miyadaiku
