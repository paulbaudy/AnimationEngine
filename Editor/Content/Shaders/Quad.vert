#version 330 core
layout (location = 0) in vec3 aPos;
uniform vec3 aRes;

uniform vec3 SphereCenter;
uniform vec3 CameraCenter;
uniform vec3 CameraForward;

out vec3 iResolution;
out vec3 FragSphereCenter;
out vec3 FragCameraCenter;
out vec3 FragCameraForward;
out vec3 TexCoords;


const vec2 quadVertices[4] = vec2[](vec2(-1.0, -1.0), vec2(1.0, -1.0), vec2(-1.0, 1.0), vec2(1.0, 1.0));

void main()
{
    //gl_Position = vec4(quadVertices[gl_VertexID], 0.0, 1.0);
    gl_Position = vec4(aPos, 1.0);
    TexCoords = aPos;
    iResolution = aRes;

    FragSphereCenter = SphereCenter;
    FragCameraCenter = CameraCenter;
    FragCameraForward = CameraForward;
}