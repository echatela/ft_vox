#pragma once

enum class ResourceType {
	SHADER,
	TEXTURE_2D,
	TEXTURE_2D_ARRAY,
};

class Resource {

	public :

		virtual ~Resource() = default;

};
