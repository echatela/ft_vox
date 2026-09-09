#include "resource_manager.hpp"

#include <stdexcept>

template <typename T>
const T*	ResourceManager::get(RESOURCE_ID id) const
{
	auto result = resources.find(id);

	if (result == resources.end())
		throw std::runtime_error("ResourceManager: Tried to get an inexistant resource");
	
	return (static_cast<T *>(result->second));
}