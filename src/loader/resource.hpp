#pragma once

enum class ResourceType {
	SHADER,
	TEXTURE_2D,
	TEXTURE_ARRAY_2D,
};

class Resource {

	public :

		virtual ~Resource() = default;

};
