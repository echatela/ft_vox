#pragma once

enum class RESOURCE_TYPE {
	RES_SHADER,
	RES_TEXTURE
};

class Resource {

	public :
		
		virtual ~Resource() = default;

};