
#include "resource_manager.hpp"

#include <iostream>

ResourceManager::ResourceManager()
{
	std::cout << "Constructor of ResourceManager" << std::endl;
	// _preload();
}

ResourceManager::~ResourceManager()
{
	std::cout << "Destructor of ResourceManager" << std::endl;
	// _clear();
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

