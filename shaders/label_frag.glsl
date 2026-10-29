#version 460 core
out vec4 FragColor;

in vec2 UV;

uniform sampler2D myTexture;
uniform vec3 myColor;

void main()
{
	float alpha = texture(myTexture, UV).a;
	FragColor = vec4(myColor, alpha);
}
