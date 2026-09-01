#version 460 core
in vec2 TexCoord;
flat in int id;

out vec4 FragColor;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
    FragColor = vec4(1.0f,1.0f,1.0f,1.0f);
    if (id == 1)
        FragColor = texture(texture1, TexCoord);
    if (id == 2)
        FragColor = texture(texture2, TexCoord);
}
