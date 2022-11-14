#pragma once
#include <nlohmann/json.hpp>

namespace Miyadaiku
{
class IPCCommand
{
public:
	IPCCommand(const nlohmann::ordered_json& _data);
	~IPCCommand() = default;

	/**
	 * @brief Excecute command.
	 * 
	 * @return responce json string
	 */
	virtual std::string Execute() = 0;

protected:
	const nlohmann::ordered_json m_data;
};
} // namespace Miyadaiku