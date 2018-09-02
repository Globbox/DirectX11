#version 330                                                                         
                                                                                     
const int MAX_POINT_LIGHTS = 2;                                                      
const int MAX_SPOT_LIGHTS = 2;                                                       
                                                                                     
in vec4 LightSpacePos;                                                               
in vec2 TexCoord0;                                                                   
in vec3 Normal0;                                                                     
in vec3 WorldPos0;                                                                   
in vec3 Tangent0; 
in vec3 ViewPos;
in vec3 FragPos;                                                                   
                                                                                     
out vec4 FragColor;                                                                  
                                                                                     
struct BaseLight                                                                     
{                                                                                    
    vec3 Color;                                                                      
    float AmbientIntensity;                                                          
    float DiffuseIntensity;                                                          
};                                                                                   
                                                                                     
struct DirectionalLight                                                              
{                                                                                    
    struct BaseLight Base;                                                           
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
    struct BaseLight Base;                                                                   
    vec3 Position;                                                                           
    Attenuation Atten;                                                                       
};                                                                                           
                                                                                             
struct SpotLight                                                                             
{                                                                                            
    struct PointLight Base;                                                                  
    vec3 Direction;                                                                          
    float Cutoff;                                                                            
};                                                                                           
                                                                                             
uniform int Point;                                                                 
uniform int Spot;                                                                  
uniform DirectionalLight MainLight;  
uniform float HeightScale;
uniform float SpecInten;                                                         
uniform float SpecPower; 


uniform PointLight Points[MAX_POINT_LIGHTS];                                           
uniform SpotLight Spots[MAX_SPOT_LIGHTS];                                              
uniform sampler2D DiffuseMap;                                                                                                                                
uniform sampler2D NormalMap; 
uniform sampler2D DepthMap;                                                               
uniform vec3 Camera;                                                                   
uniform float Alfa;                                                                 
                                                                                             
                                                                                                                                                                                      
vec4 CalcLightInternal( BaseLight Light, vec3 LightDirection, vec3 Normal)             
{                                                                                            
    vec4 AmbientColor = vec4(Light.Color, 1.0f) * Light.AmbientIntensity;                    
    float DiffuseFactor = dot(Normal, -LightDirection);                                      
                                                                                             
    vec4 DiffuseColor  = vec4(0, 0, 0, 0);                                                   
    vec4 SpecularColor = vec4(0, 0, 0, 0);                                                   
                                                                                             
    if (DiffuseFactor > 0) {                                                                 
        DiffuseColor = vec4(Light.Color, 1.0f) * Light.DiffuseIntensity * DiffuseFactor;     
                                                                                             
        vec3 VertexToEye = normalize(Camera - WorldPos0);                              
        vec3 LightReflect = normalize(reflect(LightDirection, Normal));                      
        float SpecularFactor = dot(VertexToEye, LightReflect);                               
        SpecularFactor = pow(SpecularFactor, SpecPower);                                
        if (SpecularFactor > 0) {                                                            
            SpecularColor = vec4(Light.Color, 1.0f) *                                        
                            SpecInten * SpecularFactor;                          
        }                                                                                    
    }                                                                                        
                                                                                             
    return (AmbientColor + DiffuseColor + SpecularColor);                                    
}                                                                                            
                                                                                             
vec4 CalcDirectionalLight(vec3 Normal)                                                       
{                                                                                            
    return CalcLightInternal(MainLight.Base, MainLight.Direction, Normal);   
}                                                                                            
                                                                                             
vec4 CalcPointLight( PointLight l, vec3 Normal)                                        
{                                                                                            
    vec3 LightDirection = WorldPos0 - l.Position;                                            
    float Distance = length(LightDirection);                                                 
    LightDirection = normalize(LightDirection);                                              
                                                                                             
    vec4 Color = CalcLightInternal(l.Base, LightDirection, Normal);                          
    float Attenuation =  l.Atten.Constant +                                                  
                         l.Atten.Linear * Distance +                                         
                         l.Atten.Exp * Distance * Distance;                                  
                                                                                             
    return Color / Attenuation;                                                              
}                                                                                            
                                                                                             
vec4 CalcSpotLight( SpotLight l, vec3 Normal)                                          
{                                                                                            
    vec3 LightToPixel = normalize(WorldPos0 - l.Base.Position);                              
    float SpotFactor = dot(LightToPixel, l.Direction);                                       
                                                                                             
    if (SpotFactor > l.Cutoff) {                                                             
        vec4 Color = CalcPointLight(l.Base, Normal);                                         
        return Color * (1.0 - (1.0 - SpotFactor) * 1.0/(1.0 - l.Cutoff));                    
    }                                                                                        
    else {                                                                                   
        return vec4(0,0,0,0);                                                                
    }                                                                                        
}                                                                                            
                                                                                           
    





void main()                                                                                  
{                                                                                            
    vec3 Normal = Normal0;                                                      
    vec4 TotalLight = CalcDirectionalLight(Normal);                                          
                                                                                             
    for (int i = 0 ; i < Point ; i++) {                                            
        TotalLight += CalcPointLight(Points[i], Normal);                
    }                                                                                        
                                                                                             
    for (int i = 0 ; i < Spot ; i++) {                                             
        TotalLight += CalcSpotLight(Spots[i], Normal);                  
    }                                                                                        


    vec4 SampledColor = texture2D(DiffuseMap, TexCoord0);                                  
    FragColor = vec4(vec3(SampledColor * TotalLight),Alfa);                                                   
}