
#include "resource_manager.hpp"

#include "render/shader.hpp"
#include "render/texture_2d.hpp"
#include "render/texture_2d_array.hpp"
#include <string>

void ResourceManager::_preload()
{
	for (const ResourceInfo& res : data)
	{
		if (_resources.find(res.id) != _resources.end())
			throw std::runtime_error(
			    "ResourceManager: resource already loaded");

		Resource* resourcePtr = nullptr;

		switch (res.type)
		{
		case ResourceType::SHADER:
		{
			resourcePtr = new Shader(res.paths[0], res.paths[1]);
			break;
		}
		case ResourceType::TEXTURE_2D:
		{
			resourcePtr = new Texture2D(res.paths[0]);
			break;
		}
		case ResourceType::TEXTURE_ARRAY_2D:
		{
			std::vector<std::string> files;

			for (const char* path : res.paths)
			{
				if (path)
					files.emplace_back(path);
			}
			resourcePtr =
			    new Texture2DArray(files, res.width, res.height, res.alpha);
			break;
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

void ResourceManager::destroy()
{
	if (inst)
	{
		delete inst;
		inst = nullptr;
	}
}
