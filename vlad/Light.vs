#version 330                                                                         
                                                                                     
layout (location = 0) in vec3 Position;                                              
layout (location = 1) in vec2 TexCoord;                                              
layout (location = 2) in vec3 Normal;                                                
layout (location = 3) in vec3 Tangent;  
layout (location = 4) in vec3 Bitangent;                                              
                                                                                     
uniform mat4 MVP;                                                                                                   
uniform mat4 Model;  
uniform vec3 Camera;                                                               
                                                                                                                                                   
out vec2 TexCoord0;                                                                  
out vec3 Normal0;                                                                    
out vec3 WorldPos0;                                                                  
out vec3 Tangent0; 
out vec3 ViewPos;
out vec3 FragPos;                                                                  
                                                                                     
void main()                                                                          
{      

    gl_Position   = MVP * vec4(Position, 1.0);                                                                   
    TexCoord0     = TexCoord;                                                        
    Normal0       = (Model * vec4(Normal, 0.0)).xyz;                                
    Tangent0      = (Model * vec4(Tangent, 0.0)).xyz;                               
    WorldPos0     = (Model * vec4(Position, 1.0)).xyz;  
    vec3 T   = normalize(mat3(Model) * Tangent);
    vec3 B   = normalize(mat3(Model) * Bitangent);
    vec3 N   = normalize(mat3(Model) * Normal);
    mat3 TBN = transpose(mat3(T, B, N)); 
    ViewPos  = TBN * Camera;
    FragPos  = TBN * WorldPos0; 

}