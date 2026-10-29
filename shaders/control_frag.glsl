#version 460 core
out vec4 FragColor;

in vec2 UV;

uniform sampler2D myTexture;
uniform vec3 myColor;

void main()
{
	FragColor = texture(myTexture, UV);
}
