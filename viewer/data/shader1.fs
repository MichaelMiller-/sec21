#version 330 core

out vec4 FragColor;

struct Material {
   vec3 ambient;
   vec3 diffuse;
   vec3 specular;
   float shininess;
}; 

struct Light {
   vec3 position;
   vec3 ambient;
   vec3 diffuse;
   vec3 specular;
};

struct Camera {
   vec3 position;
};

in vec3 Position;
in vec3 Normal;
in vec2 TextureCoordinate;

uniform Camera camera;
uniform Material material;
uniform Light light;  
// uniform sampler2D texture1;
// uniform sampler2D texture2;

void main()
{
   // ambient
   vec3 ambient = light.ambient * material.ambient;

   // diffuse 
   vec3 norm = normalize(Normal);
   vec3 lightDir = normalize(light.position - Position);
   float diff = max(dot(norm, lightDir), 0.0);
   vec3 diffuse = light.diffuse * (diff * material.diffuse);
   
   // specular
   vec3 viewDir = normalize(camera.position - Position);
   vec3 reflectDir = reflect(-lightDir, norm);  
   float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
   vec3 specular = light.specular * (spec * material.specular);  
      
   vec3 result = ambient + diffuse + specular;
   FragColor = vec4(result, 1.0);

   // FragColor = mix(texture(texture1, TextureCoordinate), texture(texture2, TextureCoordinate), 0.2);
}
