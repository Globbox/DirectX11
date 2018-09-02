#version 330                                                                         
                                                                                     
                                                            
in vec2 TexCoord0;                                                                   
out vec4 FragColor;                                           
//uniform sampler2D DiffuseMap;                                                                                                                                
                                                                 
                                                               
                                                                    
                                                                                             
void main()                                                                                  
{                                                                                            
                                                                                      
    vec3 col = vec3(1.0f,1.0f,1.0f);                                                                                      
   // vec3 SampledColor = (texture2D(DiffuseMap, TexCoord0.xy)).xyz;                                  
    FragColor = vec4(col,0.3f);                                                   
}