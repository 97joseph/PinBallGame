#version 330
in vec2 Texcoord;
out vec4 outColor;
uniform sampler2D texFramebuffer;
uniform sampler2D addTarget;

void main()
{
    outColor = texture(texFramebuffer, Texcoord) + texture(addTarget, Texcoord);
}