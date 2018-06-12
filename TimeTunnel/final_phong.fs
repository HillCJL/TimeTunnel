#version 330 core
out vec4 FragColor;
in vec3 NVector;
in vec3 FragPos;
in vec2 TexCoords;

uniform float ambient_strength;
uniform float diffuse_strength;
uniform float specular_strength;
uniform float n_shiny;
uniform vec3 light_pos;
uniform vec3 view_pos;
uniform vec3 light_color;
uniform vec3 object_color;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_diffuse2;
uniform sampler2D texture_diffuse3;

uniform sampler2D texture_specular1;
uniform sampler2D texture_specular2;
uniform sampler2D texture_specular3;

uniform sampler2D texture_normal1;
uniform sampler2D texture_normal2;
uniform sampler2D texture_normal3;

uniform sampler2D texture_height1;
uniform sampler2D texture_height2;
uniform sampler2D texture_height3;



void main(){
	vec3 ambient = ambient_strength * light_color * texture(texture_normal1, TexCoords).rgb;

	vec3 NV = normalize(NVector);
	vec3 light_direction = normalize(light_pos - FragPos);
	vec3 diffuse = diffuse_strength * max(0.0, dot(NV, light_direction)) * light_color * texture(texture_diffuse1, TexCoords).rgb;

    vec3 view_direction = normalize(view_pos - FragPos);
    vec3 reflect_direction = reflect(-light_direction, NV);  
    vec3 specular = specular_strength * pow(max(dot(view_direction, reflect_direction), 0.0), n_shiny) * light_color * texture(texture_specular1, TexCoords).rgb;  

	FragColor = vec4((ambient + diffuse + specular) * object_color, 1.0);
};