#include "resource_manager.hpp"

#include <stdexcept>

template <typename T>
const T*	ResourceManager::get(ResourceId id) const
{
	auto result = _resources.find(id);

	if (result == _resources.end())
		throw std::runtime_error("ResourceManager: Tried to get an inexistant resource");

	return (static_cast<T *>(result->second));
}
