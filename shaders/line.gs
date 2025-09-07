
#version 330 core
layout (points) in;
layout (line_strip, max_vertices = 2) out;

uniform vec2  screenSize;
uniform float u_thickness = 4;

void main()
{
    vec4 p1 = gl_in[0].gl_Position;
    vec4 p2 = gl_in[1].gl_Position;

    vec2 dir;
    if (abs(p1.w) > 1e-6 && abs(p2.w) > 1e-6) {
        dir = normalize((p2.xy/p2.w - p1.xy/p1.w) * screenSize);
    } else {
        // Handle special case to avoid division by zero
        dir = vec2(0.0);
    }

    vec2 offset;
    if (length(dir) > 1e-6) {
        offset = vec2(-dir.y, dir.x) * u_thickness / screenSize;
    } else {
        offset = vec2(0.0);
    }

    gl_Position = p1 + vec4(offset.xy * p1.w, 0.0, 0.0);
    EmitVertex();
    gl_Position = p1 - vec4(offset.xy * p1.w, 0.0, 0.0);
    EmitVertex();
    gl_Position = p2 + vec4(offset.xy * p2.w, 0.0, 0.0);
    EmitVertex();
    gl_Position = p2 - vec4(offset.xy * p2.w, 0.0, 0.0);
    EmitVertex();

    EndPrimitive();
}
