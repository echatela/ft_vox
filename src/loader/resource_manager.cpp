
#include "resource_manager.hpp"

#include "render/shader.hpp"
#include "render/texture.hpp"

#include <iostream>

void ResourceManager::_preload()
{
	for (ResourceInfo res : data)
	{
		if (_resources.find(res.id) != _resources.end())
			throw std::runtime_error("ResourceManager: resource already loaded");

		Resource *resourcePtr = nullptr;

		switch (res.type)
		{
			case ResourceType::SHADER:
			{
				resourcePtr = new Shader(res.paths[0], res.paths[1]);
				break ;
			}
			case ResourceType::TEXTURE:
			{
				resourcePtr = new Texture(res.paths[0]);
				break ;
			}
			default:
			{
				throw std::runtime_error("ResourceManager: invalid type");
			}
		}
		_resources[res.id] = resourcePtr;
	}
}

ResourceManager::ResourceManager()
{
	_preload();
}

ResourceManager::~ResourceManager()
{
	for (std::pair<ResourceId, Resource*> resource : _resources)
	{
		delete resource.second;
	}
}

ResourceManager& ResourceManager::instance()
{
	if (!inst)
	{
		inst = new ResourceManager();
	}
	return *inst;
}

void	ResourceManager::destroy()
{
	if (inst)
	{
		delete inst;
		inst = nullptr;
	}
}

