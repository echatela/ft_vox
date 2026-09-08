#pragma once

#include <string>
#include <map>

enum class RESOURCES_ID {RES_SHADER_2D, RES_SHADER_CHUNK, RES_SHADER_CONTROL, RES_TEXTURE_FONT};

class Resource {

};

class ResourceManager {

	private:

		inline static ResourceManager* instance = nullptr;

		std::map<RESOURCES_ID, Resource *> resources;

		ResourceManager();
		~ResourceManager();

		void _preload();
		void _clear();
		
		public:
		
		static ResourceManager& instanciate();
		static void				destroy();
		
		Resource *load(const std::string &resource_id);

		ResourceManager(const ResourceManager&) = delete;
		ResourceManager& operator=(const ResourceManager&) = delete;

};

class ResourceManagerGuard {

	public:
	
	ResourceManagerGuard() {ResourceManager::instanciate();};
	~ResourceManagerGuard() {ResourceManager::destroy();};

};