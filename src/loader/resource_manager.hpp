#pragma once

#include "resource.hpp"

#include <map>

enum class ResourceId {
	SHADER_2D,
	SHADER_CHUNK,
	SHADER_CONTROL,
	TEXTURE_FONT,
	TEXTURE_BLOCK_COBBLESTONE,
	TEXTURE_BLOCK_STONE,
	TEXTURE_BLOCK_DIRT,
};

constexpr const unsigned int kMaxPaths = 2;

struct ResourceInfo {
	ResourceId   id;
	ResourceType type;
	const char* paths[kMaxPaths] = {};
};

constexpr ResourceInfo data[] = {

	{
		ResourceId::SHADER_2D,
		ResourceType::SHADER,
		{"shaders/2d_vert.glsl", "shaders/2d_frag.glsl"}
	},
	{
		ResourceId::SHADER_CHUNK,
		ResourceType::SHADER,
		{"shaders/chunk_vert.glsl", "shaders/chunk_frag.glsl"}
	},
	{	ResourceId::SHADER_CONTROL,
		ResourceType::SHADER,
		{"shaders/control_vert.glsl", "shaders/control_frag.glsl"}
	},
	{
		ResourceId::TEXTURE_FONT,
		ResourceType::TEXTURE,
		{"assets/font/texture_mipmap_font.png"}
	},
	{
		ResourceId::TEXTURE_BLOCK_COBBLESTONE,
		ResourceType::TEXTURE,
		{"assets/block/cobblestone.png"}
	},
	{
		ResourceId::TEXTURE_BLOCK_STONE,
		ResourceType::TEXTURE,
		{"assets/block/stone.png"}
	},
	{
		ResourceId::TEXTURE_BLOCK_DIRT,
		ResourceType::TEXTURE,
		{"assets/block/dirt.png"}
	},

};

class ResourceManager {

	private:

		inline static ResourceManager* inst = nullptr;

		std::map<ResourceId, Resource *> _resources;

		ResourceManager();
		~ResourceManager();

		void _preload();

	public:

		static ResourceManager& instance();
		static void				destroy();

		template <typename T>
		const T* get(ResourceId id) const;

		ResourceManager(const ResourceManager&) = delete;
		ResourceManager& operator=(const ResourceManager&) = delete;

};

#include "resource_manager.tpp"

class ResourceManagerGuard {

	public:

	ResourceManagerGuard() {ResourceManager::instance();};
	~ResourceManagerGuard() {ResourceManager::destroy();};

};
