

#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

out VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
	vec4 LightSpacePos; 
	vec3 Normal0; 
	vec3 Tangent0;
} vs_out;

uniform mat4 gWVP;
uniform mat4 gWorld;
uniform mat4 gLightWVP; 

//uniform vec3 lightPos;
uniform vec3 gEyeWorldPos;

void main()
{
	vs_out.Normal0 = aNormal;
	vs_out.Tangent0 =aTangent;
    gl_Position      = gWVP * vec4(aPos, 1.0);
    vs_out.FragPos   = vec3(gWorld * vec4(aPos, 1.0));   
    vs_out.TexCoords = aTexCoords;    
    vs_out.LightSpacePos = gLightWVP * vec4(aPos, 1.0);   
    
    vec3 T   = normalize(mat3(gWorld) * aTangent);
    vec3 B   = normalize(mat3(gWorld) * aBitangent);
    vec3 N   = normalize(mat3(gWorld) * aNormal);

    mat3 TBN = transpose(mat3(T, B, N));

   // vs_out.TangentLightPos = TBN * lightPos;
    vs_out.TangentViewPos  = TBN * normalize(gEyeWorldPos);
    vs_out.TangentFragPos  = TBN * vs_out.FragPos;
} 