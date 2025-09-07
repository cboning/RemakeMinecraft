#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;


uniform mat4 projViewMatrix;
uniform vec3 pos;
uniform vec2 screenSize;
uniform float linewidth;

void main()
{
    vec4 linePosStart = projViewMatrix * vec4(aPos + pos, 1.0);
    vec4 linePosEnd = projViewMatrix * vec4(pos + aNormal, 1.0);

    vec3 ndc1 = linePosStart.xyz / linePosStart.w;
    vec3 ndc2 = linePosEnd.xyz / linePosEnd.w;

    vec2 lineScreenDirection = normalize((ndc2.xy - ndc1.xy) * screenSize);
    vec2 lineOffset = vec2(-lineScreenDirection.y, lineScreenDirection.x) * linewidth / screenSize;


    if (gl_VertexID % 2 == 0) {
        gl_Position = vec4((ndc1 + vec3(lineOffset, -0.0005)) * linePosStart.w, linePosStart.w);
    } else {
        gl_Position = vec4((ndc1 - vec3(lineOffset, 0.0005)) * linePosStart.w, linePosStart.w);
    }
    //gl_Position /= gl_Position.w; // 透视除法
}