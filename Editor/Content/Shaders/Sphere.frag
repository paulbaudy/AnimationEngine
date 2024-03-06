#version 330 core

in vec3 iResolution;
in vec3 FragSphereCenter;
in vec3 FragCameraCenter;
in vec3 FragCameraForward;
in mat4 FragView;

out vec4 FragColor;

float sphereIntersect(vec3 ray, vec3 dir, vec3 center, float radius)
{
 vec3 rc = ray-center;
 float c = dot(rc, rc) - (radius*radius);
 float b = dot(dir, rc);
 float d = b*b - c;
 float t = -b - sqrt(abs(d));
 float st = step(0.0, min(t,d));
 return mix(-1.0, t, st);
}



void main()
{
    vec3 SphereCenter = FragSphereCenter;//vec3(0.0,0.0,10.0);
float SphereRadius = 5.0;

vec3 CameraCenter = FragCameraCenter; //vec3(0.0,0.0, -3.0);
float imagePlaneDist = 1.0;

vec3 lightPos = vec3(10.0, 5.0, -1.0);

 // Normalized pixel coordinates (from 0 to 1)
 vec2 uv = gl_FragCoord.xy /iResolution.xy;
 
 //Center from -1 to 1
 uv = (uv * 2.0) - 1.0;
 
 //fix aspect ratio
 uv.x *= iResolution.x / iResolution.y;
 
 //Ray starts at camera center
 vec3 rayOrigin = CameraCenter;
 //Ray moves towards current pixel on image plane
 vec3 imagePlanePixel = vec3(uv.x,uv.y,CameraCenter.z) + FragCameraForward;
 
 //Calculate ray
 //vec3 rayDirection = normalize(imagePlanePixel - rayOrigin);
 vec3 rayDirection = normalize(imagePlanePixel - rayOrigin);

//Intersect ray with sphere
 float t = sphereIntersect(rayOrigin, rayDirection, SphereCenter, SphereRadius);
 
 //hitpoint
 vec3 hitPoint = rayOrigin + rayDirection * t;

t = clamp(t, 0.0, 1.0);

//Sphere normal is a vector pointing outward from center, to intersection point
vec3 normal = normalize(hitPoint - SphereCenter);
//vec3 normal = normalize(SphereCenter - hitPoint);

//The percentage this point is looking towards the light (lamberts law)
float lightLambert = max(dot(normal, normalize(lightPos - hitPoint)) , 0.0); 


// Output to screen
vec3 sphereColor = vec3(1.0,0.1,0.1);

FragColor = vec4(sphereColor * t * lightLambert, 1.0);
gl_FragDepth = 1 - t;
}