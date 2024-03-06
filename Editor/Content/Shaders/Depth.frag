#version 330 core

out vec4 FragColor;
in vec3 TexCoords;
uniform sampler2D colorImage;

float LinearizeDepth(in vec2 uv)
{
    float zNear = 0.01;
    float zFar  = 100.0;
    float depth = texture2D(colorImage, uv).x;
    return (2.0 * zNear) / (zFar + zNear - depth * (zFar - zNear));
}

void main()
{
    float c = LinearizeDepth(vec2(TexCoords));
    FragColor = vec4(c, c, c, 1.0);
    //FragColor = vec4(0, 1, 1, 1.0);
    //FragColor = texture2D(colorImage, vec2(TexCoords));
}
