#pragma once

enum class ResourceType {
	SHADER,
	TEXTURE
};

class Resource {

	public :
		
		virtual ~Resource() = default;

};