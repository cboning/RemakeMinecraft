#version 330

out vec4 FragColor;
in  vec2 passTextureCoord;
in vec4 color;

uniform sampler2D texture1;

vec4 color1;


void main()
{
    color1 = texture(texture1, passTextureCoord);

    FragColor = vec4(color1.rgb * 0.8, 1.0f) * (vec4( 0.05 + color.rgb * 0.9, 1.0f));
    if (FragColor.a == 0) discard;
}
