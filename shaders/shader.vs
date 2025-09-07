#version 330

layout(location = 0) in vec3  inVertexPosition;
layout(location = 1) in vec4  acolor;
layout(location = 2) in vec2  inTextureCoord;

out vec2 passTextureCoord;
out vec4 color;

uniform mat4 projViewMatrix;

void main()
{
    gl_Position = projViewMatrix * vec4(inVertexPosition, 1.0);

    passTextureCoord    = inTextureCoord;
    color = acolor-vec4(6.0/15.0,6.0/15.0,6.0/15.0,0.0);
}
