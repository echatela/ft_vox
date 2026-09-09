#pragma once

#include "resource.hpp"

#include <string>
#include <vector>
#include <map>

enum class RESOURCE_ID {
	RES_SHADER_2D,
	RES_SHADER_CHUNK,
	RES_SHADER_CONTROL,
	RES_TEXTURE_FONT,
	RES_TEXTURE_BLOCK_COBBLESTONE,
	RES_TEXTURE_BLOCK_STONE,
	RES_TEXTURE_BLOCK_DIRT,
};

constexpr const unsigned int kMaxPaths = 2;

struct ResourceInfo {
	RESOURCE_ID   id;
	RESOURCE_TYPE type;
	const char* paths[kMaxPaths] = {};
};

constexpr ResourceInfo data[] = {

	{	
		RESOURCE_ID::RES_SHADER_2D,
		RESOURCE_TYPE::RES_SHADER, 
		{"shaders/2d_vert.glsl", "shaders/2d_frag.glsl"}
	},
	{	
		RESOURCE_ID::RES_SHADER_CHUNK,
		RESOURCE_TYPE::RES_SHADER,
		{"shaders/chunk_vert.glsl", "shaders/chunk_frag.glsl"}
	},
	{	RESOURCE_ID::RES_SHADER_CONTROL,
		RESOURCE_TYPE::RES_SHADER,
		{"shaders/control_vert.glsl", "shaders/control_frag.glsl"}
	},
	{
		RESOURCE_ID::RES_TEXTURE_FONT,
		RESOURCE_TYPE::RES_TEXTURE,
		{"assets/font/texture_mipmap_font.png"}
	},
	{
		RESOURCE_ID::RES_TEXTURE_BLOCK_COBBLESTONE,
		RESOURCE_TYPE::RES_TEXTURE,
		{"assets/block/cobblestone.png"}
	},
	{
		RESOURCE_ID::RES_TEXTURE_BLOCK_STONE,
		RESOURCE_TYPE::RES_TEXTURE,
		{"assets/block/stone.png"}
	},
	{
		RESOURCE_ID::RES_TEXTURE_BLOCK_DIRT,
		RESOURCE_TYPE::RES_TEXTURE,
		{"assets/block/dirt.png"}
	},

};

class ResourceManager {

	private:

		inline static ResourceManager* instance = nullptr;

		std::map<RESOURCE_ID, Resource *> resources;

		ResourceManager();
		~ResourceManager();

		void _preload();

		
		public:
		
		static ResourceManager& instanciate();
		static void				destroy();
		
		template <typename T>
		const T* get(RESOURCE_ID id) const;

		ResourceManager(const ResourceManager&) = delete;
		ResourceManager& operator=(const ResourceManager&) = delete;

};

#include "resource_manager.tpp"

class ResourceManagerGuard {

	public:
	
	ResourceManagerGuard() {ResourceManager::instanciate();};
	~ResourceManagerGuard() {ResourceManager::destroy();};

};