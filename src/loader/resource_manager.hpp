#pragma once

#include "resource.hpp"

#include <map>

enum class ResourceId
{
	SHADER_2D,
	SHADER_CHUNK,
	SHADER_CONTROL,
	TEXTURE_FONT,
	TEXTURE_BLOCKS,
};

constexpr const unsigned int kMaxPaths = 8;

struct ResourceInfo
{
	ResourceId   id;
	ResourceType type;
	const char*  paths[kMaxPaths] = {};
	unsigned int width = 0;
	unsigned int height = 0;
	bool         alpha = false;
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
		ResourceType::TEXTURE_2D,
		{"assets/font/texture_mipmap_font.png"}
	},
	{
		ResourceId::TEXTURE_BLOCKS,
		ResourceType::TEXTURE_ARRAY_2D,
		{"assets/block/stone.png",
		"assets/block/dirt.png",
		"assets/block/cobblestone.png",
		"assets/block/oak_planks.png"},
		16, 16, true
	}
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
