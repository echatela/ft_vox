
#include "resource_manager.hpp"

#include "render/shader.hpp"
#include "render/texture.hpp"

#include <iostream>

void ResourceManager::_preload()
{
	for (ResourceInfo res : data)
	{
		if (resources.find(res.id) != resources.end())
			throw std::runtime_error("ResourceManager: resource already loaded");

		Resource *resourcePtr = nullptr;

		switch (res.type)
		{
			case RESOURCE_TYPE::RES_SHADER:
			{
				resourcePtr = new Shader(res.paths[0], res.paths[1]);
				break ;
			}
			case RESOURCE_TYPE::RES_TEXTURE:
			{
				resourcePtr = new Texture(res.paths[0]);
				break ;
			}
			default:
			{
				throw std::runtime_error("ResourceManager: invalid type");
			}
		}
		resources[res.id] = resourcePtr;
	}
}

ResourceManager::ResourceManager()
{
	std::cout << "Constructor of ResourceManager" << std::endl;
	_preload();
}

ResourceManager::~ResourceManager()
{
	for (std::pair<RESOURCE_ID, Resource*> resource : resources)
	{
		delete resource.second;
	}
	std::cout << "Destructor of ResourceManager" << std::endl;
}

ResourceManager& ResourceManager::instanciate()
{
	if (!instance)
	{
		instance = new ResourceManager();
	}
	return *instance;
}

void	ResourceManager::destroy()
{
	if (instance)
	{
		delete instance;
		instance = nullptr;
	}
}

