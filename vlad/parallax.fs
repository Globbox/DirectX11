#version 330 core

const int MAX_POINT_LIGHTS = 2;                                                      
const int MAX_SPOT_LIGHTS = 2;  

struct BaseLight                                                                     
{                                                                                    
    vec3 Color;                                                                      
    float AmbientIntensity;                                                          
    float DiffuseIntensity;                                                          
};                                                                                   
                                                                                     
struct DirectionalLight                                                              
{                                                                                    
    BaseLight Base;                                                           
    vec3 Direction;                                                                  
};                                                                                   
                                                                                     
struct Attenuation                                                                   
{                                                                                    
    float Constant;                                                                  
    float Linear;                                                                    
    float Exp;                                                                       
};                                                                                   
                                                                                     
struct PointLight                                                                            
{                                                                                            
    BaseLight Base;                                                                   
    vec3 Position;                                                                           
    Attenuation Atten;                                                                       
};                                                                                           
                                                                                             
struct SpotLight                                                                             
{                                                                                            
    PointLight Base;                                                                  
    vec3 Direction;                                                                          
    float Cutoff;                                                                            
};       



out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
	vec4 LightSpacePos;  
	vec3 Normal0; 
	vec3 Tangent0;
} fs_in;


uniform int gNumPointLights;                                                                 
uniform int gNumSpotLights;                                                                  
uniform DirectionalLight gDirectionalLight;                                                  
uniform PointLight gPointLights[MAX_POINT_LIGHTS];                                           
uniform SpotLight gSpotLights[MAX_SPOT_LIGHTS];                                                                                                         
uniform vec3 gEyeWorldPos;                                                                   
uniform float gMatSpecularIntensity;                                                         
uniform float gSpecularPower;       

uniform sampler2D gColorMap;
uniform sampler2D gHightMap;
uniform sampler2D gNormalMap;
  


uniform float heit;    
  

                                                                                         
                                                                                         
                                                                                             
vec4 CalcLightInternal(BaseLight Light, vec3 LightDirection, vec3 Normal,             
                       float ShadowFactor)                                                   
{                                                                                            
    vec4 AmbientColor = vec4(Light.Color, 1.0f) * Light.AmbientIntensity;                    
    float DiffuseFactor = dot(Normal, -LightDirection);                                      
                                                                                             
    vec4 DiffuseColor  = vec4(0, 0, 0, 0);                                                   
    vec4 SpecularColor = vec4(0, 0, 0, 0);                                                   
                                                                                             
    if (DiffuseFactor > 0) {                                                                 
        DiffuseColor = vec4(Light.Color, 1.0f) * Light.DiffuseIntensity * DiffuseFactor;     
                                                                                             
        vec3 VertexToEye = normalize(gEyeWorldPos - fs_in.FragPos);                              
        vec3 LightReflect = normalize(reflect(LightDirection, Normal));                      
        float SpecularFactor = dot(VertexToEye, LightReflect);                               
        SpecularFactor = pow(SpecularFactor, gSpecularPower);                                
        if (SpecularFactor > 0) {                                                            
            SpecularColor = vec4(Light.Color, 1.0f) *                                        
                            gMatSpecularIntensity * SpecularFactor;                          
        }                                                                                    
    }                                                                                        
                                                                                             
    return (AmbientColor + ShadowFactor * (DiffuseColor + SpecularColor));                   
}                                                                                            
                                                                                             
vec4 CalcDirectionalLight(vec3 Normal)                                                       
{                                                                                                 
    return CalcLightInternal(gDirectionalLight.Base, gDirectionalLight.Direction, Normal, 1.0);   
}                                                                                                 
                                                                                             
vec4 CalcPointLight(PointLight l, vec3 Normal, vec4 LightSpacePos)                    
{                                                                                            
    vec3 LightDirection = fs_in.FragPos - l.Position;                                            
    float Distance = length(LightDirection);                                                 
    LightDirection = normalize(LightDirection);                                              
    float ShadowFactor = 0.0;                                    
                                                                                             
    vec4 Color = CalcLightInternal(l.Base, LightDirection, Normal, ShadowFactor);            
    float Attenuation =  l.Atten.Constant +                                                  
                         l.Atten.Linear * Distance +                                         
                         l.Atten.Exp * Distance * Distance;                                  
                                                                                             
    return Color / Attenuation;                                                              
}                                                                                            
                                                                                             
vec4 CalcSpotLight(SpotLight l, vec3 Normal, vec4 LightSpacePos)                      
{                                                                                            
    vec3 LightToPixel = normalize(fs_in.FragPos - l.Base.Position);                              
    float SpotFactor = dot(LightToPixel, l.Direction);                                       
                                                                                             
    if (SpotFactor > l.Cutoff) {                                                             
        vec4 Color = CalcPointLight(l.Base, Normal, LightSpacePos);                          
        return Color * (1.0 - (1.0 - SpotFactor) * 1.0/(1.0 - l.Cutoff));                    
    }                                                                                        
    else {                                                                                   
        return vec4(0,0,0,0);                                                                
    }                                                                                        
}                                                                                            
                                                                                             
vec3 CalcBumpedNormal()                                                                      
{                                                                                            
    vec3 Normal = normalize(fs_in.Normal0);                                                        
    vec3 Tangent = normalize(fs_in.Tangent0);                                                      
    Tangent = normalize(Tangent - dot(Tangent, Normal) * Normal);                            
    vec3 Bitangent = cross(Tangent, Normal);                                                 
    vec3 BumpMapNormal = texture(gNormalMap, fs_in.TexCoords).xyz;                                 
    BumpMapNormal = 2.0 * BumpMapNormal - vec3(1.0, 1.0, 1.0);                               
    vec3 NewNormal;                                                                          
    mat3 TBN = mat3(Tangent, Bitangent, Normal);                                             
    NewNormal = TBN * BumpMapNormal;                                                         
    NewNormal = normalize(NewNormal);                                                        
    return NewNormal;                                                                        
}                      


vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir)
{ 
    // number of depth layers
    const float minLayers = 8;
    const float maxLayers = 32;
    float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0, 0.0, 1.0), viewDir)));  
    // calculate the size of each layer
    float layerDepth = 1.0 / numLayers;
    // depth of current layer
    float currentLayerDepth = 0.0;
    // the amount to shift the texture coordinates per layer (from vector P)
    vec2 P = viewDir.xy / viewDir.z * heit; 
    vec2 deltaTexCoords = P / numLayers;
  
    // get initial values
    vec2  currentTexCoords     = texCoords;
    float currentDepthMapValue = texture(gHightMap, currentTexCoords).r;
      
    while(currentLayerDepth < currentDepthMapValue)
    {
        // shift texture coordinates along direction of P
        currentTexCoords -= deltaTexCoords;
        // get depthmap value at current texture coordinates
        currentDepthMapValue = texture(gHightMap, currentTexCoords).r;  
        // get depth of next layer
        currentLayerDepth += layerDepth;  
    }
    
    // get texture coordinates before collision (reverse operations)
    vec2 prevTexCoords = currentTexCoords + deltaTexCoords;

    // get depth after and before collision for linear interpolation
    float afterDepth  = currentDepthMapValue - currentLayerDepth;
    float beforeDepth = texture(gHightMap, prevTexCoords).r - currentLayerDepth + layerDepth;
 
    // interpolation of texture coordinates
    float weight = afterDepth / (afterDepth - beforeDepth);
    vec2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);

    return finalTexCoords;
}

  
void main()
{         

vec3 Normal = CalcBumpedNormal();                                                        
    vec4 TotalLight = CalcDirectionalLight(Normal);                                          
                                                                                             
    for (int i = 0 ; i < gNumPointLights ; i++) {                                            
        TotalLight += CalcPointLight(gPointLights[i], Normal, fs_in.LightSpacePos);                
    }                                                                                        
                                                                                             
    for (int i = 0 ; i < gNumSpotLights ; i++) {                                             
        TotalLight += CalcSpotLight(gSpotLights[i], Normal, fs_in.LightSpacePos);                  
    }      
    // offset texture coordinates with Parallax Mapping
    vec3 viewDir   = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
	vec2 texCoords = fs_in.TexCoords;

    texCoords = ParallaxMapping(fs_in.TexCoords,  viewDir);
	    
           
    if(texCoords.x > 1.0 || texCoords.y > 1.0 || texCoords.x < 0.0 || texCoords.y < 0.0)
        discard;
    // then sample textures with new texture coords
    vec4 diffuse = texture2D(gColorMap, texCoords);
   // vec3 normal  = texture(normalMap, texCoords);
   // normal = normalize(normal * 2.0 - 1.0);
	  FragColor =  diffuse * TotalLight;
}